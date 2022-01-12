#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "elf_header.h"
#include "elf_main.h"
#include "elf_reltab.h"
#include <elf.h>
#include "elf_shdrs.h"
#include <ctype.h>

void implantation( Filedata * filedata, Filedata * newfile);

void calcul_val(Elf32_Ext_Rel *  ext_tab, Elf32_Addr * addr, Elf32_Off offset_sec, Filedata * filedata, int index, Filedata * newdata);

void replace_data(Filedata * fp, uint32_t val_symbol, Elf32_Addr * addr_p, Filedata * newdata);