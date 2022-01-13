#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

#ifndef _ELF_MAIN_
#define _ELF_MAIN_


typedef struct{
  Elf32_Word  rel_sh_name;    //name of the relocation table section
  Elf32_Off   rel_sh_offset;  //offset of the section
  Elf32_Half  rel_ent_num;    //number of Elf32_Rel entries
  Elf32_Rel*  rel_ents;       //table of entries
} Elf32_Ext_Rel ;

typedef struct{
  Elf32_Half      rel_num; //number of relocation table sections
  Elf32_Ext_Rel*  rel_tab; //table of relocation tables pertaining to those sections
} Elf32_Rel_Tab;

typedef struct{
  Elf32_Half sym_tab_num; //number of symbol table entries
  Elf32_Sym* sym_entries; //actual symbol table
} Elf32_Sym_Tab;

/*
Data Structure for ELF files which contains the data 
of each part of an object file
*/
typedef struct filedata {
    const char *        file_name;            // file name
    Elf32_Ehdr          file_header;          // file header
    FILE *              file;                 // pointer to file
    uint32_t            file_offset;          // offset of file
    uint32_t            file_size;            // size of file
    Elf32_Shdr *        section_headers;      // section header table
    Elf32_Rel_Tab       reloc_table;          // relocation table
    Elf32_Sym_Tab       symbol_table;         // symbol table
    char *              string_table;         // string table
    int                 string_table_length;  // string table length
} Filedata;

/*================================================================
    Functions to obtain good data
  ================================================================*/

/*
change_endian_16:
  changes a 16-bit integer from little-endian to big-endian (or vice-versa)
*/
uint16_t change_endian_16(uint16_t num);

/*
change_endian_32:
  changes a 32-bit integer from little-endian to big-endian (or vice-versa)
*/
uint32_t change_endian_32(uint32_t num);

/*
Function: big_endian 
    Takes strings in 32 bits and performs the big endian operation 
*/
unsigned big_endian(unsigned char *field, int size);


/*================================================================
    Functions to process file depending on options
  ================================================================*/

/*
Function: Process File Header
    Should print the same as option -h of readelf
*/
bool process_header(Filedata * filedata);

/*
Function: Process Sections' Header
    Should print the same as option -S of readelf
*/
bool process_sections_header(Filedata * filedata);

/*
Function: Process Section Name
    Should print the same as the option -x ".name of section" of readelf
*/
bool process_section_name(Filedata * filedata);

/*
Function: Process Symbol Table
    Should print the same as the option -s of readelf
*/
bool process_symtab(Filedata * filedata);

/*
Function: Process Relocation
    Should print the same as the option -r of readelf
*/
bool process_reloc(Filedata * filedata);

/*
Function: Get filedata 
    Obtains and stores the values of the ELF file into
    the corresponding structure in filedata
*/
bool get_filedata(char *file_name, Filedata *filedata);

/*
Function: Process File 
    Prints the corresponding options
*/
bool process_file(Filedata *filedata);

/*================================================================
    Functions for details of file
  ================================================================*/

// Find Size of File
long int size_of_file(FILE * filename);

/* 
Function: Free Filedata
    Used to free the memory allocated for filedata
    in order to prevent memory leak
*/
void free_filedata(Filedata * filedata);

/*
Function: help
    Displays the options of ./readelf function in case 
    of misuse
*/
void help();

/* 
Function: elf_main
    Called by readelf main to process the given ELF file
    depending on the option(s) given by the user.
    If no option is given, help is written in terminal. 
*/
int elf_main(int argc, char ** argv);

#endif