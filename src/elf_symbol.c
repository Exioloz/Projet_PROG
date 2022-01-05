#include "elf_symbol.h"

Elf32_Sym * get_symbols(Filedata *filedata, Elf32_Shdr *section, int *num){
    Elf32_Sym * temp = NULL;
    if (section->sh_size == 0){
        if (num != NULL)
	        *num = 0;
        printf("SECTION NULL\n");
        return NULL;
    }
    int size = section->sh_size / section->sh_entsize;
    if (size * sizeof (Elf_Sym) > section->sh_size + 1)
        fprintf(stderr,"Size of section is not a multiple of its sh_entsize (0x%lx)\n", (unsigned long)section->sh_entsize);
    
    Elf_Sym * sym = (Elf_Sym *) get_section(NULL, filedata, section->sh_offset, 1, section->sh_size);
    if (sym == NULL)
        goto exit;
    temp = (Elf32_Sym*)malloc(sizeof(Elf32_Sym)*size);
    Elf32_Sym * psym;
    if (temp == NULL){
        fprintf(stderr, "Out of memory reading symbols \n");
        goto exit;
    }
    int j;
    for (j=0, psym = temp; j < size; j++, psym++){
        psym->st_name = big_endian(sym[j].st_name, sizeof(sym[j].st_name));
        psym->st_value = big_endian(sym[j].st_value, sizeof(sym[j].st_value));
        psym->st_size = big_endian(sym[j].st_size, sizeof(sym[j].st_size));
        psym->st_shndx = big_endian(sym[j].st_shndx, sizeof(sym[j].st_shndx));
        psym->st_info = big_endian(sym[j].st_info, sizeof(sym[j].st_info));
        psym->st_other = big_endian(sym[j].st_other, sizeof(sym[j].st_other));
    }
    exit:
        free(sym);
        if (num != NULL)
            *num = temp == NULL ? 0 : size;
        return temp;

}

char * get_symbol_type(Filedata *filedata, int type){
    switch (type){
        case STT_NOTYPE:	return "NOTYPE";
        case STT_OBJECT:	return "OBJECT";
        case STT_FUNC:	    return "FUNC";
        case STT_SECTION:	return "SECTION";
        case STT_FILE:	    return "FILE";
        case STT_COMMON:	return "COMMON";
        case STT_TLS:	    return "TLS";
        default:            return "<unknown>";
    }
}

char * get_symbol_bind(Filedata *filedata, int bind){
    switch (bind){
        case STB_LOCAL:	    return "LOCAL";
        case STB_GLOBAL:	return "GLOBAL";
        case STB_WEAK:	    return "WEAK";
        default:            return "<unknown>";
    }
}

char *get_symbol_visibility (int visibility){
    switch (visibility){
        case STV_DEFAULT:	return "DEFAULT";
        case STV_INTERNAL:	return "INTERNAL";
        case STV_HIDDEN:	return "HIDDEN";
        case STV_PROTECTED: return "PROTECTED";
        default:
            fprintf(stderr,"Unrecognized visibility value: %u\n", visibility);
            return "<unknown>";
        }
}

char * get_symbol_index_type (Filedata * filedata, unsigned int type){
    static char buff[32];
    switch (type){
        case SHN_UNDEF:	    return "UND";
        case SHN_ABS:	    return "ABS";
        case SHN_COMMON:	return "COM";
        default:
            sprintf(buff, "%3d", type);
            break;
    }
    return buff;
}

void print_symbol(Filedata * filedata, int num, Elf32_Sym * symtab, Elf32_Shdr * section, char *strtab, int strtab_size){
    Elf32_Sym * temp_sym;
    temp_sym = symtab + num;
    printf("%6d: ",  num);
    printf("%08x ", temp_sym->st_value);
    printf("%5d ", temp_sym->st_size);
    printf("%-7s ", get_symbol_type(filedata, ELF32_ST_TYPE(temp_sym->st_info)));
    printf("%-6s ", get_symbol_bind(filedata, ELF32_ST_BIND(temp_sym->st_info)));
    printf("%-7s ", get_symbol_visibility(ELF32_ST_VISIBILITY(temp_sym->st_other)));
    printf("%4s ", get_symbol_index_type(filedata, temp_sym->st_shndx));
    // Print Name ?????
    return;
}

bool process_symbol_table(Filedata * filedata){
    Elf32_Shdr * section;
    
    if (filedata->section_headers != NULL){
        int i;
        for (i = 0, section = filedata->section_headers; i < filedata->file_header.e_shnum; i++, section++){
            char * strtab = NULL;
            Elf32_Sym * symtab;
            unsigned int strtab_size = 0;

            if (section->sh_type != SHT_SYMTAB && section->sh_type != SHT_DYNSYM)
                continue;
            if (section->sh_entsize == 0){
                fprintf(stderr, "\nSymbol Table %d has sh_entsize of zero\n", i);
                continue;
            }
            int num;
            num = section->sh_size / section->sh_entsize;
            printf("\nSymbol table '%s' contains %d entries:\n", get_section_name(filedata, section->sh_name), num);
            printf ("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");
            symtab = get_symbols(filedata, section, &num);
            
            if (symtab == NULL)
                continue;
            if (section->sh_link == filedata->file_header.e_shstrndx){
                strtab = filedata->string_table;
                strtab_size = filedata->string_table_length;
            } 
            else if (section->sh_link < filedata->file_header.e_shnum){
                Elf32_Shdr * string;
                string = filedata->section_headers + section->sh_link;
                strtab = (char *)get_section(NULL, filedata, string->sh_offset, 1, string->sh_size);
                strtab_size = string != NULL ? string->sh_size : 0;
            }
            int j;
            for (j=0; j < num; j++){
                print_symbol(filedata, j, symtab, section, strtab, strtab_size);
                printf("\n");
            }
            free(symtab);
            if (strtab != filedata->string_table)
                free(strtab);
        }   
    }
    return true;
}