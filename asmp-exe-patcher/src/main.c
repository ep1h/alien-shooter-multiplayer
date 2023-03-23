#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool patch_bytes_(FILE* file, long offset, const void* buf,
                         size_t buf_size)
{
    if (fseek(file, offset, SEEK_SET) == 0)
    {
        if (fwrite(buf, buf_size, 1, file) == 1)
        {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    /* Check arguments */
    if (argc != 2)
    {
        printf("Usage: %s path_to_game_executable.\n", argv[0]);
        return 111;
    }

    /* Open original file */
    FILE* file = fopen(argv[1], "rb");
    if (!file)
    {
        printf("Error: file %s does not exists.\n", argv[1]);
        return -2;
    }

    /* Validate original file */
    /* Define a sequence of bytes to perform file validation */
    const unsigned char check_bytes[] = {0x89, 0x0D, 0x38, 0x07, 0x49, 0x00};
    /* Compare bytes of the file with the sequence */
    if (fseek(file, 0x4FCC, SEEK_SET) != 0)
    {
        printf("Error: unable to validate the file %s.\n", argv[1]);
        return -3;
    }
    /* Validate */
    for (unsigned int i = 0; i < sizeof(check_bytes); i++)
    {
        unsigned char c;
        if (fread(&c, 1, 1, file) != 1)
        {
            printf("Error: unable to read %ld byte from file %s to perform "
                   "validation.\n",
                   ftell(file), argv[1]);
            fclose(file);
            return -4;
        }
        if (c != check_bytes[i])
        {
            printf(
                "Error: validation failed. Byte at offset 0x%lx in file %s is "
                "different than expected.\n",
                ftell(file), argv[1]);
            return -5;
        }
    }
    rewind(file); /* Reset file ptr */

    /* Build backup file name */
    size_t backup_file_name_size = strlen(argv[1]) + 4 + 1;
    char* backup_file_name = malloc(backup_file_name_size);
    if (!backup_file_name)
    {
        printf("Error: unable to allocate memory.\n");
        fclose(file);
        return -6;
    }
    if (sprintf(backup_file_name, "%s.bak", argv[1]) !=
        (int)backup_file_name_size - 1)
    {
        fclose(file);
        return -7;
    }

    /* Check if the backup file already exists */
    FILE* backup_file = fopen(backup_file_name, "r");
    if (backup_file)
    {
        printf("Error: backup file (%s) already exists.\n", backup_file_name);
        fclose(backup_file);
        fclose(file);
        free(backup_file_name);
        return -8;
    }
    backup_file = 0;

    /* Create backup file */
    backup_file = fopen(backup_file_name, "wb");
    if (!backup_file)
    {
        printf("Error: unable to create backup file.\n");
        fclose(file);
        free(backup_file_name);
        return -9;
    }

    /* Copy original file bytes to backup */
    char buf[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buf, 1, 1024, file)) > 0)
    {
        fwrite(buf, 1, bytes_read, backup_file);
    }
    if (ftell(file) != ftell(backup_file) || ftell(file) == 0)
    {
        printf("Error: unable to copy original file bytes to backup.\n");
        free(backup_file_name);
        fclose(backup_file);
        fclose(file);
        return -10;
    }
    fclose(backup_file);
    fclose(file);

    /* Backup is created. Open original file for patching */
    file = fopen(argv[1], "r+b");
    if (!file)
    {
        printf("Error: unable to open %s for writing.\n", argv[1]);
        free(backup_file_name);
        return -11;
    }

    /* Patch original file */
    /* .rdata:0047E670 61 73 6D 70 2E 64 6C 6C 00   | "asmp.dll" */
    const unsigned char rdata_dll_name[] = {
        0x61, 0x73, 0x6D, 0x70, 0x2E, 0x64, 0x6C, 0x6C, 0x00}; /* asmp.dll */

    /* 00404FCC | E9 6F4E0700              | jmp alienshooter.479E40 */
    const unsigned char injector_caller_bytes[] = {0xE9, 0x6F, 0x4E, 0x07,
                                                   0x00};

    /*
    00479E40 | 60               | pushad
    00479E41 | 9C               | pushfd
    00479E42 | 68 70E64700      | push alienshooter.47E670  | 47E670:"asmp.dll"
    00479E47 | FF15 68A04700    | call dword ptr ds:[<&LoadLibraryA>]
    00479E4D | 9D               | popfd
    00479E4E | 61               | popad
    00479E4F | 890D 38074900    | mov dword ptr ds:[490738],ecx
    00479E55 | E9 78B1F8FF      | jmp alienshooter.404FD2
    */
    const unsigned char injection_logic_bytes[] = {
        0x60,                               /* pushad */
        0x9C,                               /* pushf */
        0x68, 0x70, 0xE6, 0x47, 0x00,       /* push 47E670 | rdata "asmp.dll" */
        0xFF, 0x15, 0x68, 0xA0, 0x47, 0x00, /* call dword ptr LoadLibraryA */
        0x9D,                               /* popfd */
        0x61,                               /* popad */
        0x89, 0x0D, 0x38, 0x07, 0x49, 0x00, /* original bytes from 404FCC */
        0xE9, 0x78, 0xB1, 0xF8, 0xFF        /* jump back to 0x404FCC + 6 */
    };

    if (!patch_bytes_(file, 0x7E670, rdata_dll_name, sizeof(rdata_dll_name)))
    {
        printf("Error: unable to write patch bytes in file.\n");
        fclose(file);
        free(backup_file_name);
        return -12;
    }
    if (!patch_bytes_(file, 0x4FCC, injector_caller_bytes,
                      sizeof(injector_caller_bytes)))
    {
        printf("Error: unable to write patch bytes in file.\n");
        fclose(file);
        free(backup_file_name);
        return -13;
    }
    if (!patch_bytes_(file, 0x79E40, injection_logic_bytes,
                      sizeof(injection_logic_bytes)))
    {
        printf("Error: unable to write patch bytes in file.\n");
        fclose(file);
        free(backup_file_name);
        return -14;
    }
    printf("File %s patched successfully. The original file is backed up as: "
           "%s.",
           argv[1], backup_file_name);
    fclose(file);
    free(backup_file_name);
    return 0;
}
