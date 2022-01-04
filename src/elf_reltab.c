#include "elf_reltab.h"

bool section_type_is_rel(Elf32_Word sec_type){
  return (sec_type==SHT_REL) ? true : false ;
}

bool get_rel_table(Filedata *filedata){
  int i;
  int nb_relsec = 0;
  Elf_Shdr* section = filedata->section_headers;
  for (i = 0 ; i < filedata->file_header.e_shnum; i++){
   if(section_type_is_rel(section[i].sh_type)){
     nb_relsec += 1;
   }
  }

  Elf32_Rel rel_ent[nb_relsec];

  for (i = 0 ; i < filedata->file_header.e_shnum; i++){
    if(section_type_is_rel(section[i].sh_type)){
      rel_ent[i].r_offset;
    }
  }
  return true;
}

bool process_rel_table(Filedata *filedata){
  int i;
  Elf_Shdr* section = filedata->section_headers;
  for (i = 0 ; i < filedata->file_header.e_shnum; i++){
    if(section_type_is_rel(section[i].sh_type)){
      printf("Relocation section '%s' at offset 0x%x contains %d entries:\n", get_section_name(filedata, &section[i]), 
              section[i].sh_offset, section[i].sh_size/8);
      
    }
  }
  return true;
}