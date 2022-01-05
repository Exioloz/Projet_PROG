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

void read_symbol_table(Filedata * filedata);