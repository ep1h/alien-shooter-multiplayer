# alien-shooter-multiplayer
Alien Shooter is a singleplayer isometric top-down shooter released in 2003. The contents of this repository are the result of reverse engineering and development in C, allowing to play this game over network.

## Navigation
- [Repository navigation](#Repository-navigation)
- [Build](#Build)
- [Injection in game process](#Injection-in-game-process)
- [Server-client architecture](#Server-client-architecture)
- [Demonstration](#Demonstration)

## Repository navigation
- [/asmp-dll](/asmp-dll) - client source code. The client is a .dll library, the injection of which into the game process changes the game logic so that it becomes possible to play over the network.
  - [/asmp-dll/src/game](/asmp-dll/src/game) - data types, addresses of functions and global objects obtained as a result of reverse engineering. There is almost no executing logic in this directory.
  - [/asmp-dll/src/dllmain.c](/asmp-dll/src/dllmain.c) - entry point. Everything starts here.
  - [/asmp-dll/src/utils/hook](/asmp-dll/src/utils/hook) - component for functions hooking.
- [/asmp-server](/asmp-server) - server source code. The server is a 32-bit Windows executable file.
- [/asmp-exe-patcher](/asmp-exe-patcher) - patcher source code. The patcher is a program that modifies the original AlienShooter.exe in such a way that, when launched, it automatically injects a client for playing over the network (asmp.dll). See section [Injection in game process](#Injection-in-game-process) for details.
- [/common](/common) - common components and header files (containers, mutexes, network protocol headers, etc.) that can be used by both the server and the client. To avoid code duplication and simplify logic of makefiles that build a specific part of the project (server/client/patcher/tests/)), the general logic is placed in this directory.
  - [/common/src/net](/common/src/net) - low-level client-server source code.
  - [/common/utils/containers](/common/utils/containers) - standard containers implementation ([list](/common/src/utils/containers/list), [map](/common/src/utils/containers/map), [FIFO/LIFO priority queue](/common/src/utils/containers/pqueue), [FIFO/LIFO queue](/common/src/utils/containers/queue)).
  - [/common/src/utils/mem](/common/src/utils/mem) - memory allocator.
- [/game](/game) - modified game files. At the moment, these are menu markup files (.men) and menu logic description files (.lgc).
- [/test](/test) - unit tests of critical logic.
- [/AlienShooter.exe.idb](/AlienShooter.exe.idb
) - IDA database with all information found by reverse engineering the game's executable file (AlienShooter.exe).

## Build
### OS Windows
1. Make sure **GCC**, **Makefile**, **git-bash** are installed.

2. Clone the repository
    ```bash
   git clone https://github.com/ep1h/alien-shooter-multiplayer
   ```
3. Navigate to the root directory of the project using git-bash

4. Build asmp.dll
   ```bash
   ./mingw32-make asmp-dll build
   ```
   On successful build, asmp.dll will be located at `asmp-dll/build/asmp.dll` path.

5. Build asmp-server.exe
   ```
   ./mingw32-make asmp-server build
   ```
   On successful build, asmp-server will be located at `asmp-server/build/asmp-server.exe` path.

6. Build asmp-exe-patcher.exe
   ```
   ./mingw32-make asmp-exe-patcher build
   ```
   On successful build, asmp-server will be located at `asmp-exe-patcher/build/asmp-exe-p4tch.exe` path.

   Note: patcher name is written with a '4' instead of 'a', because Windows automatically adds admin rights requirement to run .exe files with "patch" pattern in the name (lol).


## Injection in game process
The client (asmp.dll) can be injected into the game process using any dll injector/asi loader. However, to avoid dependency on third-party software, and also to ensure that hooks to some game functions are installed at the right time, a [patcher](/asmp-exe-patcher) of the game's executable was written.  
The patcher overwrites bytes of the original game executable (AlienShooter.exe) in three places:
1. At offset `0x7E670` from the beginning of the AlienShooter.exe file (these are unused .rdata segment bytes present for section alignment), the name of the .dll to be injected ("asmp.dll") is written.
2. The `0x4FCC` file offset is perfect place to inject the .dll into the game. At the time of execution of the instructions at this address, the game structures have already been initialized, but the game loop has not yet started. At this point, it is necessary to jump to the bytecode described in the previous item.
   | File offset | Original Bytes      | Original Asm    | Pathed Bytes     | Patched Asm   |
   | ----------- | ------------------- | --------------- | ---------------- | ------------- |
   | 0x4FCC      | `89 0D 38 07 49 00` | `mov game, ecx` | `E9 6F 4E 07 00` | `jmp 479E40h` |
   | 0x4FD2      |                     |                 | `90`             | `nop`         |

3. At offset `0x79E40` from the beginning of the AlienShooter.exe file (these are unused .rdata segment bytes present for section alignment), a bytecode is written that pushes onto the stack the address of the .rdata segment containing the name of the .dll to be loaded (`0x7E670 | "asmp.dll"`) and calls WinAPI function __LoadLibraryA__. Also, for safety, at the beginning, the values ​​of all registers are stored on the stack, and after calling LoadLibrary, they are restored from the stack.  
After calling __LoadLibrary__ and restoring registers values ​​from the stack, original bytes from `0x4FCC`, which were overwritten in step 1, shoudl be executed. After that, a jump to the address `0x4FCC` should be performed, that is, a natural continuation of the execution of the game logic.
   | File offset | Bytes           | Asm                                   | Comment                                      |
   | ----------- | --------------- | ------------------------------------- | -------------------------------------------- |
   | 0x79E40     | `60`            | `pushad`                              | Save general-purpose registers on stack      |
   | 0x79E41     | `9C`            | `pushfd`                              | Save EFLAGS on stack                         |
   | 0x79E42     | `68 70E64700`   | `push "asmp.dll"`                     |                                              |
   | 0x79E47     | `FF15 68A04700` | `call dword ptr ds:[<&LoadLibraryA>]` |                                              |
   | 0x79E4D     | `9D`            | `popfd`                               | Restore general-purpose registers from stack |
   | 0x79E4E     | `61`            | `popad`                               | Restore EFLAGS from stack                    |
   | 0x79E4F     | `890D 38074900` | `mov dword ptr ds:[490738],ecx`       | Original 6 bytes from `0x4FCC`               |
   | 0x79E55     | `E9 78B1F8FF`   | `jmp 404FD2h`                         | Jump back to `0x4FCC + 6`                    |

All the logic described in the current chapter is automated and performed by the patcher. Simply run `asmp-exe-p4tch.exe` with path to original AlienShooter.exe as an argument. As a result, the AlienShooter.exe file will be patched, and the original one will be saved side by side and will be called `AlienShooter.exe.bak`.


**Question**: _Why not just create, for example, AlienShooterMultiplayer.exe in the game directory?_  
**Answer:** The game uses the name of the executable file to access keys in the registry containing the game's configuration and license information. Running the game through a renamed .exe will reset the settings and license.

## Server-client architecture
Both server and client are divided into two parts:
### 1. Low-level server
Core server-client logic, implements connection/disconnection and connection maintenance logic (source code is here: [/common/src/net](/common/src/net)). In theory, it can be reused in other projects, since it does not contain any project-specific logic.

Low-level client does not create additional threads for receiving/sending data. Instead, a state machine is used that performs non-blocking processing of the current state when the net_client_tick function is called. This allows elegantly embed send/receive logic into game loop.
### 2. Top-level server-client
Extends the low-level server-client to provide functionality for online play.  
Top-level server implementation: [/asmp-server](/asmp-server)  
Top-level client implementation: [/asmp-dll/src/client](/asmp-dll/src/client)

## Demonstration
### Menu
https://user-images.githubusercontent.com/46194184/227988748-8c160bc6-3c59-44e1-b92a-78d5bff4617c.mp4

### Gameplay
https://user-images.githubusercontent.com/46194184/227989329-74a4ce54-e170-4b3f-9573-6b84e5d6c0d4.MOV
