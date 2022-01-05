#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#ifndef _ELF_READ
#define _ELF_READ

typedef struct elf_shdr {
  unsigned int	sh_name;		
  unsigned int	sh_type;		
  unsigned int	sh_flags;		
  unsigned int	sh_addr;		
  unsigned int	sh_offset;		
  unsigned int	sh_size;		
  unsigned int	sh_link;		
  unsigned int	sh_info;		
  unsigned int	sh_addralign;		
  unsigned int	sh_entsize;		
} Elf_Shdr;

typedef struct{
  Elf32_Word rel_sh_name;
  Elf32_Rel* rel_tab;
} Elf32_Ext_Rel ;

/**/
typedef struct filedata {
    const char *        file_name;
    Elf32_Ehdr          file_header;
    FILE *              file;
    uint32_t           file_offset;
    uint32_t           file_size;
    Elf_Shdr *          section_headers;
    Elf32_Phdr *        program_headers;
    Elf32_Ext_Rel *      reloc_table;
    char *              string_table;
    int                 string_table_length;
} Filedata;

#endif

/*
big_endian - only for 32 bits
*/
unsigned big_endian(unsigned char *field, int size);