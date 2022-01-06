#include "readelf.h"
#include "elf_shdrs.h"

#ifndef _ELF_SYMBOL_
#define _ELF_SYMBOL_

typedef struct elf_sym {
  unsigned char	st_name[4];		/* Symbol name, index in string tbl */
  unsigned char	st_value[4];		/* Value of the symbol */
  unsigned char	st_size[4];		/* Associated symbol size */
  unsigned char	st_info[1];		/* Type and binding attributes */
  unsigned char	st_other[1];		/* No defined meaning, 0 */
  unsigned char	st_shndx[2];		/* Associated section index */
} Elf_Sym;

Elf32_Sym * get_symbols(Filedata *filedata, Elf32_Shdr *section, int *num);

char * get_symbol_type(Filedata *filedata, int type);

char * get_symbol_bind(Filedata *filedata, int bind);

char *get_symbol_visibility (int visibility);

char * get_symbol_index_type (Filedata * filedata, unsigned int type);

void print_symbol(Filedata * filedata, int num, Elf32_Sym * symtab, Elf32_Shdr * section, char *strtab, int strtab_size);

bool process_symbol_table(Filedata * filedata);

#endif