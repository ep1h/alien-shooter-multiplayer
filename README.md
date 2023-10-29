# alien-shooter-multiplayer
Alien Shooter is a single-player isometric top-down shooter released in 2003. This repository contains the results of reverse engineering and development in C, which enables network play for the game.

## Navigation
- [Repository navigation](#Repository-navigation)
- [Prerequisites](#Prerequisites)
- [Build](#Build)
- [Injection in game process](#Injection-in-game-process)
- [Server-client architecture](#Server-client-architecture)
- [Demonstration](#Demonstration)

## Repository navigation
- [/asmp-dll](/asmp-dll) - Client source code. The client is a .dll library; injecting it into the game process extends the game logic to enable network play.
  - [/asmp-dll/src/game](/asmp-dll/src/game) - API for interacting with the game. Contains data types, function addresses, and global objects obtained through reverse engineering.
    - [/asmp-dll/src/game/types](/asmp-dll/src/game/types) - Game types.
    - [/asmp-dll/src/game/addresses.h](/asmp-dll/src/game/addresses.h) - List of functions and data addresses.
  - [/asmp-dll/src/multiplayer](/asmp-dll/src/multiplayer) - Top-level client implementation and multiplayer logic.
    - [/asmp-dll/src/multiplayer/client](/asmp-dll/src/multiplayer/client) - Top-level client implementation.
  - [/asmp-dll/src/dllmain.c](/asmp-dll/src/dllmain.c) - Entry point. Everything starts here.
  - [/asmp-dll/src/utils/hook](/asmp-dll/src/utils/hook) - Component for function hooking.
- [/asmp-server](/asmp-server) - Server source code. The server is a 32-bit Windows executable file.
- [/asmp-exe-patcher](/asmp-exe-patcher) - Patcher source code. The patcher modifies the original AlienShooter.exe so that it automatically injects the network play client (asmp.dll) when launched. See the ["Injection in game process"](#Injection-in-game-process) section for details.
- [/common](/common) - Common components and header files (containers, mutexes, network protocol headers, etc.) used by both the server and the client. To avoid code duplication and simplify makefiles logic for building specific project parts (server / client / patcher / tests), general logic is placed in this directory.
  - [/common/src/net](/common/src/net) - Low-level client-server source code.
  - [/common/src/utils/containers](/common/src/utils/containers) - Standard container implementations ([list](/common/src/utils/containers/list), [map](/common/src/utils/containers/map), [FIFO/LIFO priority queue](/common/src/utils/containers/pqueue), [FIFO/LIFO queue](/common/src/utils/containers/queue)).
  - [/common/src/utils/mem](/common/src/utils/mem) - Memory allocator.
- [/game](/game) - Modified game files. Currently, these include menu markup files (.men) and menu logic description files (.lgc).
- [/test](/test) - Unit tests for critical logic.
- [/AlienShooter.exe.idb](/AlienShooter.exe.idb) - IDA database with all information found by reverse engineering the game's executable file (AlienShooter.exe).

## Prerequisites
### OS Windows
* Ensure **GCC**, **Make** are installed.
* Depending on your installation, the `make` command might be called `mingw32-make`. If `make` doesn't work, try using `./mingw32-make` instead.

### OS Linux
Install Make:
```bash
apt-get install make
```
Install Mingw:

For 32-bit system:
```bash
apt-get install mingw32
```
For 64-bit system:
```bash
apt-get install mingw-w64
```

## Build
1. Clone the repository:
```bash
git clone https://github.com/ep1h/alien-shooter-multiplayer
```

2. Navigate to the root directory of the project.
```bash
cd alien-shooter-multiplayer
```

3. Build asmp.dll
```bash
./mingw32-make asmp-dll build
```
On successful build, asmp.dll will be located at `asmp-dll/build/asmp.dll` path.

4. Build asmp-server.exe
```bash
./mingw32-make asmp-server build
```
On successful build, asmp-server will be located at `asmp-server/build/asmp-server.exe` path.

5. Build asmp-exe-patcher.exe
```bash
./mingw32-make asmp-exe-patcher build
```
On successful build, asmp-exe-patcher will be located at `asmp-exe-patcher/build/asmp-exe-p4tch.exe` path.

Note: The patcher name uses a '4' instead of 'a', because Windows automatically adds admin rights requirements to run .exe files with "patch" pattern in the name.


## Documentation
To generate the project documentation, use the following command:
```bash
./mingw32-make documentation
```
Once generated, the documentation will be available in the [/docs](/docs) directory of the project.

## Injection in game process
The client (asmp.dll) can be injected into the game process using any dll injector/asi loader. However, to avoid dependency on third-party software, and ensure that hooks to some game functions are installed at the right time, a [patcher](/asmp-exe-patcher) of the game's executable was written.  
The patcher overwrites bytes of the original game executable (AlienShooter.exe) in three places:

1. At offset `0x7E670` from the beginning of the AlienShooter.exe file (these are unused .rdata segment bytes present for section alignment), the name of the .dll to be injected ("asmp.dll") is written.

2. The `0x4FCC` file offset is the perfect place to inject the .dll into the game. At the time of execution of the instructions at this address, the game structures have already been initialized, but the game loop has not yet started. At this point, it is necessary to jump to the bytecode which will load the asmp.dll in into the game process (this bytecode will be described in the next item).
   | File offset | Original Bytes      | Original Asm    | Pathed Bytes     | Patched Asm   |
   | ----------- | ------------------- | --------------- | ---------------- | ------------- |
   | 0x4FCC      | `89 0D 38 07 49 00` | `mov game, ecx` | `E9 6F 4E 07 00` | `jmp 479E40h` |
   | 0x4FD2      |                     |                 | `90`             | `nop`         |

3. At offset `0x79E40` from the beginning of the AlienShooter.exe file (these are unused .rdata segment bytes present for section alignment), a bytecode is written that pushes onto the stack the address of the .rdata segment containing the name of the .dll to be loaded (`0x7E670 | "asmp.dll"`) and calls WinAPI function __LoadLibraryA__. Also, for safety, at the beginning, the values ​​of all registers are stored on the stack, and after calling __LoadLibraryA__, they are restored from the stack.  
After calling __LoadLibrary__ and restoring registers values ​​from the stack, original bytes from `0x4FCC`, which were overwritten in step 2, should be executed. After that, a jump to the address `0x4FD2` should be performed, that is, a natural continuation of the execution of the game logic.
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
The server-client architecture is divided into two parts:
### 1. Low-level server-client
The core server-client logic handles connection, disconnection, and connection maintenance (source code can be found at: [/common/src/net](/common/src/net)). This component is designed to be reusable in other projects, as it does not contain any project-specific logic.

The low-level client does not create additional threads for receiving or sending data. Instead, it employs a state machine for non-blocking processing of the current state when the ```net_client_tick``` function is called. This approach enables complete integration of the send/receive logic into the game loop.
### 2. Top-level server-client
Extends the low-level server-client to provide functionality for online play.  
Top-level server implementation: [/asmp-server](/asmp-server)  
Top-level client implementation: [/asmp-dll/src/multiplayer/client](/asmp-dll/src/multiplayer/client)

## Demonstration
### Menu
https://user-images.githubusercontent.com/46194184/227988748-8c160bc6-3c59-44e1-b92a-78d5bff4617c.mp4

### Gameplay
https://user-images.githubusercontent.com/46194184/230725748-09ea9940-c1c2-4e04-9b91-7db1ca6da255.mp4
