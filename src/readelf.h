#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#ifndef _FILEDATA_
#define _FILEDATA_
/**/
typedef struct filedata {
    const char *        file_name;
    Elf32_Ehdr          file_header;
    FILE *              file;
    u_int32_t           file_offset;
    u_int32_t           file_size;
    Elf32_Shdr *        section_headers;
    Elf32_Phdr *        program_headers;
    char *              string_table;
} Filedata;

/*
big_endian - only for 32 bits
*/
unsigned big_endian(unsigned char *field, int size);

#endif