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
        case EM_NONE:           return("No machine"); break;
        case EM_M32:            return("AT&T WE 32100"); break;
        case EM_SPARC:          return("SUN SPARC"); break;
        case EM_386:            return("Intel 80386"); break;
        case EM_68K:            return("Motorola m68k family"); break;
        case EM_88K:            return("Motorola m88k family"); break;
        case EM_IAMCU:          return("Intel MCU"); break;
        case EM_860:            return("Intel 80860"); break;
        case EM_MIPS:           return("MIPS R3000 big-endian"); break;
        case EM_S370:           return("IBM System/370"); break;
        case EM_MIPS_RS3_LE:    return("MIPS R3000 little-endian"); break;
                        /* reserved 11-14 */
        case EM_PARISC:         return("HPPA"); break;
                        /* reserved 16 */
        case EM_VPP500:         return("Fujitsu VPP500"); break;
        case EM_SPARC32PLUS:    return("Sun's \"v8plus\""); break;
        case EM_960:            return("Intel 80960"); break;
        case EM_PPC:            return("PowerPC"); break;
        case EM_PPC64:          return("PowerPC 64-bit"); break;
        case EM_S390:           return("IBM S390"); break;
        case EM_SPU:            return("IBM SPU/SPC"); break;
                        /* reserved 24-35 */
        case EM_V800:           return("NEC V800 series"); break;
        case EM_FR20:           return("Fujitsu FR20"); break;
        case EM_RH32:           return("TRW RH-32"); break;
        case EM_RCE:            return("Motorola RCE"); break;
        case EM_ARM:            return("ARM"); break;
        case EM_FAKE_ALPHA:     return("Digital Alpha"); break;
        case EM_SH:             return("Hitachi SH"); break;
        case EM_SPARCV9:        return("SPARC v9 64-bit"); break;
        case EM_TRICORE:        return("Siemens Tricore"); break;
        case EM_ARC:            return("Argonaut RISC Core"); break;
        case EM_H8_300:         return("Hitachi H8/300"); break;
        case EM_H8_300H:        return("Hitachi H8/300H"); break;
        case EM_H8S:            return("Hitachi H8S"); break;
        case EM_H8_500:         return("Hitachi H8/500"); break;
        case EM_IA_64:          return("Intel Merced"); break;
        case EM_MIPS_X:         return("Stanford MIPS-X"); break;
        case EM_COLDFIRE:       return("Motorola Coldfire"); break;
        case EM_68HC12:         return("Motorola M68HC12"); break;
        case EM_MMA:            return("Fujitsu MMA Multimedia Accelerator"); break;
        case EM_PCP:            return("Siemens PCP"); break;
        case EM_NCPU:           return("Sony nCPU embeeded RISC"); break;
        case EM_NDR1:           return("Denso NDR1 microprocessor"); break;
        case EM_STARCORE:       return("Motorola Start*Core processor"); break;
        case EM_ME16:           return("Toyota ME16 processor"); break;
        case EM_ST100:          return("STMicroelectronic ST100 processor"); break;
        case EM_TINYJ:          return("Advanced Logic Corp. Tinyj emb.fam"); break;
        case EM_X86_64:         return("AMD x86-64 architecture"); break;
        case EM_PDSP:           return("Sony DSP Processor"); break;
        case EM_PDP10:          return("Digital PDP-10"); break;
        case EM_PDP11:          return("Digital PDP-11"); break;
        case EM_FX66:           return("Siemens FX66 microcontroller"); break;
        case EM_ST9PLUS:        return("STMicroelectronics ST9+ 8/16 mc"); break;
        case EM_ST7:            return("STmicroelectronics ST7 8 bit mc"); break;
        case EM_68HC16:         return("Motorola MC68HC16 microcontroller"); break;
        case EM_68HC11:         return("Motorola MC68HC11 microcontroller"); break;
        case EM_68HC08:         return("Motorola MC68HC08 microcontroller"); break;
        case EM_68HC05:         return("Motorola MC68HC05 microcontroller"); break;
        case EM_SVX:            return("Silicon Graphics SVx"); break;
        case EM_ST19:           return("STMicroelectronics ST19 8 bit mc"); break;
        case EM_VAX:            return("Digital VAX"); break;
        case EM_CRIS:           return("Axis Communications 32-bit emb.proc"); break;
        case EM_JAVELIN:        return("Infineon Technologies 32-bit emb.proc"); break;
        case EM_FIREPATH:       return("Element 14 64-bit DSP Processor"); break;
        case EM_ZSP:            return("LSI Logic 16-bit DSP Processor"); break;
        case EM_MMIX:           return("Donald Knuth's educational 64-bit proc"); break;
        case EM_HUANY:          return("Harvard University machine-independent object files"); break;
        case EM_PRISM:          return("SiTera Prism"); break;
        case EM_AVR:            return("Atmel AVR 8-bit microcontroller"); break;
        case EM_FR30:           return("Fujitsu FR30"); break;
        case EM_D10V:           return("Mitsubishi D10V"); break;
        case EM_D30V:           return("Mitsubishi D30V"); break;
        case EM_V850:           return("NEC v850"); break;
        case EM_M32R:           return("Mitsubishi M32R"); break;
        case EM_MN10300:        return("Matsushita MN10300"); break;
        case EM_MN10200:        return("Matsushita MN10200"); break;
        case EM_PJ:             return("picoJava"); break;
        case EM_OPENRISC:       return("OpenRISC 32-bit embedded processor"); break;
        case EM_ARC_COMPACT:    return("ARC International ARCompact"); break;
        case EM_XTENSA:         return("Tensilica Xtensa Architecture"); break;
        case EM_VIDEOCORE:      return("Alphamosaic VideoCore"); break;
        case EM_TMM_GPP:        return("Thompson Multimedia General Purpose Proc"); break;
        case EM_NS32K:          return("National Semi. 32000"); break;
        case EM_TPC:            return("Tenor Network TPC"); break;
        case EM_SNP1K:          return("Trebia SNP 1000"); break;
        case EM_ST200:          return("STMicroelectronics ST200"); break;
        case EM_IP2K:           return("Ubicom IP2xxx"); break;
        case EM_MAX:            return("MAX processor"); break;
        case EM_CR:             return("National Semi. CompactRISC"); break;
        case EM_F2MC16:         return("Fujitsu F2MC16"); break;
        case EM_MSP430:         return("Texas Instruments msp430"); break;
        case EM_BLACKFIN:       return("Analog Devices Blackfin DSP"); break;
        case EM_SE_C33:         return("Seiko Epson S1C33 family"); break;
        case EM_SEP:            return("Sharp embedded microprocessor"); break;
        case EM_ARCA:           return("Arca RISC"); break;
        case EM_UNICORE:        return("PKU-Unity & MPRC Peking Uni. mc series"); break;
        case EM_EXCESS:         return("eXcess configurable cpu"); break;
        case EM_DXP:            return("Icera Semi. Deep Execution Processor"); break;
        case EM_ALTERA_NIOS2:   return("Altera Nios II"); break;
        case EM_CRX:            return("National Semi. CompactRISC CRX"); break;
        case EM_XGATE:          return("Motorola XGATE"); break;
        case EM_C166:           return("Infineon C16x/XC16x"); break;
        case EM_M16C:           return("Renesas M16C"); break;
        case EM_DSPIC30F:       return("Microchip Technology dsPIC30F"); break;
        case EM_CE:             return("Freescale Communication Engine RISC"); break;
        case EM_M32C:           return("Renesas M32C"); break;
                        /* reserved 121-130 */
        case EM_TSK3000:        return("Altium TSK3000"); break;
        case EM_RS08:           return("Freescale RS08"); break;
        case EM_SHARC:          return("Analog Devices SHARC family"); break;
        case EM_ECOG2:          return("Cyan Technology eCOG2"); break;
        case EM_SCORE7:         return("Sunplus S+core7 RISC"); break;
        case EM_DSP24:          return("New Japan Radio (NJR) 24-bit DSP"); break;
        case EM_VIDEOCORE3:     return("Broadcom VideoCore III"); break;
        case EM_LATTICEMICO32:  return("RISC for Lattice FPGA"); break;
        case EM_SE_C17:         return("Seiko Epson C17"); break;
        case EM_TI_C6000:       return("Texas Instruments TMS320C6000 DSP"); break;
        case EM_TI_C2000:       return("Texas Instruments TMS320C2000 DSP"); break;
        case EM_TI_C5500:       return("Texas Instruments TMS320C55x DSP"); break;
        case EM_TI_ARP32:       return("Texas Instruments App. Specific RISC"); break;
        case EM_TI_PRU:         return("Texas Instruments Prog. Realtime Unit"); break;
                        /* reserved 145-159 */
        case EM_MMDSP_PLUS:     return("STMicroelectronics 64bit VLIW DSP"); break;
        case EM_CYPRESS_M8C:    return("Cypress M8C"); break;
        case EM_R32C:           return("Renesas R32C"); break;
        case EM_TRIMEDIA:       return("NXP Semi. TriMedia"); break;
        case EM_QDSP6:          return("QUALCOMM DSP6"); break;
        case EM_8051:           return("Intel 8051 and variants"); break;
        case EM_STXP7X:         return("STMicroelectronics STxP7x"); break;
        case EM_NDS32:          return("Andes Tech. compact code emb. RISC"); break;
        case EM_ECOG1X:         return("Cyan Technology eCOG1X"); break;
        case EM_MAXQ30:         return("Dallas Semi. MAXQ30 mc"); break;
        case EM_XIMO16:         return("New Japan Radio (NJR) 16-bit DSP"); break;
        case EM_MANIK:          return("M2000 Reconfigurable RISC"); break;
        case EM_CRAYNV2:        return("Cray NV2 vector architecture"); break;
        case EM_RX:             return("Renesas RX"); break;
        case EM_METAG:          return("Imagination Tech. META"); break;
        case EM_MCST_ELBRUS:    return("MCST Elbrus"); break;
        case EM_ECOG16:         return("Cyan Technology eCOG16"); break;
        case EM_CR16:           return("National Semi. CompactRISC CR16"); break;
        case EM_ETPU:           return("Freescale Extended Time Processing Unit"); break;
        case EM_SLE9X:          return("Infineon Tech. SLE9X"); break;
        case EM_L10M:           return("Intel L10M"); break;
        case EM_K10M:           return("Intel K10M"); break;
                        /* reserved 182 */
        case EM_AARCH64:        return("ARM AARCH64"); break;
                        /* reserved 184 */
        case EM_AVR32:          return("Amtel 32-bit microprocessor"); break;
        case EM_STM8:           return("STMicroelectronics STM8"); break;
        case EM_TILE64:         return("Tileta TILE64"); break;
        case EM_TILEPRO:        return("Tilera TILEPro"); break;
        case EM_MICROBLAZE:     return("Xilinx MicroBlaze"); break;
        case EM_CUDA:           return("NVIDIA CUDA"); break;
        case EM_TILEGX:         return("Tilera TILE-Gx"); break;
        case EM_CLOUDSHIELD:    return("CloudShield"); break;
        case EM_COREA_1ST:      return("KIPO-KAIST Core-A 1st gen."); break;
        case EM_COREA_2ND:      return("KIPO-KAIST Core-A 2nd gen."); break;
        case EM_ARC_COMPACT2:   return("Synopsys ARCompact V2"); break;
        case EM_OPEN8:          return("Open8 RISC"); break;
        case EM_RL78:           return("Renesas RL78"); break;
        case EM_VIDEOCORE5:     return("Broadcom VideoCore V"); break;
        case EM_78KOR:          return("Renesas 78KOR"); break;
        case EM_56800EX:        return("Freescale 56800EX DSC"); break;
        case EM_BA1:            return("Beyond BA1"); break;
        case EM_BA2:            return("Beyond BA2"); break;
        case EM_XCORE:          return("XMOS xCORE"); break;
        case EM_MCHP_PIC:       return("Microchip 8-bit PIC(r)"); break;
                        /* reserved 205-209 */
        case EM_KM32:           return("KM211 KM32"); break;
        case EM_KMX32:          return("KM211 KMX32"); break;
        case EM_EMX16:          return("KM211 KMX16"); break;
        case EM_EMX8:           return("KM211 KMX8"); break;
        case EM_KVARC:          return("KM211 KVARC"); break;
        case EM_CDP:            return("Paneve CDP"); break;
        case EM_COGE:           return("Cognitive Smart Memory Processor"); break;
        case EM_COOL:           return("Bluechip CoolEngine"); break;
        case EM_NORC:           return("Nanoradio Optimized RISC"); break;
        case EM_CSR_KALIMBA:    return("CSR Kalimba"); break;
        case EM_Z80:            return("Zilog Z80"); break;
        case EM_VISIUM:         return("Controls and Data Services VISIUMcore"); break;
        case EM_FT32:           return("FTDI Chip FT32"); break;
        case EM_MOXIE:          return("Moxie processor"); break;
        case EM_AMDGPU:         return("AMD GPU"); break;
                        /* reserved 225-242 */
        case EM_RISCV:          return("RISC-V"); break;
        case EM_BPF:            return("Linux BPF -- in-kernel virtual machine"); break;
        default:                return("Reserved for future use"); break;
    }
}

/*
Get OS/ABI Name
*/
char * get_osabi(Filedata *filedata, unsigned int osabi){
    switch (osabi){
        case ELFOSABI_NONE:		return "UNIX - System V";
        case ELFOSABI_HPUX:		return "UNIX - HP-UX";
        case ELFOSABI_NETBSD:	return "UNIX - NetBSD";
        case ELFOSABI_GNU:		return "UNIX - GNU";
        case ELFOSABI_SOLARIS:	return "UNIX - Solaris";
        case ELFOSABI_AIX:		return "UNIX - AIX";
        case ELFOSABI_IRIX:		return "UNIX - IRIX";
        case ELFOSABI_FREEBSD:	return "UNIX - FreeBSD";
        case ELFOSABI_TRU64:	return "UNIX - TRU64";
        case ELFOSABI_MODESTO:	return "Novell - Modesto";
        case ELFOSABI_OPENBSD:	return "UNIX - OpenBSD";
    default:
        if (osabi >= 64)
	    switch (filedata->file_header.e_machine){
	        case EM_ARM:
	            switch (osabi){
	                case ELFOSABI_ARM:	return "ARM";
	            default:
		            break;
	            }
	            break;
            default: break;
    }
    return "<unknown>";
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
    printf(" OS/ABI:                            %s\n", get_osabi(filedata, header->e_ident[EI_OSABI]));
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