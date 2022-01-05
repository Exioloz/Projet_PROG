#include "elf_reltab.h"

bool section_type_is_rel(Elf32_Word sec_type){
  return (sec_type==SHT_REL) ? true : false ;
}

uint16_t change_endian_16(uint16_t num){
  return (num >> 8) | (num << 8);
}

uint32_t change_endian_32(uint32_t num){
  return (num >> 24) | ((num >> 8) & 0x0000ff00) | ((num << 8) & 0x00ff0000) | (num << 24) ;
}

bool get_rel_table(Filedata *filedata){
  int i,j;
  //int test;
  int nb_relsec = 0;
  Elf32_Shdr* section = filedata->section_headers;
  for (i = 0 ; i < filedata->file_header.e_shnum; i++){
   if(section_type_is_rel(section[i].sh_type)){
     nb_relsec += 1;
   }
  }
  filedata->reloc_table.rel_num = nb_relsec;
  filedata->reloc_table.rel_tab = (Elf32_Ext_Rel *) malloc(sizeof(Elf32_Ext_Rel)*nb_relsec);
  if(filedata->reloc_table.rel_tab == NULL){
    printf("Out of memory for allocation.\n");
    exit(3);
  }
  int offset, ent_num;
  Elf32_Ext_Rel* tab ;

  for (i = 0 , tab = filedata->reloc_table.rel_tab ; i < filedata->file_header.e_shnum; i++){
    if(section_type_is_rel(section[i].sh_type)){
      printf("Found relocation section %s\n", get_section_name(filedata, section[i].sh_name)); 
      offset = section[i].sh_offset;
      ent_num = section[i].sh_size/8;
      tab->rel_ent_num = ent_num;
      tab->rel_sh_name = section[i].sh_name;
      tab->rel_sh_offset = offset;
      tab->rel_ents = (Elf32_Rel *) malloc(sizeof(Elf32_Rel)*ent_num);
      if(tab->rel_ents == NULL){
        printf("Out of memory for allocation.\n");
        exit(3);
      }
      for(j = 0 ; j < ent_num ; j++){
        fseek(filedata->file, offset+j*sizeof(Elf32_Rel), SEEK_SET);
        printf("offset = %ld\n", offset+j*sizeof(Elf32_Rel));
        fread(&(tab->rel_ents[j]), sizeof(tab->rel_ents[j]), 1, filedata->file);
       }
       tab++;
    }
  }
  return true;
}

bool process_rel_table(Filedata *filedata){
  int i,j;
  Elf32_Rel_Tab tab = filedata->reloc_table;
  int tab_ent = tab.rel_num;
  Elf32_Ext_Rel* ents;
  for (i = 0 , ents = tab.rel_tab ; i < tab_ent ; i++, ents++){
    if(ents->rel_ent_num == 1){
      printf("Relocation section '%s' at offset 0x%x contains %d entry:\n", get_section_name(filedata, ents->rel_sh_name), 
              ents->rel_sh_offset, ents->rel_ent_num ); 
    }
    else{
      printf("Relocation section '%s' at offset 0x%x contains %d entries:\n", get_section_name(filedata, ents->rel_sh_name), 
              ents->rel_sh_offset, ents->rel_ent_num ); 
    }
    printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
      for(j = 0 ; j < ents->rel_ent_num ; j++){
        printf("%8.8x  ", change_endian_32(ents->rel_ents[i].r_offset));
        printf("%8.8x\n", change_endian_32(ents->rel_ents[i].r_info));
       }
      printf("\n");
  }
  return true;
}