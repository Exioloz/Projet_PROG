#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "elf_header.h"
#include "elf_main.h"
#include <elf.h>
#include "elf_shdrs.h"
#include <ctype.h>

int read_section(Filedata * filedata, char * section_name);