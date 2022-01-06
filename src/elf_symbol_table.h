#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "elf_header.h"
#include "readelf.h"
#include <elf.h>
#include "elf_shdrs.h"
#include <ctype.h>

int process_symbol_table(Filedata * filedata);

char * get_st_type(Elf32_Sym *symtable, int i);

char * get_st_bind(Elf32_Sym *symtable, int i);

int get_index_strtab(Filedata * filedata, char * temp, int index_strtab);

int get_index_symtab(Filedata * filedata, char * temp, int index_symtab);

int get_st_value(Elf32_Sym *symtable, int i);

int get_st_size(Elf32_Sym *symtable, int i);

char * get_st_vis(unsigned int other);

char * get_st_name(Elf32_Sym *symtable, int i,char * strtemp);