#include "elf_main.h"
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
Function: Get Elf Class

    Using the class value from the ELF file,
    returns the corresponding capacity (32 or 64)

*/
const char * get_elf_class(unsigned int elf_class);

/*
Function: Get Elf Data

    Using the data value from the ELF file,
    returns the data encoding of the data in the file

*/
const char * get_elf_data(unsigned int elf_data);

/*
Function: Get File Type

    Using the type value from the ELF file,
    returns the corresponding file type

*/
char * get_file_type(unsigned e_type);

/*
Function: Get Machine Name

    Using the machine value from the ELF file,
    returns the corresponding machine name

*/
char * get_machine_name(unsigned e_machine);

/*
Function: Get OS/ABI Name

    Using the OSABI value of file header, 
    returns a corresponding OS/ABI name

*/
char * get_osabi(Filedata *filedata, unsigned int osabi);

/*
Function: Get File Header

    Obtains the values of the file header from the ELF file 
    and stores them into filedata as file_header

*/
bool get_file_header(Filedata *filedata);

/*================================================================
    Process File Header from Obtained Data
  ================================================================*/

/*
Function: Process File Header

    Decodes the data in the file_header and 
    prints the file header in a similar format to readelf

*/
bool process_file_header(Filedata * filedata);

#endif

