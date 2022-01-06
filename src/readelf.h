#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#ifndef _ELF_READ
#define _ELF_READ


typedef struct{
  Elf32_Word rel_sh_name; //name of the relocation table section
  Elf32_Off rel_sh_offset; //offset of the section
  Elf32_Half rel_ent_num; //number of Elf32_Rel entries
  Elf32_Rel* rel_ents; //table of entries
} Elf32_Ext_Rel ;

typedef struct{
  Elf32_Half rel_num; //number of relocation table sections
  Elf32_Ext_Rel* rel_tab; //table of relocation tables pertaining to those sections
} Elf32_Rel_Tab;

typedef struct{
  Elf32_Half sym_tab_num;
  Elf32_Sym* sym_entries;
} Elf32_Sym_Tab;
/**/
/*
Data Structure for ELF files which contains the data 
of each part of an object file
*/
typedef struct filedata {
    const char *        file_name;
    Elf32_Ehdr          file_header;
    FILE *              file;
    uint32_t            file_offset;
    uint32_t            file_size;
    Elf32_Shdr *        section_headers;
    Elf32_Phdr *        program_headers;
    Elf32_Rel_Tab       reloc_table;
    Elf32_Sym_Tab       symbol_table;
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

/*
change_endian_16:
  changes a 16-bit integer from little-endian to big-endian (or vice-versa)
*/
uint16_t change_endian_16(uint16_t num);

/*
change_endian_32:
  changes a 32-bit integer from little-endian to big-endian (or vice-versa)
*/
uint32_t change_endian_32(uint32_t num);