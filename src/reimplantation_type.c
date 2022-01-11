#include "reimplantation_type.h"

void implantation(Elf32_Addr * text_addr, Elf32_Addr * data_addr, Filedata * filedata, Filedata * newfile){
    Elf32_Ext_Rel * ext_tab;
    ext_tab = filedata->reloc_table.rel_tab;
    for(int i = 0; i < filedata->reloc_table.rel_num; i++ ) {
        if (strcmp(get_section_name(filedata, ext_tab[i].rel_sh_name), ".rel.text")==0) {
            calcul_val(ext_tab, text_addr, filedata, i, newfile);
        } else if (strcmp(get_section_name(filedata, ext_tab[i].rel_sh_name), ".rel.data")==0) {
            calcul_val(ext_tab, data_addr, filedata, i, newfile);
        }
    }

}

void calcul_val(Elf32_Ext_Rel *  ext_tab, Elf32_Addr * addr, Filedata * filedata, int index, Filedata * newfile){
    Elf32_Sym* symtable = filedata->symbol_table.sym_entries;
    uint32_t val_symbol=0;
    int idx=0;

    for (int i = 0; i < ext_tab[index].rel_ent_num; i++){
        Elf32_Addr * addr_p= malloc(sizeof(Elf32_Addr));
        switch (ELF32_R_TYPE(change_endian_32(ext_tab[index].rel_ents[i].r_info))){
            case R_ARM_ABS32:
            case R_ARM_ABS16:
            case R_ARM_ABS8: //S
                idx = ELF32_R_SYM(change_endian_32(ext_tab[index].rel_ents[i].r_info));
                val_symbol=(uint32_t)get_st_value(symtable, idx);
                *addr_p = *addr;
                *addr_p=*addr+change_endian_32(ext_tab[index].rel_ents[i].r_offset);
                replace_data(filedata, val_symbol, addr_p, newfile);//replace the value starts at p by addr_symbol
                free(addr_p);
                break;
            case R_ARM_CALL:
            case R_ARM_JUMP24: //S - P
                idx = ELF32_R_SYM(change_endian_32(ext_tab[index].rel_ents[i].r_info));
                val_symbol=get_st_value(symtable, idx);
                *addr_p = *addr;
                val_symbol=val_symbol-(*addr+change_endian_32(ext_tab[index].rel_ents[i].r_offset));
                *addr_p=*addr+change_endian_32(ext_tab[index].rel_sh_offset);
                replace_data(filedata,val_symbol, addr_p, newfile);
                free(addr_p);
                break;
            default:
                break;
        }
    }

}

void replace_data(Filedata * filedata, uint32_t val_symbol, Elf32_Addr * addr_p, Filedata * newfile){
    int a = 0;
    printf("\nVal:%x\n",val_symbol);
    printf("\nAddr_p:%x\n",*addr_p);

    //  char * file_name="2";
    //  newfile->file = fopen(file_name, "rb+");
    if (newfile->file == NULL){
        fprintf(stderr,"Input file %s is not readble.\n", newfile->file_name);
        free_filedata(newfile);
    }
    rewind(newfile->file);
    a = fseek(newfile->file,*addr_p,SEEK_SET);
    //  a = fseek(newfile->file, 0x20, SEEK_SET);

    char * str = "hello world";
    //  val_symbol=1234;
    //  a = fwrite( &val_symbol, sizeof(val_symbol) , 1, newfile->file);
    a = fwrite( str, sizeof(uint32_t) , 1, newfile->file);
    if (0 == a) {
        printf("\nfail to write\n");
    }
}