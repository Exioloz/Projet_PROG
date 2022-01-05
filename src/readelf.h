#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#ifndef _ELF_READ
#define _ELF_READ


typedef struct{
  Elf32_Word rel_sh_name;
  Elf32_Rel* rel_tab;
} Elf32_Ext_Rel ;

/**/
/*
Data Structure for ELF files which contains the data 
of each part of an object file
*/
typedef struct filedata {
    const char *        file_name;
    Elf32_Ehdr          file_header;
    FILE *              file;
    uint32_t           file_offset;
    uint32_t           file_size;
    Elf32_Shdr *          section_headers;
    Elf32_Phdr *        program_headers;
    Elf32_Ext_Rel *      reloc_table;
    char *              string_table;
    int                 string_table_length;
} Filedata;

#endif

/*
Free Filedata (to not have repetitive code)
*/
void free_filedata(Filedata * filedata);

/*
big_endian - only for 32 bits
*/
unsigned big_endian(unsigned char *field, int size);