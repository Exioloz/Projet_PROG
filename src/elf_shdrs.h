#include "elf_main.h"

#ifndef _ELF_SHDRS_
#define _ELF_SHDRS_

// Used to obtain Sections' Header from file
typedef struct {
  unsigned char	sh_name[4];		
  unsigned char	sh_type[4];		
  unsigned char	sh_flags[4];		
  unsigned char	sh_addr[4];		
  unsigned char	sh_offset[4];	
  unsigned char	sh_size[4];	
  unsigned char	sh_link[4];		
  unsigned char	sh_info[4];	
  unsigned char	sh_addralign[4];	
  unsigned char	sh_entsize[4];	
} Elf32_External_Shdr;

/*================================================================
    Get Values of File Section Headers from File
  ================================================================*/

/*
Function: Get Section

    Using the offset, size and number,
    returns the data obtained from ELF file 
    starting from the offset position
        Not just used for section header table
    
*/
void * get_section(void *var, Filedata *filedata, unsigned long offset, unsigned long size, unsigned long num);

/*
Get Section Headers
    
    Obtains the section header table data from 
    an ELF file and stores it into section_headers
    
*/
bool get_section_headers(Filedata *filedata);

/*
Function: Get Section Type

    Using the type value of the section header table,
    returns the corresponding section type

*/
char * get_section_type(Filedata * filedata, int sh_type);

/*
Function: Get Name of Section Header

    Using the index of the section name,
    returns the name of the section from string table

*/
char * get_section_name(Filedata *filedata, Elf32_Word sh_name);

/*
Function: Formatted Section Name

    Using the width and string, 
    prints the string with the proper spacing for output

*/
void section_name(int width, char * symbol);

/*
Function: Get Section Flags

    Using the flags value of section header,
    returns the corresponding flag characters

*/
char * get_section_flags(Filedata * filedata, unsigned int flags);

/*================================================================
    Process File Section Headers from Obtained Data
  ================================================================*/

/*
Function: Process Section Headers
    
    Decodes the data in the section header table and
    prints the section header table in a similar format to readelf

*/
bool process_section_headers (Filedata * filedata);

#endif