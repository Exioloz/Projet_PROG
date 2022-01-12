#include "elf_symbol_table.h"

/*
  Function: BigLittleSwa

  Swaps octets to change value from big endian to little endian and vice versa
  returns the value changed
*/
#define BigLittleSwap32(A) ((((uint32_t)(A) & 0xff000000) >> 24) |  (((uint32_t)(A) & 0x00ff0000) >> 8 ) | (((uint32_t)(A) & 0x0000ff00) << 8 ) | (((uint32_t)(A) & 0x000000ff) << 24))
#define BigLittleSwap16(A) ((((uint16_t)(A) & 0xff00) >> 8 ) |  (((uint16_t)(A) & 0x00ff) << 8 ))

/*
  Function: get_st_type

  Obtains st_info in symbol table ((i)&0xf)
  returns the type corresponding
*/
char * get_st_type(Elf32_Sym *symtable, int i) {
    switch (symtable[i].st_info & 0xf) {
        case 0:
            return "NOTYPE";
        case 1:
            return "OBJECT";
        case 2:
            return "FUNC";
        case 3:
            return "SECTION";
        case 4:
            return "FILE";
        case 13:
            return "LOPROC";
        case 15:
            return "HIPROC";

        default:
            return ("unknown");
    }

}
/*
  Function: get_st_bind

  Obtains st_info in symbol table ((i)>>4)
  returns the type corresponding
*/
char * get_st_bind(Elf32_Sym *symtable, int i){
    switch(symtable[i].st_info >> 4){
        case 0: return "LOCAL";
        case 1: return "GLOBAL";
        case 2: return "WEAK";
        case 3: return "NUM";
        case 13: return "LOPROC";
        case 15: return "HIPROC";

        default: return ("unknown");
    }
}

/*
  Function: get_index_strtab

  Takes filedata and a pointer pointing to the start of .shstrtab section
  returns the index of string table where locates the name '.strtab' and other info about it
*/
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

/*
  Function: get_index_symtab

  Takes filedata and a pointer pointing to the start of .shstrtab section
  returns the index of string table where locates the name '.symtab' and other info about it
*/
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

/*
  Function: get_st_value

  returns st value
*/
int get_st_value(Elf32_Sym *symtable, int i){
    return symtable[i].st_value;
}

/*
  Function: get_st_name

  Uses a pointer pointing to the start of .strtab and st name in symtable
  returns a string which shows the real name
*/
char * get_st_name(Elf32_Sym *symtable, int i,char * strtemp){
    strtemp=strtemp+BigLittleSwap32(symtable[i].st_name);
    return strtemp;
}

/*
  Function: get_st_value

  returns st size
*/
int get_st_size(Elf32_Sym *symtable, int i){
    return symtable[i].st_size;
}
/*
  Function: get_st_vis

  Obtains st_other in symbol table
  returns the visibility corresponding
*/
char * get_st_vis(unsigned int other){
    switch(other){
        case 0: return "DEFAULT\t";
        default: return ("unknown\t");
    }

}

/*
    Function: get_symbol_table

    Gets the data relating to symbol table from the ELF file
    and puts it into filedata.
*/

bool get_symbol_table(Filedata * filedata) {
    int size = filedata->section_headers[filedata->file_header.e_shstrndx].sh_size;
    int offset = filedata->section_headers[filedata->file_header.e_shstrndx].sh_offset;

    rewind(filedata->file);

    fseek(filedata->file, offset, SEEK_SET);
    char shstrtab[size];
    char *temp = shstrtab;

    size_t a = fread(shstrtab, size, 1, filedata->file);
    if (0 == a) {
        printf("\nfail to read\n");
        return false;
    }
    int index_strtab = 0;
    index_strtab = get_index_strtab(filedata, temp, index_strtab);
    fseek(filedata->file, filedata->section_headers[index_strtab].sh_offset, SEEK_SET);
    char strtab[filedata->section_headers[index_strtab].sh_size];
    a = fread(strtab, filedata->section_headers[index_strtab].sh_size, 1, filedata->file);
    if (0 == a) {
        printf("\nfail to read\n");
        return false;
    }

    int index_symtab = 0;
    index_symtab = get_index_symtab(filedata, temp, index_symtab);
    int number_sym = filedata->section_headers[index_symtab].sh_size / filedata->section_headers[index_symtab].sh_entsize;

    filedata->symbol_table.sym_tab_num = number_sym;
    filedata->symbol_table.sym_entries = (Elf32_Sym *) malloc(sizeof(Elf32_Sym) * number_sym);
    if (filedata->symbol_table.sym_entries == NULL) {
        fprintf(stderr, "Not able to allocate memory for symtable");
        return false;
    }

    fseek(filedata->file, filedata->section_headers[index_symtab].sh_offset, SEEK_SET);

    a = fread(filedata->symbol_table.sym_entries, sizeof(Elf32_Sym) * number_sym, 1, filedata->file);
    if (0 == a) {
        printf("\nfail to read\n");
        return false;
    }

    return true;
}

/*================================================================
    Process symbol table from Obtained Data
  ================================================================*/

/*
Process Symbol Table
Input : filedata
Output : returns a bool to determine whether symbol table was processed properly
Obtain the data from the structure and print them as readelf
*/

bool process_symbol_table(Filedata * filedata){
    Elf32_Sym_Tab sym_tab = filedata->symbol_table;
    Elf32_Half number_sym = sym_tab.sym_tab_num;
    Elf32_Sym* symtable = sym_tab.sym_entries;

    int size = filedata->section_headers[filedata->file_header.e_shstrndx].sh_size;
    int offset = filedata->section_headers[filedata->file_header.e_shstrndx].sh_offset;

    rewind(filedata->file);

    fseek(filedata->file, offset, SEEK_SET);
    char shstrtab[size];
    char *temp = shstrtab;

    size_t a = fread(shstrtab, size, 1, filedata->file);
    if (0 == a) {
        printf("\nfail to read\n");
        return false;
    }
    int index_strtab = 0;
    index_strtab = get_index_strtab(filedata, temp, index_strtab);
    fseek(filedata->file, filedata->section_headers[index_strtab].sh_offset, SEEK_SET);
    char strtab[filedata->section_headers[index_strtab].sh_size];
    a = fread(strtab, filedata->section_headers[index_strtab].sh_size, 1, filedata->file);
    if (0 == a) {
        printf("\nfail to read\n");
        return false;
    }
    char *strtemp = strtab;
    
    printf("Symbol table '.symtab' contains %d entries:\n", number_sym);
            printf("%7s  %-8s %s   %s      %s    %s          %s    %s\n",
           "Num:", "Value", "Size", "Type", "Bind", "Vis", "Ndx", "Name");
    for (int i=0; i<number_sym; i++) {
        printf("%6d:  %08x  %-6u", i, change_endian_32(get_st_value(symtable, i)), get_st_size(symtable, i));

        printf("%-8s ", get_st_type(symtable,i));
        printf(" %-8s", get_st_bind(symtable,i));
        printf("%s", get_st_vis(symtable[i].st_other));
        switch (BigLittleSwap16(symtable[i].st_shndx)) {
            case 0:
                printf("UND ");
                break;case 0xfff1:
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
        printf("%s\n", get_st_name(symtable, i, strtemp));

    }
    return true;
}
