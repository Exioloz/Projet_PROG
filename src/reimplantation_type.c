#include "reimplantation_type.h"

void implantation(Filedata * filedata, Filedata * newfile){
    int i,j;
    Elf32_Rel_Tab rels = filedata->reloc_table;
    Elf32_Ext_Rel * ext_tab = rels.rel_tab;
    Elf32_Shdr *sec_hdr = newfile->section_headers ;
    char *sec_name;

    for(i = 0; i < rels.rel_num ; i++ ) {
        sec_name = &get_section_name(filedata, ext_tab[i].rel_sh_name)[4] ;
        for(j = 0 ; j < newfile->file_header.e_shnum ; j++){
            //compare section names in relo table with names in section header
           if(strcmp(sec_name, get_section_name(newfile, sec_hdr[j].sh_name))==0){
                calcul_val(ext_tab, &sec_hdr[j].sh_addr, sec_hdr[j].sh_offset, filedata, i, newfile);
            }
        }   
    }
}

void calcul_val(Elf32_Ext_Rel *  ext_tab, Elf32_Addr * addr,Elf32_Off offset_sec, Filedata * filedata, int index, Filedata * newfile){
    uint32_t val_symbol=0;
    int idx=0;
    Elf32_Sym *new_sym_tab = newfile->symbol_table.sym_entries;

    for (int i = 0; i < ext_tab[index].rel_ent_num; i++){
        Elf32_Addr * addr_p= malloc(sizeof(Elf32_Addr));
        switch (ELF32_R_TYPE(change_endian_32(ext_tab[index].rel_ents[i].r_info))){
            case R_ARM_ABS32:
            case R_ARM_ABS16:
            case R_ARM_ABS8: //S
                //get index of the variable in symbol table
                idx = ELF32_R_SYM(change_endian_32(ext_tab[index].rel_ents[i].r_info));
                //get its value from symbol table
                val_symbol=(uint32_t)(get_st_value(new_sym_tab, idx));
                //addr_p = offset of the section where locates values we need to relocate + value's offset in relo table
                *addr_p=offset_sec+change_endian_32(ext_tab[index].rel_ents[i].r_offset);
                replace_data(filedata, val_symbol, addr_p, newfile);
                free(addr_p);
                break;
            case R_ARM_CALL:
            case R_ARM_JUMP24: //S - P
                idx = ELF32_R_SYM(change_endian_32(ext_tab[index].rel_ents[i].r_info));
                val_symbol=(uint32_t)change_endian_32(get_st_value(new_sym_tab, idx));
                //S-P, the new value = value from the new symbol table - (the addr of the section in the memory + value's offset in relo table)
                val_symbol=(val_symbol-(*addr+change_endian_32(ext_tab[index].rel_ents[i].r_offset)));
                //val & mask asked by the manuel, >> 2 cuz the first byte is type
                val_symbol=change_endian_32((val_symbol & 0x03FFFFFE)>>2);
                //addr_p = offset of the section where locates values we need to relocate + value's offset in relo table
                *addr_p=offset_sec+change_endian_32(ext_tab[index].rel_ents[i].r_offset);
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

        if (newfile->file == NULL){
            fprintf(stderr,"Input file %s is not readble.\n", newfile->file_name);
            free_filedata(newfile);
        }

    a = fseek(newfile->file,*addr_p,SEEK_SET);
    // the original data in the section
    uint8_t *data_old = malloc(sizeof(uint8_t)*4);
    a = fread(data_old, sizeof(uint32_t),1,newfile->file);
    // val_symbol copied to data_sym to manipulate as an array
    uint8_t *data_sym = malloc(sizeof(uint8_t)*4);
    memmove(data_sym, &val_symbol, 4);

    // compare every byte in data_old with 0xff and 0
    // if true, then data_old takes the corresponding byte in data_sym
    // if false, data_old = data_old + data_sym
    if(data_old[1] == 0xff || data[1] == 0){
        data_old [1]=data_sym[1];
    }
    else{
        data_old[1]+=data_sym[1];
    }

    if(data_old[2] == 0xff || data[2] == 0){
        data_old[2]=data_sym[2];
    }
    else{
        data_old[2]+=data_sym[2];
    }

    if(data_old[3] == 0xff || data[3] == 0){
        data_old[3]=data_sym[3];
    }
    else{
        data_old[3]+=data_sym[3];
    }

    // pointer to addr_p
    a = fseek(newfile->file,*addr_p,SEEK_SET);
    // fwrite data_old at the place addr_p
    a = fwrite( data_old, sizeof(uint32_t) , 1, newfile->file);

    if (1 != a) {
        printf("\nfail to write\n");
    }
}