#include "elf_main.h"
#include "elf_header.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"
#include "elf_reltab.h"
#include "read_section.h"
#include <string.h>

/*
Function: help message
  Prints a help message in case of error
*/
void reloc_help_msg(){
  printf("\nUsage: ./readelf .text=<starting-address> .data=<starting-address> <elf-file>\n");
  printf(".text and .data have to be in order and both have to be there \n");
  printf("starting address is a 32-bit hexadecimal value with 0x prefix \n");
}

/*
Function: Get text address
  
*/
void get_text_addr(char *arg, Elf32_Addr *addr){
  char val[11];
  if(arg[0] == '.' && arg[5] == '='){
    arg[5] = '\0'; //split argument into two parts
    if(strlen(&arg[6]) > 10){
      fprintf(stderr, "Address value has to be 32-bit (max 8 digit) hexadecimal number!\n");
      exit(1);
    }
    strcpy(val, &arg[6]);
    if(strcmp(arg,".text") == 0){
      *addr = (Elf32_Addr) strtol(val, NULL, 0);
    }
    else{
      fprintf(stderr, "Invalid section name\n");
      reloc_help_msg();
      exit(1);
    }
  }
  else{
    fprintf(stderr, "Invalid starting address argument syntax!\n");
    exit(1);
  }
}

/*
Function: Get Data Address

*/
void get_data_addr(char *arg, Elf32_Addr *addr){
  char val[11];
  if(arg[0] == '.' && arg[5] == '='){
    arg[5] = '\0'; //split argument into two parts
    if(strlen(&arg[6]) > 10){
      fprintf(stderr, "Address value has to be 32-bit (max 8 digit) hexadecimal number!\n");
      exit(1);
    }
    strcpy(val, &arg[6]);
    if(strcmp(arg,".data") == 0){
      *addr = (Elf32_Addr) strtol(val, NULL, 0);
    }
    else{
      fprintf(stderr, "Invalid section name\n");
      reloc_help_msg();
      exit(1);
    }
  }
  else{
    fprintf(stderr, "Invalid starting address argument syntax!\n");
    exit(1);
  }
}

//changes endianness of the bytes in the file header
void change_header_endian(Filedata *filedata){
    Elf32_Ehdr *head = &filedata->file_header;
    head->e_type            = change_endian_16(head->e_type);             
    head->e_machine         = change_endian_16(head->e_machine);       
    head->e_version         = change_endian_32(head->e_version);      
    head->e_entry           = change_endian_32(head->e_entry);           
    head->e_phoff           = change_endian_32(head->e_phoff);           
    head->e_shoff           = change_endian_32(head->e_shoff);           
    head->e_flags           = change_endian_32(head->e_flags);           
    head->e_ehsize          = change_endian_16(head->e_ehsize);        
    head->e_phentsize       = change_endian_16(head->e_phentsize);   
    head->e_phnum           = change_endian_16(head->e_phnum);           
    head->e_shentsize       = change_endian_16(head->e_shentsize);   
    head->e_shnum           = change_endian_16(head->e_shnum);           
    head->e_shstrndx        = change_endian_16(head->e_shstrndx);     
}

//changes endianness of bytes in the section headers
void change_section_endian(Filedata *filedata){
  int i;
  Elf32_Shdr* sec_hdr = filedata->section_headers;
  for(i=0 ; i < filedata->file_header.e_shnum ; i++){
    sec_hdr[i].sh_addralign = change_endian_32(sec_hdr[i].sh_addralign) ;
    sec_hdr[i].sh_entsize = change_endian_32(sec_hdr[i].sh_entsize) ;
    sec_hdr[i].sh_flags = change_endian_32(sec_hdr[i].sh_flags) ;
    sec_hdr[i].sh_info = change_endian_32(sec_hdr[i].sh_info) ;
    sec_hdr[i].sh_link = change_endian_32(sec_hdr[i].sh_link) ;
    sec_hdr[i].sh_size = change_endian_32(sec_hdr[i].sh_size) ;
    sec_hdr[i].sh_type = change_endian_32(sec_hdr[i].sh_type) ;
    sec_hdr[i].sh_name = change_endian_32(sec_hdr[i].sh_name) ;
    sec_hdr[i].sh_offset = change_endian_32(sec_hdr[i].sh_offset);
    sec_hdr[i].sh_addr = change_endian_32(sec_hdr[i].sh_addr) ;
  }
}

void renumerotation(Filedata *filedata, Filedata *newfile, Elf32_Addr text_addr, Elf32_Addr data_addr){
  int i,j;
  int sec_num = filedata->file_header.e_shnum ; //number of sections
  Elf32_Rel_Tab rel_tab = filedata->reloc_table; //relocation tables
  int rel_sec_num = rel_tab.rel_num; //number of relocation sections
  int new_sec_num = sec_num - rel_sec_num ; //new number of sections


  //copy file header
  memcpy(&(newfile->file_header), &(filedata->file_header), sizeof(Elf32_Ehdr));
  newfile->file_header.e_shnum = new_sec_num; //put new num of sections
  //string table index should be changed but not sure how
  //copy string table
  memcpy(&(newfile->string_table), &(filedata->string_table), sizeof(filedata->string_table));
  //copy string table length
  newfile->string_table_length = filedata->string_table_length;

  newfile->file_header.e_shstrndx = newfile->file_header.e_shnum - 1;
  
  //sec_off += newfile->file_header.e_ehsize ;

  //allocate new section header table
  newfile->section_headers = malloc(sizeof(Elf32_Shdr) * new_sec_num);

  //conversion table for symbol section index
  Elf32_Section new_stndx[sec_num];

  Elf32_Shdr* new_sec_hdr = newfile->section_headers;
  Elf32_Shdr* sec_hdr = filedata->section_headers;
  for(i=0, j=0 ; i < sec_num && j < new_sec_num ; i++){
    new_stndx[i] = 0 ;
    if(sec_hdr[i].sh_type != SHT_REL){
      new_stndx[i] = (Elf32_Section) j ;
      new_sec_hdr[j].sh_addralign = sec_hdr[i].sh_addralign ;
      new_sec_hdr[j].sh_entsize = sec_hdr[i].sh_entsize ;
      new_sec_hdr[j].sh_flags = sec_hdr[i].sh_flags ;
      new_sec_hdr[j].sh_info = sec_hdr[i].sh_info ;
      if(sec_hdr[i].sh_type == SHT_SYMTAB){
        new_sec_hdr[j].sh_link = j+1 ;
      }
      else{
        new_sec_hdr[j].sh_link = sec_hdr[i].sh_link ;
      }
      new_sec_hdr[j].sh_size = sec_hdr[i].sh_size ;
      new_sec_hdr[j].sh_type = sec_hdr[i].sh_type ;
      new_sec_hdr[j].sh_name = sec_hdr[i].sh_name ;

      if(strcmp(get_section_name(newfile, sec_hdr[i].sh_name),".text")==0){
        new_sec_hdr[j].sh_addr = text_addr;
      }
      else if(strcmp(get_section_name(newfile, sec_hdr[i].sh_name),".data")==0){
        new_sec_hdr[j].sh_addr = data_addr;
      }
      else{
        new_sec_hdr[j].sh_addr = sec_hdr[i].sh_addr;
      }

      if(sec_hdr[i].sh_type == SHT_NULL){
        new_sec_hdr[j].sh_offset = 0x0 ;
      }
      else if(strcmp(get_section_name(newfile, sec_hdr[i].sh_name),".text")==0){
        new_sec_hdr[j].sh_offset = text_addr + sec_hdr[i].sh_offset ;
      }
      else{
        new_sec_hdr[j].sh_offset = data_addr + sec_hdr[i].sh_offset ;
      }
      j++;
    }
  }

  //change start of section headers in file header
  newfile->file_header.e_shoff = new_sec_hdr[new_sec_num-1].sh_offset + new_sec_hdr[new_sec_num-1].sh_size ;
  //change file type to executable
  newfile->file_header.e_type = ET_EXEC ;

  //symbol index correction
  Elf32_Sym_Tab *old_sym_tab = &filedata->symbol_table; //old symbol table
  Elf32_Sym_Tab *new_sym_tab = &newfile->symbol_table; //new symbol table
  new_sym_tab->sym_tab_num = old_sym_tab->sym_tab_num ; //number of symbols
  new_sym_tab->sym_entries = malloc(sizeof(Elf32_Sym)*old_sym_tab->sym_tab_num); //allocate new symbol table
  memcpy(new_sym_tab->sym_entries, old_sym_tab->sym_entries, sizeof(Elf32_Sym)*old_sym_tab->sym_tab_num); //copy actual symbol table

  Elf32_Section ndx ;
  Elf32_Addr newval ;
  Elf32_Sym * sym_ents = new_sym_tab->sym_entries ;
  for(i=0 ; i < new_sym_tab->sym_tab_num ; i++){
    ndx = new_stndx[change_endian_16(sym_ents[i].st_shndx)] ;
    sym_ents[i].st_shndx = (Elf32_Section) change_endian_16(ndx);
    newval = new_sec_hdr[ndx].sh_addr + change_endian_32(sym_ents[i].st_value);
    sym_ents[i].st_value = change_endian_32(newval);
  }

}

/*
  GENERATE NEW BINARY FILE
  */
void write_file(Filedata *filedata, Filedata *newfile){
  
  //counters
  int i,j ;
  //other variables
  Elf32_Sym_Tab *new_sym_tab = &newfile->symbol_table; //new symbol table
  int sec_num = filedata->file_header.e_shnum ; //old number of sections
  int new_sec_num = newfile->file_header.e_shnum ; //new number of sections
  Elf32_Shdr* sec_hdr = filedata->section_headers; //old section headers
  Elf32_Shdr* new_sec_hdr = newfile->section_headers; //new section headers
  int sh_sec_off = newfile->file_header.e_shoff; //offset of shdr table
  

  //section content
  char *content_buffer = malloc(0x1000);
  if(content_buffer == NULL){
    fprintf(stderr, "Failed to allocate memory for buffer.\n");
    exit(1);
  }
  
  //change endianness
  change_header_endian(newfile);

  //copy header to new file
  if(fseek(newfile->file, 0, SEEK_SET) != 0) {
    fprintf(stderr, "Failed to seek header.\n");
    exit(1);
  }
  if(fwrite(&newfile->file_header, sizeof(newfile->file_header), 1, newfile->file) != 1){
    fprintf(stderr, "Failed to write header.\n");
    exit(1);
  }

  //re-change endianness
  change_header_endian(newfile);

  //copy sections to new file
  for(i=0, j=0 ; i < sec_num && j < new_sec_num ; i++){
    if(sec_hdr[i].sh_type == SHT_SYMTAB){
      if(fseek(newfile->file, new_sec_hdr[j].sh_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to seek section (write).\n");
        exit(1);
      }
      if(fwrite(new_sym_tab->sym_entries, sizeof(Elf32_Sym), new_sym_tab->sym_tab_num, newfile->file) != new_sym_tab->sym_tab_num){
        fprintf(stderr, "Failed to write section.\n");
        exit(1);
      }
      j++;
    }
    else if(sec_hdr[i].sh_type != SHT_REL && sec_hdr[i].sh_type != SHT_SYMTAB){
      rewind(filedata->file);
      if(fseek(filedata->file, sec_hdr[i].sh_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to seek section (read).\n");
        exit(1);
      }
      if(fread(content_buffer, 1, sec_hdr[i].sh_size, filedata->file) != sec_hdr[i].sh_size){
        fprintf(stderr, "Failed to read section.\n");
        exit(1);
      }

      if(fseek(newfile->file, new_sec_hdr[j].sh_offset, SEEK_SET) != 0) {
        fprintf(stderr, "Failed to seek section (write).\n");
        exit(1);
      }
      if(fwrite(content_buffer, 1, sec_hdr[i].sh_size, newfile->file) != sec_hdr[i].sh_size){
        fprintf(stderr, "Failed to write section.\n");
        exit(1);
      }
      j++;
    }
  }

  //change section header endianness
  change_section_endian(newfile);

  //copy section header table to new file
  if(fseek(newfile->file, sh_sec_off , SEEK_SET) != 0) {
    fprintf(stderr, "Failed to seek section (write).\n");
    exit(1);
  }
  if(fwrite(new_sec_hdr, newfile->file_header.e_shentsize, newfile->file_header.e_shnum, newfile->file) != newfile->file_header.e_shnum){
    fprintf(stderr, "Failed to write section.\n");
    exit(1);
  }

  //re-change section header endianness
  change_section_endian(newfile);

  free(content_buffer);
}


/*================================================================
    MAIN FUNCTION
  ================================================================*/

int main(int argc, char ** argv){

  //needs to have 3 arguments
  if(argc != 4){
    reloc_help_msg();
    return EXIT_FAILURE;
  };

  //this part is to handle the arguments
  
  char arg1[strlen(argv[1])+1]; //+1 to account for '\0'
  char arg2[strlen(argv[2])+1];
  strcpy(arg1, argv[1]);
  strcpy(arg2, argv[2]);

  Elf32_Addr text_addr, data_addr; //the starting addresses

  get_text_addr(arg1, &text_addr);
  get_data_addr(arg2, &data_addr);
  
  //original filedata allocation
  Filedata * filedata = NULL;
  filedata = calloc(1, sizeof *filedata);
  if (filedata == NULL){
      fprintf(stderr, "Memory allocation failed for file data structure\n");
      return EXIT_FAILURE;
  }
  //get original filedata
  if (! get_filedata(argv[argc-1], filedata)){
    return EXIT_FAILURE;
  }

  //new filedata allocation
  Filedata * newfile = NULL;
  newfile = calloc(1, sizeof *newfile);
  if (newfile == NULL){
      fprintf(stderr, "Memory allocation failed for new file data structure\n");
      return EXIT_FAILURE;
  }

  newfile->file = fopen("out.bin", "wb+");
  if (newfile->file == NULL){
      fprintf(stderr, "Failed to open output file.\n");
      return EXIT_FAILURE;
  }

  //RELOCATION TO DO
  //i'm still thinking for this part haha
  //if you have any concrete ideas, please feel free to implement them here

  //removing the relocation sections
  if(filedata->file_header.e_type == ET_REL){
    renumerotation(filedata, newfile, text_addr, data_addr);
    
    write_file(filedata, newfile);
    
    process_file_header(newfile);
    process_section_headers(newfile);
    process_symbol_table(newfile);
  }
  else{
    fprintf(stderr, "Input file is not a relocatable file.\n");
    exit(1);
  }

  free_filedata(filedata);

  return EXIT_SUCCESS;
}