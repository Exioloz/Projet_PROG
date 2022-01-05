#include "readelf.h"
#include <elf.h>

#ifndef _FILE_HEADER_
#define _FILE_HEADER_

#define EM_MIPS_RS4_BE  10

/*================================================================
    Get Values of File Section Headers from File
  ================================================================*/

// Used to obtain File Header from file
typedef struct elf32_header{
    unsigned char       e_ident[EI_NIDENT];
    unsigned char       e_type[2];                    
    unsigned char       e_machine[2];                 
    unsigned char       e_version[4];         
    unsigned char       e_entry[4];          
    unsigned char       e_phoff[4];          
    unsigned char       e_shoff[4];        
    unsigned char       e_flags[4];          
    unsigned char       e_ehsize[2];                 
    unsigned char       e_phentsize[2];   
    unsigned char       e_phnum[2];         
    unsigned char       e_shentsize[2];           
    unsigned char       e_shnum[2];              
    unsigned char       e_shstrndx[2];             
} Elf32_Head;

/*
Get Elf Class
*/
const char * get_elf_class(unsigned int elf_class);

/*
Get Elf Data
*/
const char * get_elf_data(unsigned int elf_data);

/*
Get File Type
*/
char * get_file_type(unsigned e_type);

/*
Get Machine Name
*/
char * get_machine_name(unsigned e_machine);

/*
Get OS/ABI Name
*/
char * get_osabi(Filedata *filedata, unsigned int osabi);

/*
Get File Header

*/
bool get_file_header(Filedata *filedata);

/*================================================================
    Process File Header from Obtained Data
  ================================================================*/

/*
Process File Header
Input : filedata
Output : returns a bool to determine whether file header was processed properly
Decodes the data in the file header and prints as readelf
*/
bool process_file_header(Filedata * filedata);

#endif

