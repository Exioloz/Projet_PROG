#include "elf_main.h"
#include "elf_header.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"
#include "elf_reltab.h"
#include "read_section.h"

/*================================================================
    MAIN FUNCTION
  ================================================================*/

// To be changed if we want to read multiple files at once
int main(int argc, char ** argv){
    return elf_main(argc, argv);
}