#include "readelf.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"

#ifndef _ELF_RELTAB_
#define _ELF_RELTAB_

/*
  Function: get_rel_table
  
    Gets the data relating to relocation tables from the ELF file
    and puts it into filedata.
*/
bool get_rel_table(Filedata *filedata);

/*
    Function: process_rel_table
    
    Prints the data taken with get_rel_table in filedata
    in a similar format to readelf.
*/
bool process_rel_table(Filedata *filedata);

#endif
