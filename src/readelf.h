#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#ifndef _ELF_READ
#define _ELF_READ


typedef struct{
  Elf32_Word rel_sh_name;
  Elf32_Off rel_sh_offset;
  Elf32_Half rel_ent_num;
  Elf32_Rel* rel_ents;
} Elf32_Ext_Rel ;

typedef struct{
  Elf32_Half rel_num;
  Elf32_Ext_Rel* rel_tab;
} Elf32_Rel_Tab;

typedef struct{
  Elf32_Half sym_num;
  Elf32_Off sym_sh_offset;
  Elf32_Word sym_sh_name;
  Elf32_Sym* sym_tab;
} Elf32_Ext_Sym;

typedef struct{
  Elf32_Half sym_tab_num;
  Elf32_Ext_Sym* sym_ext_tabs;
} Elf32_Sym_Tab;

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
    Elf32_Rel_Tab       reloc_table;
    Elf32_Sym_Tab       symbol_table;
    char *              string_table;
    int                 string_table_length;
} Filedata;

#endif

/*================================================================
    Functions to obtain good data
  ================================================================*/

uint16_t change_endian_16(uint16_t num);

uint32_t change_endian_32(uint32_t num);

unsigned big_endian(unsigned char *field, int size);


/*================================================================
    Functions to process file depending on options
  ================================================================*/

bool process_header(Filedata * filedata);

bool process_sections_header(Filedata * filedata);

bool process_section_name(Filedata * filedata);

bool process_symtab(Filedata * filedata);

bool process_file(char *file_name);

/*================================================================
    Functions for details of file
  ================================================================*/

long int size_of_file(FILE * filename);

void free_filedata(Filedata * filedata);