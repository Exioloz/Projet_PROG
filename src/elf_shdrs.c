#include "elf_shdrs.h"
#include <ctype.h>


/*================================================================
    Get Values of File Section Headers from File
  ================================================================*/

/*
Get Section
    Reads string table and returns the value of string table from file
*/
void * get_section(void *var, Filedata *filedata, unsigned long offset, unsigned long size, unsigned long num){
    if (size == 0 || num == 0)
        return NULL;
    // Terminate invalid string table sections with '\0'
    unsigned long total_size = size * num;
    void * temp_var = var;
    if (temp_var == NULL){
        temp_var = malloc((size_t)total_size + 1);
        if (temp_var == NULL){
            fprintf(stderr, "Memory allocation for string table sections failed\n");
            return NULL;
        }
        ((char *)temp_var)[total_size] = '\0';
    }

    if (fseek (filedata->file, offset, SEEK_SET))
        return NULL;

    if (fread(temp_var, size, num, filedata->file)!= num){
        fprintf(stderr, "Unable to read Section Headers\n");
        if (temp_var != var)
            free(temp_var);
        return NULL;    
    }
    return temp_var;
}

/*
Get Section Headers
    Obtains data for section headers from file by storing string table into shdrs
*/
bool get_section_headers(Filedata *filedata){
    int size = filedata->file_header.e_shentsize;
    int num = filedata->file_header.e_shnum;
    if (size == 0 || num == 0)
        return false;
    Elf32_External_Shdr * shdrs = (Elf32_External_Shdr *) get_section(NULL, filedata, filedata->file_header.e_shoff, size, num);
    if (shdrs == NULL)
        return false;
    filedata->section_headers = (Elf32_Shdr *) malloc(sizeof(Elf32_Shdr)*num);
    if (filedata->section_headers == NULL){
        fprintf(stderr, "Out of memory reading section headers\n");
        free (shdrs);
        return false;
    }

    Elf32_Shdr * internal;
    int i;
    for (i = 0, internal = filedata->section_headers; i < num; i++, internal++){
        internal->sh_name      = big_endian(shdrs[i].sh_name, sizeof(shdrs[i].sh_name));
        internal->sh_type      = big_endian(shdrs[i].sh_type, sizeof(shdrs[i].sh_type));
        internal->sh_flags     = big_endian(shdrs[i].sh_flags, sizeof(shdrs[i].sh_flags));
        internal->sh_addr      = big_endian(shdrs[i].sh_addr, sizeof(shdrs[i].sh_addr));
        internal->sh_offset    = big_endian(shdrs[i].sh_offset, sizeof(shdrs[i].sh_offset));
        internal->sh_size      = big_endian(shdrs[i].sh_size, sizeof(shdrs[i].sh_size));
        internal->sh_link      = big_endian(shdrs[i].sh_link, sizeof(shdrs[i].sh_link));
        internal->sh_info      = big_endian(shdrs[i].sh_info, sizeof(shdrs[i].sh_info));
        internal->sh_addralign = big_endian(shdrs[i].sh_addralign, sizeof(shdrs[i].sh_addralign));
        internal->sh_entsize   = big_endian(shdrs[i].sh_entsize, sizeof(shdrs[i].sh_entsize));
    }
    // Verify that obtained values are valid
    if (filedata->section_headers->sh_link > num)
	    fprintf(stderr,"Section %u has an out of range sh_link value of %u\n", i, filedata->section_headers->sh_link);
    if (filedata->section_headers->sh_flags & SHF_INFO_LINK && filedata->section_headers->sh_info > num)
	    fprintf(stderr,"Section %u has an out of range sh_info value of %u\n", i, filedata->section_headers->sh_info);
    free(shdrs);
    return true;
}

/*================================================================
    Process File Section Headers from Obtained Data
  ================================================================*/
/*
Get Name of Section Type
*/
char * get_section_type(Filedata * filedata, int sh_type){
    switch(sh_type){
        case SHT_NULL:		        return "NULL";
        case SHT_PROGBITS:          return "PROGBITS";
        case SHT_SYMTAB:	        return "SYMTAB";
        case SHT_STRTAB:	        return "STRTAB";
        case SHT_RELA:		        return "RELA";
        case SHT_REL:		        return "REL";
        case SHT_HASH:		        return "HASH";
        case SHT_DYNAMIC:	        return "DYNAMIC";
        case SHT_NOTE:		        return "NOTE";
        case SHT_NOBITS:	        return "NOBITS";
        case SHT_SHLIB:		        return "SHLIB";
        case SHT_DYNSYM:	        return "DYNSYM";
        case SHT_LOPROC:            return "LOPROC"; 
        case SHT_HIPROC:            return "HIPROC";
        case SHT_LOUSER:            return "LOUSER";
        case SHT_HIUSER:            return "HIUSER";
        case SHT_ARM_ATTRIBUTES:    return "ARM_ATTRIBUTES";
        default:                    return "<unknown>";
    }
    return "<error>";
}

/*
Get Name of Section Header
*/
char * get_section_name(Filedata *filedata, Elf32_Word sh_name){
  if (filedata->string_table == NULL)
    return ("<no-strings>");
  if (sh_name >= filedata->string_table_length)
    return ("<corrupt>");
  return filedata->string_table + sh_name;
}

/*
Print Name of Section Header with correct formatting
*/
void section_name(int width, char * symbol){
    unsigned int remaining_width;
    if (width == 0)
        return;

    remaining_width = width;
    while(remaining_width>0){
        char c = *symbol++;
        if (c == 0){
            printf("%.*s", remaining_width, "                   ");
            break;
        }
        if (isprint(c)){
            putchar(c);
            remaining_width--;
        }
    }
}

/*
Get Section Flags
*/
char * get_section_flags(Filedata * filedata, unsigned int flags){
    static char buff[1024];
    char *temp = buff;
    while (flags){
        unsigned int flag;
        flag = flags & - flags;
        flags &= ~ flag;
        switch(flag){
            case SHF_WRITE:		        *temp = 'W'; break;
            case SHF_ALLOC:		        *temp = 'A'; break;
            case SHF_EXECINSTR:		    *temp = 'X'; break;
            case SHF_MERGE:		        *temp = 'M'; break;
            case SHF_STRINGS:		    *temp = 'S'; break;
            case SHF_INFO_LINK:		    *temp = 'I'; break;
            case SHF_LINK_ORDER:	    *temp = 'L'; break;
            case SHF_OS_NONCONFORMING:	*temp = 'O'; break;
            case SHF_GROUP:		        *temp = 'G'; break;
            case SHF_TLS:		        *temp = 'T'; break;
            case SHF_EXCLUDE:		    *temp = 'E'; break;
            case SHF_COMPRESSED:	    *temp = 'C'; break;
            // add x, o, l, y, v
            default:                    *temp = 'x'; break;
        }
        temp++;
    }
    *temp = '\0';
    return buff;
}

/*
Process Section Headers
    Prints Data of section headers and key for flags
*/
bool process_section_headers(Filedata * filedata){
    Elf32_Shdr * section;
    // Verify if error with section headers
    if (filedata->file_header.e_shnum == 0){
        if (filedata->file_header.e_shoff != 0){
            fprintf(stderr, "Section header offset is non-zero, but no section headers\n");
            return false;
        }
        else 
            printf("There are no sections in this file.\n");
        return true;
    }

    // Get String Table for Section Header Names
    if (filedata->file_header.e_shstrndx != SHN_UNDEF
        && filedata->file_header.e_shstrndx < filedata->file_header.e_shnum){
        section = filedata->section_headers + filedata->file_header.e_shstrndx;
        if (section->sh_size != 0){
            filedata->string_table = (char *) get_section (NULL, filedata, section->sh_offset, 1, section->sh_size);
            filedata->string_table_length = filedata->string_table != NULL ? section->sh_size : 0;
        }
        else
            printf("Size is 0???\n");
    }

    // Print Section Header information to output
    printf("\nSection Headers:\n");
    printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    
    int i;
    for (i = 0, section = filedata->section_headers; i < filedata->file_header.e_shnum; i++, section++){
        printf("  [%2u] ", i);
        section_name(17, get_section_name(filedata, section->sh_name));
        printf(" %-15.15s ", get_section_type(filedata, section->sh_type));
        printf("%8.8x" , section->sh_addr);
	    printf( " %6.6lx %6.6lx %2.2lx", (unsigned long) section->sh_offset, (unsigned long) section->sh_size, (unsigned long) section->sh_entsize);
        printf(" %3s ", get_section_flags(filedata, section->sh_flags));
        printf("%2u %3u %2lu\n", section->sh_link, section->sh_info, (unsigned long) section->sh_addralign);

    }

    printf("Key to Flags:\n\
    W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n\
    L (link order), O (extra OS processing required), G (group), T (TLS),\n\
    C (compressed), x (unknown), o (OS specific), E (exclude),  \n    ");
    
    if (filedata->file_header.e_machine == EM_X86_64
        || filedata->file_header.e_machine == EM_L10M
        || filedata->file_header.e_machine == EM_K10M)
        printf("l (large), ");
    else if (filedata->file_header.e_machine == EM_ARM)
        printf("y (purecode), ");
    else if (filedata->file_header.e_machine == EM_PPC)
        printf("v (VLE), ");
    printf("p (processor specific)\n");


    return true;
}