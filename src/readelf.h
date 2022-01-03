#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef _ELF_READ
#define _ELF_READ

/*================================================================
    File Header Definitions
  ================================================================*/
#define EI_NIDENT   16

// Magic bytes of ELF file
#define ELFMAG0     0x7f
#define ELFMAG1     'E'
#define ELFMAG2     'L'
#define ELFMAG3     'F'

// Magic bytes - value obtained from file
#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4
#define EI_DATA         5
#define EI_VERSION      6
#define EI_ABIVERSION   8
#define EI_PAD          9

// Class Identifier
#define ELFCLASSNONE    0
#define ELFCLASS32      1
#define ELFCLASS64      2

// Data 
#define ELFDATANONE     0
#define ELFDATA2LSB     1
#define ELFDATA2MSB     2

// Version
#define EV_NONE         0
#define EV_CURRENT      1

// Object File Type
#define ET_NONE         0
#define ET_REL          1
#define ET_EXEC         2
#define ET_DYN          3
#define ET_CORE         4
#define ET_LOPROC       0xff00
#define ET_HIPROC       0xffff

// Machine
#define EM_M32          1
#define EM_SPARC        2
#define EM_386          3 
#define EM_68K          4 
#define EM_88K          5 
#define EM_860          7 
#define EM_MIPS         8  
#define EM_MIPS_RS4_BE  10 


typedef struct elf32_ehdr{
    unsigned char       e_ident[EI_NIDENT];
    u_int16_t           e_type;                     // Elf32_Half
    u_int16_t           e_machine;                  // Elf32_Half
    u_int32_t           e_version;                  // Elf32_Word
    u_int32_t           e_entry;                    // Elf32_Addr
    u_int32_t           e_phoff;                    // Elf32_Off
    u_int32_t           e_shoff;                    // Elf32_Off
    u_int32_t           e_flags;                    // Elf32_Word
    u_int16_t           e_ehsize;                   // Elf32_Half
    u_int16_t           e_phentsize;                // Elf32_Half
    u_int16_t           e_phnum;                    // Elf32_Half
    u_int16_t           e_shentsize;                // Elf32_Half
    u_int16_t           e_shnum;                    // Elf32_Half
    u_int16_t           e_shstrndx;                 // Elf32_Half
} Elf32_Ehdr;

/* In order to use the same Big Endian Function throughout the project,
All of the entries must be an unsigned char and transformed depending on 
the size - think of it as a temporary holder*/
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

/*================================================================
    Section Header Definitions
  ================================================================*/

// Special Section Indexes
#define SHN_UNDEF       0
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC      0xff00
#define SHN_HIPROC      0xff1f
#define SHN_ABS         0xfff1
#define SHN_COMMON      0xfff2
#define SHN_HIRESERVE   0xffff

// Section Types
#define SHT_NULL        0
#define SHT_PROGBITS    1
#define SHT_SYMTAB      2
#define SHT_STRTAB      3
#define SHT_RELA        4
#define SHT_HASH        5
#define SHT_DYNAMIC     6
#define SHT_NOTE        7
#define SHT_NOBITS      8
#define SHT_REL         9
#define SHT_SHLIB       10
#define SHT_DYNSYM      11
#define SHT_LOPROC      0x70000000
#define SHT_HIPROC      0x7fffffff
#define SHT_LOUSER      0x80000000
#define SHT_HIUSER      0xffffffff

typedef struct elf32_shdr{
    u_int32_t           sh_name;
    u_int32_t           sh_type;
    u_int32_t           sh_flags;
    u_int32_t           sh_addr;
    u_int32_t           sh_offset;
    u_int32_t           sh_size;
    u_int32_t           sh_link;
    u_int32_t           sh_info;
    u_int32_t           sh_addralign;
    u_int32_t           sh_entsize;
} Elf32_Shdr;

typedef struct elf32_shdrs{
    unsigned char       sh_name[4];
    unsigned char       sh_type[4];
    unsigned char       sh_flags[4];
    unsigned char       sh_addr[4];
    unsigned char       sh_offset[4];
    unsigned char       sh_size[4];
    unsigned char       sh_link[4];
    unsigned char       sh_info[4];
    unsigned char       sh_addralign[4];
    unsigned char       sh_entsize[4];
} Elf32_Shdrs;

typedef struct elf32_int_shdrs{
    unsigned int       sh_name;
    unsigned int       sh_type;
    unsigned long      sh_flags;
    unsigned long      sh_addr;
    unsigned long      sh_offset;
    unsigned long      sh_size;
    unsigned long      sh_link;
    unsigned long      sh_info;
    unsigned long      sh_addralign;
    unsigned int       sh_entsize;
} Elf32_Int_Ehdr;

/**/
typedef struct {
    u_int32_t           p_type;
    u_int32_t           p_offset;
    u_int32_t           p_vaddr;
    u_int32_t           p_paddr;
    u_int32_t           p_filesize;
    u_int32_t           p_memesize;
    u_int32_t           p_flags;
    u_int32_t           p_align;
} Elf32_Phdr;

/**/
typedef struct filedata {
    const char *        file_name;
    Elf32_Ehdr          file_header;
    FILE *              file;
    u_int32_t           file_offset;
    u_int32_t           file_size;
    Elf32_Shdr *        section_headers;
    Elf32_Phdr *        program_headers;
    char *              string_table;
} Filedata;

#endif

/*
big_endian - only for 32 bits
*/
unsigned big_endian(unsigned char *field, int size);