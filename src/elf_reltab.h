#include "readelf.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"

#ifndef _ELF_RELTAB_
#define _ELF_RELTAB_

bool get_rel_table(Filedata *filedata);

bool process_rel_table(Filedata *filedata);

#endif
