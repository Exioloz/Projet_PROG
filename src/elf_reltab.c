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

char* get_reloc_type(Elf32_Word rel_info){
  switch(rel_info){
    case R_ARM_NONE: return("R_ARM_NONE"); break;   /* No reloc */
    case R_ARM_PC24: return("R_ARM_PC24"); break;   /* Deprecated PC relative 26
                bit branch.  */
    case R_ARM_ABS32: return("R_ARM_ABS32"); break;   /* Direct 32 bit  */
    case R_ARM_REL32: return("R_ARM_REL32"); break;   /* PC relative 32 bit */
    case R_ARM_PC13: return("R_ARM_PC13"); break;
    case R_ARM_ABS16: return("R_ARM_ABS16"); break;   /* Direct 16 bit */
    case R_ARM_ABS12: return("R_ARM_ABS12"); break;   /* Direct 12 bit */
    case R_ARM_THM_ABS5: return("R_ARM_THM_ABS5"); break;   /* Direct & 0x7C (LDR, STR).  */
    case R_ARM_ABS8: return("R_ARM_ABS8"); break;   /* Direct 8 bit */
    case R_ARM_SBREL32: return("R_ARM_SBREL32"); break;
    case R_ARM_THM_PC22: return("R_ARM_THM_PC22"); break;   /* PC relative 24 bit (Thumb32 BL).  */
    case R_ARM_THM_PC8: return("R_ARM_THM_PC8"); break;   /* PC relative & 0x3FC
                (Thumb16 LDR, ADD, ADR).  */
    case R_ARM_AMP_VCALL9: return("R_ARM_AMP_VCALL9"); break;
    case R_ARM_TLS_DESC: return("R_ARM_TLS_DESC"); break;   /* Dynamic relocation.  */
    case R_ARM_THM_SWI8: return("R_ARM_THM_SWI8"); break;   /* Reserved.  */
    case R_ARM_XPC25: return("R_ARM_XPC25"); break;   /* Reserved.  */
    case R_ARM_THM_XPC22: return("R_ARM_THM_XPC22"); break;   /* Reserved.  */
    case R_ARM_TLS_DTPMOD32: return("R_ARM_TLS_DTPMOD32"); break;   /* ID of module containing symbol */
    case R_ARM_TLS_DTPOFF32: return("R_ARM_TLS_DTPOFF32"); break;   /* Offset in TLS block */
    case R_ARM_TLS_TPOFF32: return("R_ARM_TLS_TPOFF32"); break;   /* Offset in static TLS block */
    case R_ARM_COPY: return("R_ARM_COPY"); break;   /* Copy symbol at runtime */
    case R_ARM_GLOB_DAT: return("R_ARM_GLOB_DAT"); break;   /* Create GOT entry */
    case R_ARM_JUMP_SLOT: return("R_ARM_JUMP_SLOT"); break;   /* Create PLT entry */
    case R_ARM_RELATIVE: return("R_ARM_RELATIVE"); break;   /* Adjust by program base */
    case R_ARM_GOTOFF: return("R_ARM_GOTOFF"); break;   /* 32 bit offset to GOT */
    case R_ARM_GOTPC: return("R_ARM_GOTPC"); break;   /* 32 bit PC relative offset to GOT */
    case R_ARM_GOT32: return("R_ARM_GOT32"); break;   /* 32 bit GOT entry */
    case R_ARM_PLT32: return("R_ARM_PLT32"); break;   /* Deprecated, 32 bit PLT address.  */
    case R_ARM_CALL: return("R_ARM_CALL"); break;   /* PC relative 24 bit (BL, BLX).  */
    case R_ARM_JUMP24: return("R_ARM_JUMP24"); break;   /* PC relative 24 bit
                (B, BL<cond>).  */
    case R_ARM_THM_JUMP24: return("R_ARM_THM_JUMP24"); break;   /* PC relative 24 bit (Thumb32 B.W).  */
    case R_ARM_BASE_ABS: return("R_ARM_BASE_ABS"); break;   /* Adjust by program base.  */
    case R_ARM_ALU_PCREL_7_0: return("R_ARM_ALU_PCREL_7_0"); break;   /* Obsolete.  */
    case R_ARM_ALU_PCREL_15_8: return("R_ARM_ALU_PCREL_15_8"); break;   /* Obsolete.  */
    case R_ARM_ALU_PCREL_23_15: return("R_ARM_ALU_PCREL_23_15"); break;   /* Obsolete.  */
    case R_ARM_LDR_SBREL_11_0: return("R_ARM_LDR_SBREL_11_0"); break;   /* Deprecated, prog. base relative.  */
    case R_ARM_ALU_SBREL_19_12: return("R_ARM_ALU_SBREL_19_12"); break;   /* Deprecated, prog. base relative.  */
    case R_ARM_ALU_SBREL_27_20: return("R_ARM_ALU_SBREL_27_20"); break;   /* Deprecated, prog. base relative.  */
    case R_ARM_TARGET1: return("R_ARM_TARGET1"); break;
    case R_ARM_SBREL31: return("R_ARM_SBREL31"); break;   /* Program base relative.  */
    case R_ARM_V4BX: return("R_ARM_V4BX"); break;
    case R_ARM_TARGET2: return("R_ARM_TARGET2"); break;
    case R_ARM_PREL31: return("R_ARM_PREL31"); break;   /* 32 bit PC relative.  */
    case R_ARM_MOVW_ABS_NC: return("R_ARM_MOVW_ABS_NC"); break;   /* Direct 16-bit (MOVW).  */
    case R_ARM_MOVT_ABS: return("R_ARM_MOVT_ABS"); break;   /* Direct high 16-bit (MOVT).  */
    case R_ARM_MOVW_PREL_NC: return("R_ARM_MOVW_PREL_NC"); break;   /* PC relative 16-bit (MOVW).  */
    case R_ARM_MOVT_PREL: return("R_ARM_MOVT_PREL"); break;   /* PC relative (MOVT).  */
    case R_ARM_THM_MOVW_ABS_NC: return("R_ARM_THM_MOVW_ABS_NC"); break;   /* Direct 16 bit (Thumb32 MOVW).  */
    case R_ARM_THM_MOVT_ABS: return("R_ARM_THM_MOVT_ABS"); break;   /* Direct high 16 bit
                (Thumb32 MOVT).  */
    case R_ARM_THM_MOVW_PREL_NC: return("R_ARM_THM_MOVW_PREL_NC"); break;   /* PC relative 16 bit
                (Thumb32 MOVW).  */
    case R_ARM_THM_MOVT_PREL: return("R_ARM_THM_MOVT_PREL"); break;   /* PC relative high 16 bit
                (Thumb32 MOVT).  */
    case R_ARM_THM_JUMP19: return("R_ARM_THM_JUMP19"); break;   /* PC relative 20 bit
                (Thumb32 B<cond>.W).  */
    case R_ARM_THM_JUMP6: return("R_ARM_THM_JUMP6"); break;   /* PC relative X & 0x7E
                (Thumb16 CBZ, CBNZ).  */
    case R_ARM_THM_ALU_PREL_11_0: return("R_ARM_THM_ALU_PREL_11_0"); break;   /* PC relative 12 bit
                (Thumb32 ADR.W).  */
    case R_ARM_THM_PC12: return("R_ARM_THM_PC12"); break;   /* PC relative 12 bit
                (Thumb32 LDR{D,SB,H,SH}).  */
    case R_ARM_ABS32_NOI: return("R_ARM_ABS32_NOI"); break;   /* Direct 32-bit.  */
    case R_ARM_REL32_NOI: return("R_ARM_REL32_NOI"); break;   /* PC relative 32-bit.  */
    case R_ARM_ALU_PC_G0_NC: return("R_ARM_ALU_PC_G0_NC"); break;   /* PC relative (ADD, SUB).  */
    case R_ARM_ALU_PC_G0: return("R_ARM_ALU_PC_G0"); break;   /* PC relative (ADD, SUB).  */
    case R_ARM_ALU_PC_G1_NC: return("R_ARM_ALU_PC_G1_NC"); break;   /* PC relative (ADD, SUB).  */
    case R_ARM_ALU_PC_G1: return("R_ARM_ALU_PC_G1"); break;   /* PC relative (ADD, SUB).  */
    case R_ARM_ALU_PC_G2: return("R_ARM_ALU_PC_G2"); break;   /* PC relative (ADD, SUB).  */
    case R_ARM_LDR_PC_G1: return("R_ARM_LDR_PC_G1"); break;   /* PC relative (LDR,STR,LDRB,STRB).  */
    case R_ARM_LDR_PC_G2: return("R_ARM_LDR_PC_G2"); break;   /* PC relative (LDR,STR,LDRB,STRB).  */
    case R_ARM_LDRS_PC_G0: return("R_ARM_LDRS_PC_G0"); break;   /* PC relative (STR{D,H},
                LDR{D,SB,H,SH}).  */
    case R_ARM_LDRS_PC_G1: return("R_ARM_LDRS_PC_G1"); break;   /* PC relative (STR{D,H},
                LDR{D,SB,H,SH}).  */
    case R_ARM_LDRS_PC_G2: return("R_ARM_LDRS_PC_G2"); break;   /* PC relative (STR{D,H},
                LDR{D,SB,H,SH}).  */
    case R_ARM_LDC_PC_G0: return("R_ARM_LDC_PC_G0"); break;   /* PC relative (LDC, STC).  */
    case R_ARM_LDC_PC_G1: return("R_ARM_LDC_PC_G1"); break;   /* PC relative (LDC, STC).  */
    case R_ARM_LDC_PC_G2: return("R_ARM_LDC_PC_G2"); break;   /* PC relative (LDC, STC).  */
    case R_ARM_ALU_SB_G0_NC: return("R_ARM_ALU_SB_G0_NC"); break;   /* Program base relative (ADD,SUB).  */
    case R_ARM_ALU_SB_G0: return("R_ARM_ALU_SB_G0"); break;   /* Program base relative (ADD,SUB).  */
    case R_ARM_ALU_SB_G1_NC: return("R_ARM_ALU_SB_G1_NC"); break;   /* Program base relative (ADD,SUB).  */
    case R_ARM_ALU_SB_G1: return("R_ARM_ALU_SB_G1"); break;   /* Program base relative (ADD,SUB).  */
    case R_ARM_ALU_SB_G2: return("R_ARM_ALU_SB_G2"); break;   /* Program base relative (ADD,SUB).  */
    case R_ARM_LDR_SB_G0: return("R_ARM_LDR_SB_G0"); break;   /* Program base relative (LDR,
                STR, LDRB, STRB).  */
    case R_ARM_LDR_SB_G1: return("R_ARM_LDR_SB_G1"); break;   /* Program base relative
                (LDR, STR, LDRB, STRB).  */
    case R_ARM_LDR_SB_G2: return("R_ARM_LDR_SB_G2"); break;   /* Program base relative
                (LDR, STR, LDRB, STRB).  */
    case R_ARM_LDRS_SB_G0: return("R_ARM_LDRS_SB_G0"); break;   /* Program base relative
                (LDR, STR, LDRB, STRB).  */
    case R_ARM_LDRS_SB_G1: return("R_ARM_LDRS_SB_G1"); break;   /* Program base relative
                (LDR, STR, LDRB, STRB).  */
    case R_ARM_LDRS_SB_G2: return("R_ARM_LDRS_SB_G2"); break;   /* Program base relative
                (LDR, STR, LDRB, STRB).  */
    case R_ARM_LDC_SB_G0: return("R_ARM_LDC_SB_G0"); break;   /* Program base relative (LDC,STC).  */
    case R_ARM_LDC_SB_G1: return("R_ARM_LDC_SB_G1"); break;   /* Program base relative (LDC,STC).  */
    case R_ARM_LDC_SB_G2: return("R_ARM_LDC_SB_G2"); break;   /* Program base relative (LDC,STC).  */
    case R_ARM_MOVW_BREL_NC: return("R_ARM_MOVW_BREL_NC"); break;   /* Program base relative 16
                bit (MOVW).  */
    case R_ARM_MOVT_BREL: return("R_ARM_MOVT_BREL"); break;   /* Program base relative high
                16 bit (MOVT).  */
    case R_ARM_MOVW_BREL: return("R_ARM_MOVW_BREL"); break;   /* Program base relative 16
                bit (MOVW).  */
    case R_ARM_THM_MOVW_BREL_NC: return("R_ARM_THM_MOVW_BREL_NC"); break;   /* Program base relative 16
                bit (Thumb32 MOVW).  */
    case R_ARM_THM_MOVT_BREL: return("R_ARM_THM_MOVT_BREL"); break;   /* Program base relative high
                16 bit (Thumb32 MOVT).  */
    case R_ARM_THM_MOVW_BREL: return("R_ARM_THM_MOVW_BREL"); break;   /* Program base relative 16
                bit (Thumb32 MOVW).  */
    case R_ARM_TLS_GOTDESC: return("R_ARM_TLS_GOTDESC"); break;
    case R_ARM_TLS_CALL: return("R_ARM_TLS_CALL"); break;
    case R_ARM_TLS_DESCSEQ: return("R_ARM_TLS_DESCSEQ"); break;   /* TLS relaxation.  */
    case R_ARM_THM_TLS_CALL: return("R_ARM_THM_TLS_CALL"); break;
    case R_ARM_PLT32_ABS: return("R_ARM_PLT32_ABS"); break;
    case R_ARM_GOT_ABS: return("R_ARM_GOT_ABS"); break;   /* GOT entry.  */
    case R_ARM_GOT_PREL: return("R_ARM_GOT_PREL"); break;   /* PC relative GOT entry.  */
    case R_ARM_GOT_BREL12: return("R_ARM_GOT_BREL12"); break;   /* GOT entry relative to GOT
                origin (LDR).  */
    case R_ARM_GOTOFF12: return("R_ARM_GOTOFF12"); break;   /* 12 bit, GOT entry relative
                to GOT origin (LDR, STR).  */
    case R_ARM_GOTRELAX: return("R_ARM_GOTRELAX"); break;
    case R_ARM_GNU_VTENTRY: return("R_ARM_GNU_VTENTRY"); break;
    case R_ARM_GNU_VTINHERIT: return("R_ARM_GNU_VTINHERIT"); break;
    case R_ARM_THM_PC11: return("R_ARM_THM_PC11"); break;   /* PC relative & 0xFFE (Thumb16 B).  */
    case R_ARM_THM_PC9: return("R_ARM_THM_PC9"); break;   /* PC relative & 0x1FE
                (Thumb16 B/B<cond>).  */
    case R_ARM_TLS_GD32: return("R_ARM_TLS_GD32"); break;   /* PC-rel 32 bit for global dynamic
                thread local data */
    case R_ARM_TLS_LDM32: return("R_ARM_TLS_LDM32"); break;   /* PC-rel 32 bit for local dynamic
                thread local data */
    case R_ARM_TLS_LDO32: return("R_ARM_TLS_LDO32"); break;   /* 32 bit offset relative to TLS
                block */
    case R_ARM_TLS_IE32: return("R_ARM_TLS_IE32"); break;   /* PC-rel 32 bit for GOT entry of
                static TLS block offset */
    case R_ARM_TLS_LE32: return("R_ARM_TLS_LE32"); break;   /* 32 bit offset relative to static
                TLS block */
    case R_ARM_TLS_LDO12: return("R_ARM_TLS_LDO12"); break;   /* 12 bit relative to TLS
                block (LDR, STR).  */
    case R_ARM_TLS_LE12: return("R_ARM_TLS_LE12"); break;   /* 12 bit relative to static
                TLS block (LDR, STR).  */
    case R_ARM_TLS_IE12GP: return("R_ARM_TLS_IE12GP"); break;   /* 12 bit GOT entry relative
                to GOT origin (LDR).  */
    case R_ARM_ME_TOO: return("R_ARM_ME_TOO"); break;   /* Obsolete.  */
    case R_ARM_THM_TLS_DESCSEQ16: return("R_ARM_THM_TLS_DESCSEQ16"); break;
    case R_ARM_THM_TLS_DESCSEQ32: return("R_ARM_THM_TLS_DESCSEQ32"); break;
    case R_ARM_THM_GOT_BREL12: return("R_ARM_THM_GOT_BREL12"); break;   /* GOT entry relative to GOT
                origin, 12 bit (Thumb32 LDR).  */
    case R_ARM_IRELATIVE: return("R_ARM_IRELATIVE"); break;
    case R_ARM_RXPC25: return("R_ARM_RXPC25"); break;
    case R_ARM_RSBREL32: return("R_ARM_RSBREL32"); break;
    case R_ARM_THM_RPC22: return("R_ARM_THM_RPC22"); break;
    case R_ARM_RREL32: return("R_ARM_RREL32"); break;
    case R_ARM_RABS22: return("R_ARM_RABS22"); break;
    case R_ARM_RPC24: return("R_ARM_RPC24"); break;
    case R_ARM_RBASE: return("R_ARM_RBASE"); break;
    /* Keep this the last entry.  */
    case R_ARM_NUM: return("R_ARM_NUM"); break;
    default:
      return ("<unknown>");
  }
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
      //printf("Found relocation section %s\n", get_section_name(filedata, section[i].sh_name)); 
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
        //printf("offset = %lx\n", offset+j*sizeof(Elf32_Rel));
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
        printf("%8.8x  ", change_endian_32(ents->rel_ents[j].r_offset));
        printf("%8.8x  ", change_endian_32(ents->rel_ents[j].r_info));
        printf("%s     ", get_reloc_type(ELF32_R_TYPE(change_endian_32(ents->rel_ents[j].r_info))));
        //printf("%8.8x  ", ELF32_R_SYM(change_endian_32(ents->rel_ents[j].r_info)));
        printf("\n");
       }
      printf("\n");
  }
  return true;
}