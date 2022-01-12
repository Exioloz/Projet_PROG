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
           if(strcmp(sec_name, get_section_name(newfile, sec_hdr[j].sh_name))==0){
                calcul_val(ext_tab, &sec_hdr[j].sh_addr, sec_hdr[j].sh_offset, filedata, i, newfile);
            }
        }   
    }
}

void calcul_val(Elf32_Ext_Rel *  ext_tab, Elf32_Addr * addr,Elf32_Off offset_sec, Filedata * filedata, int index, Filedata * newfile){
    Elf32_Sym* symtable = filedata->symbol_table.sym_entries;
    uint32_t val_symbol=0;
    int idx=0;
    Elf32_Sym *new_sym_tab = newfile->symbol_table.sym_entries;

    printf("Offset:%d\n", offset_sec);
    for (int i = 0; i < ext_tab[index].rel_ent_num; i++){
        Elf32_Addr * addr_p= malloc(sizeof(Elf32_Addr));
        switch (ELF32_R_TYPE(change_endian_32(ext_tab[index].rel_ents[i].r_info))){
            case R_ARM_ABS32:
            case R_ARM_ABS16:
            case R_ARM_ABS8: //S
                idx = ELF32_R_SYM(change_endian_32(ext_tab[index].rel_ents[i].r_info));
                printf("nom:%s\n",get_section_name(filedata, filedata->section_headers[change_endian_16(symtable[idx].st_shndx)].sh_name));
                printf("idx:%d\n",idx);
                val_symbol=(uint32_t)(get_st_value(new_sym_tab, idx));
                *addr_p = *addr;
                *addr_p=offset_sec+change_endian_32(ext_tab[index].rel_ents[i].r_offset);
                printf("Ofset:%d\n",change_endian_32(ext_tab[index].rel_ents[i].r_offset));
                replace_data(filedata, val_symbol, addr_p, newfile);//replace the value starts at p by addr_symbol
                free(addr_p);
                break;
            case R_ARM_CALL:
            case R_ARM_JUMP24: //S - P
                idx = ELF32_R_SYM(change_endian_32(ext_tab[index].rel_ents[i].r_info));
                printf("nom:%s\n",get_section_name(filedata, filedata->section_headers[change_endian_16(symtable[idx].st_shndx)].sh_name));
                printf("idx:%d\n",idx);
                val_symbol=(uint32_t)change_endian_32(get_st_value(new_sym_tab, idx));
                val_symbol=(val_symbol-(*addr+change_endian_32(ext_tab[index].rel_ents[i].r_offset)));
                val_symbol=change_endian_32(((uint16_t)val_symbol & 0x03FFFFFE)>>2);

                *addr_p=offset_sec+change_endian_32(ext_tab[index].rel_ents[i].r_offset);
                printf("Ofset:%d\n",change_endian_32(ext_tab[index].rel_ents[i].r_offset));
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

    uint32_t val;

    if (newfile->file == NULL){
        fprintf(stderr,"Input file %s is not readble.\n", newfile->file_name);
        free_filedata(newfile);
    }
    //  rewind(newfile->file);
    a = fseek(newfile->file,*addr_p,SEEK_SET);
    fread(&val, sizeof(uint32_t),1,newfile->file);
    val_symbol=val_symbol+val;
    a = fseek(newfile->file,*addr_p,SEEK_SET);
    a = fwrite( &val_symbol, sizeof(uint32_t) , 1, newfile->file);
    if (1 != a) {
        printf("\nfail to write\n");
    }
}