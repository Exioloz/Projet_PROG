#include "elf_header.h"

/*================================================================
    Get Values of File Header from File
  ================================================================*/

/*
Get Elf Class
*/
const char * get_elf_class(unsigned int elf_class){
    switch (elf_class){
        case ELFCLASSNONE:  return "none";
        case ELFCLASS32:    return "ELF32";
        case ELFCLASS64:    return "ELF64";
        default:
            return "<unknown>";
    }
}

/*
Get Elf Data
*/
const char * get_elf_data(unsigned int elf_data){
    switch(elf_data){
        case ELFDATANONE: return ("none");
        case ELFDATA2LSB: return ("2's complement, little endian");
        case ELFDATA2MSB: return ("2's complement, big endian");
        default:
            return ("<unknown>");
    }
}

/*
Get File Type
*/
char * get_file_type(unsigned e_type){
    switch (e_type){
        case ET_NONE:   return ("NONE (None)");
        case ET_REL:    return ("REL (Relocatable file)");
        case ET_EXEC:   return ("EXEC (Executable file)");
        case ET_DYN:    return ("DYN (Shared object file)");
        case ET_CORE:   return ("CORE (Core file)");
        case ET_LOPROC: return ("LOPROC (Processor-specific)");
        case ET_HIPROC: return ("HIPROC (Processor-specific)");
        default:        return ("Not a treated File Type");
    }
}
/*
Get Machine Name
*/
char * get_machine_name(unsigned e_machine){
    switch (e_machine){
        case ET_NONE:        return ("No Machine");
        case EM_M32:         return ("AT&T WE 32100");
        case EM_SPARC:       return ("SPARC");
        case EM_386:         return ("Intel Architecture");
        case EM_68K:         return ("Motorola 68000");
        case EM_88K:         return ("Motorola 88000");
        case EM_860:         return ("Intel 80860");
        case EM_MIPS:        return ("MIPS RS3000 Big-Endian");
        case EM_MIPS_RS4_BE: return ("MIPS RS4000 Big-Endian");
        case EM_ARM:         return ("ARM");
        default: 
            if (e_machine != 9)
                return ("Reserved for future use");
            return ("Not Specified in Elf.pdf"); // e_machine == 9
    }
}

/*
Get File Header

*/
bool get_file_header(Filedata *filedata){
    if (fread(filedata->file_header.e_ident, EI_NIDENT, 1, filedata->file) != 1)
        return false;
    switch (filedata->file_header.e_ident[EI_DATA]){
        case ELFDATA2MSB:
            break;
        default:
        case ELFDATANONE:
        case ELFDATA2LSB:
            fprintf(stderr,"Project only in Big Endian\n");
            return false;
    }
    Elf32_Head header;
    if (fread(header.e_type, sizeof(header) - EI_NIDENT, 1, filedata->file) != 1)
        return false;
    
    filedata->file_header.e_type            = big_endian(header.e_type, sizeof(header.e_type));
    filedata->file_header.e_machine         = big_endian(header.e_machine, sizeof(header.e_machine));
    filedata->file_header.e_version         = big_endian(header.e_version, sizeof(header.e_version));
    filedata->file_header.e_entry           = big_endian(header.e_entry, sizeof(header.e_entry));
    filedata->file_header.e_phoff           = big_endian(header.e_phoff, sizeof(header.e_phoff));
    filedata->file_header.e_shoff           = big_endian(header.e_shoff, sizeof(header.e_shoff));
    filedata->file_header.e_flags           = big_endian(header.e_flags, sizeof(header.e_flags));
    filedata->file_header.e_ehsize          = big_endian(header.e_ehsize, sizeof(header.e_ehsize));
    filedata->file_header.e_phentsize       = big_endian(header.e_phentsize, sizeof(header.e_phentsize));
    filedata->file_header.e_phnum           = big_endian(header.e_phnum, sizeof(header.e_phnum));
    filedata->file_header.e_shentsize       = big_endian(header.e_shentsize, sizeof(header.e_shentsize));
    filedata->file_header.e_shnum           = big_endian(header.e_shnum, sizeof(header.e_shnum));
    filedata->file_header.e_shstrndx        = big_endian(header.e_shstrndx, sizeof(header.e_shstrndx));
    
    return true;
}

/*================================================================
    Process File Header from Obtained Data
  ================================================================*/

/*
Process File Header
Input : filedata
Output : returns a bool to determine whether file header was processed properly
Decodes the data in the file header and prints as readelf
*/
bool process_file_header(Filedata * filedata){
    Elf32_Ehdr * header = & filedata->file_header;
    if (header->e_ident[EI_MAG0] != ELFMAG0 
        || header->e_ident[EI_MAG1] != ELFMAG1 
        || header->e_ident[EI_MAG2] != ELFMAG2 
        || header->e_ident[EI_MAG3] != ELFMAG3){
        printf("Not an ELF file - wrong starting magic bytes \n");
        return false;
    }
    unsigned i;
    printf("Elf Header:\n");
    printf(" Magic:   ");
    for (i = 0; i < EI_NIDENT; i++){
        printf("%2.2x ", header->e_ident[i]);
    }
    printf("\n");
    printf(" Class:                             %s\n", get_elf_class(header->e_ident[EI_CLASS]));
    printf(" Data:                              %s\n", get_elf_data(header->e_ident[EI_DATA]));
    printf(" Version:                           %d%s\n", header->e_ident[EI_VERSION], 
                                                        (header->e_ident[EI_VERSION] == EV_CURRENT ? (" (current)") : 
                                                        (header->e_ident[EI_VERSION] != EV_NONE) ? (" <unknown>") : ""));
    printf(" OS/ABI                             TO DO\n");
    printf(" ABI Version                        %d\n", header->e_ident[EI_ABIVERSION]);
    printf(" Type:                              %s\n", get_file_type(filedata->file_header.e_type));
    printf(" Machine:                           %s\n", get_machine_name(header->e_machine));
    printf(" Version                            0x%x\n", header->e_version);
    printf(" Entry point address                0x%x\n", header->e_entry);
    printf(" Start of program headers           %d (bytes into file)\n", header->e_phoff);
    printf(" Start of section headers:          %d (bytes into file)\n", header->e_shoff);
    printf(" Flags:                             0x%x\n", header->e_flags);
    printf(" Size of this header:               %u (bytes)\n", header->e_ehsize);
    printf(" Size of program headers:           %u (bytes)\n", header->e_phentsize);
    printf(" Number of program headers:         %u", header->e_phnum);
    printf("\n");
    printf(" Size of section headers:           %u (bytes)\n", header->e_shentsize);
    printf(" Number of section headers:         %u", header->e_shnum);
    printf("\n");
    printf(" Section header string table index: %u\n", header->e_shstrndx);
    return true;
}