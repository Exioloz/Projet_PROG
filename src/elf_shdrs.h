#include "readelf.h"

#ifndef _ELF_SHDRS_
#define _ELF_SHDRS_

/*================================================================
    Get Values of File Section Headers from File
  ================================================================*/

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

void * get_section(void *var, Filedata *filedata, unsigned long offset, unsigned long size, unsigned long num);

bool get_section_headers(Filedata *filedata);

/*================================================================
    Process File Section Headers from Obtained Data
  ================================================================*/

char * get_section_type(Filedata * filedata, int sh_type);

char * get_section_name(Filedata *filedata, Elf32_Word sh_name);

void section_name(int width, char * symbol);

char * get_section_type(Filedata * filedata, int sh_type);

char * get_section_flags(Filedata * filedata, unsigned int flags);

bool process_section_headers (Filedata * filedata);

#endif