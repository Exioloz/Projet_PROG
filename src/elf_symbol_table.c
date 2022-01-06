#include "elf_symbol_table.h"

#define BigLittleSwap32(A) ((((uint32_t)(A) & 0xff000000) >> 24) |  (((uint32_t)(A) & 0x00ff0000) >> 8 ) | (((uint32_t)(A) & 0x0000ff00) << 8 ) | (((uint32_t)(A) & 0x000000ff) << 24))
#define BigLittleSwap16(A) ((((uint16_t)(A) & 0xff00) >> 8 ) |  (((uint16_t)(A) & 0x00ff) << 8 ))

char * get_st_type(Elf32_Sym *symtable, int i) {
    switch (symtable[i].st_info & 0xf) {
        case 0:     return "NOTYPE";
        case 1:     return "OBJECT";
        case 2:     return "FUNC";
        case 3:     return "SECTION";
        case 4:     return "FILE";
        case 13:    return "LOPROC";
        case 15:    return "HIPROC";
        default:    return ("unknown");
    }

}

char * get_st_bind(Elf32_Sym *symtable, int i){
    switch(symtable[i].st_info >> 4){
        case 0:     return "LOCAL";
        case 1:     return "GLOBAL";
        case 2:     return "WEAK";
        case 3:     return "NUM";
        case 13:    return "LOPROC";
        case 15:    return "HIPROC";
        default:    return ("unknown");
    }
}


int get_index_strtab(Filedata * filedata, char * temp, int index_strtab){
    char * a = temp;
    for (int i=0; i < filedata->file_header.e_shnum; i++){
        temp=a;
        temp=temp+filedata->section_headers[i].sh_name;
        if(strcmp(temp, ".strtab")!=0) continue;
        index_strtab=i;
    }
    return index_strtab;
}
int get_index_symtab(Filedata * filedata, char * temp, int index_symtab){
    char * a = temp;
    for (int i = 0; i < filedata->file_header.e_shnum; i++){
        temp = a;
        temp = temp + filedata->section_headers[i].sh_name;
        if (strcmp(temp, ".symtab") != 0) continue;
        index_symtab = i;
    }
    return index_symtab;
}

int get_st_value(Elf32_Sym *symtable, int i){
    return symtable[i].st_value;
}

char * get_st_name(Elf32_Sym *symtable, int i,char * strtemp){
    strtemp=strtemp+BigLittleSwap32(symtable[i].st_name);
    return strtemp;
}

int get_st_size(Elf32_Sym *symtable, int i){
    return symtable[i].st_size;
}

char * get_st_vis(unsigned int other){
    switch(other){
        case 0: return "DEFAULT\t";
        default: return ("unknown\t");
    }

}

int process_symbol_table(Filedata * filedata){
    rewind(filedata->file);

    fseek(filedata->file, filedata->section_headers[filedata->file_header.e_shstrndx].sh_offset, SEEK_SET);
    char shstrtab[filedata->section_headers[filedata->file_header.e_shstrndx].sh_size];
    char *temp = shstrtab;

    size_t a = fread(shstrtab, filedata->section_headers[filedata->file_header.e_shstrndx].sh_size, 1, filedata->file);
    if (0 == a){
        fprintf(stderr,"\nFail to read shstrtab\n");
        return false;
    }
    int index_strtab = 0;
    index_strtab=get_index_strtab(filedata, temp, index_strtab);
    fseek(filedata->file, filedata->section_headers[index_strtab].sh_offset, SEEK_SET);
    char strtab[filedata->section_headers[index_strtab].sh_size];
    a = fread(strtab, filedata->section_headers[index_strtab].sh_size, 1, filedata->file);
    if (0 == a) {
        fprintf(stderr,"\nFailed to read strtab\n");
        return false;
    }
    char *strtemp=strtab;

    int index_symtab = 0;
    index_symtab =  get_index_symtab(filedata, temp, index_symtab);
    int number_sym = filedata->section_headers[index_symtab].sh_size / filedata->section_headers[index_symtab].sh_entsize;
    printf("La table de symboles << .symtab >> contient %d entrees :\n", number_sym);
    Elf32_Sym *symtable=(Elf32_Sym*)malloc(sizeof(Elf32_Sym) * number_sym);
    if (symtable == NULL){
        fprintf(stderr, "Not able to allocate memory for symbol table\n");
        return false;
    }

    fseek(filedata->file, filedata->section_headers[index_symtab].sh_offset, SEEK_SET);

    a = fread(symtable, sizeof(Elf32_Sym)*number_sym, 1, filedata->file);
    if (0 == a){
        fprintf(stderr,"\nFailed to read symbol table\n");
        return false;
    }

    printf("%7s  %-8s %s   %s      %s    %s          %s    %s\r\n",
           "Num:", "Value", "Size", "Type", "Bind", "Vis", "Ndx", "Name");
    for (int i=0; i<number_sym; i++) {
        printf("%6d:  %08x  %-6u", i, get_st_value(symtable, i), get_st_size(symtable, i));

        printf("%-8s ", get_st_type(symtable,i));
        printf(" %-8s", get_st_bind(symtable,i));
        printf("%s", get_st_vis(symtable[i].st_other));
        switch (BigLittleSwap16(symtable[i].st_shndx)){
            case 0:
                printf("UND ");
                break;
            case 0xfff1:
                printf("ABS ");
                break;
            case 0xfff2:
                printf("COMMON ");
                break;
            default:
                printf("%hu\t", BigLittleSwap16(symtable[i].st_shndx));
                break;

        }
        strtemp=strtab;
        printf("%s\r\n", get_st_name(symtable, i, strtemp));
    }
    return true;

}
