#include "elf_main.h"
#include "elf_header.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"
#include "elf_reltab.h"
#include "read_section.h"
#include <string.h>

//help message
void reloc_help_msg(){
  printf("\nUsage: ./readelf .text=<starting-address> .data=<starting-address> <elf-file>\n");
  printf(".text and .data have to be in order and both have to be there \n");
  printf("starting address is a 32-bit hexadecimal value with 0x prefix \n");
}

void get_text_addr(char *arg, Elf32_Addr *addr){
  char val[11];
  //printf("arg = %s\n", arg);
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

void get_data_addr(char *arg, Elf32_Addr *addr){
  char val[11];
  //printf("arg = %s\n", arg);
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

  //RELOCATION TO DO
  //i'm still thinking for this part haha
  //if you have any concrete ideas, please feel free to implement them here
  int i,j;

  //actual relocation here


  //after relocation
  //removing the relocation sections
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

  //newfile->file_header.e_shstrndx = newfile->file_header.e_shnum - 1;

  //allocate new section header table
  newfile->section_headers = malloc(sizeof(Elf32_Shdr) * new_sec_num);

  Elf32_Shdr* new_sec_hdr = newfile->section_headers;
  Elf32_Shdr* sec_hdr = filedata->section_headers;
  for(i=0, j=0 ; i < sec_num && j < new_sec_num ; i++){
    if(sec_hdr[i].sh_type != SHT_REL){
      memcpy(&(new_sec_hdr[j]), &(sec_hdr[i]), sizeof(Elf32_Shdr));
      if(strcmp(get_section_name(newfile, sec_hdr[i].sh_name),".text")==0){
        new_sec_hdr[j].sh_addr = text_addr;
      }
      if(strcmp(get_section_name(newfile, sec_hdr[i].sh_name),".data")==0){
        new_sec_hdr[j].sh_addr = data_addr;
      }
      j++;
    }
  }

  process_file_header(newfile);
  process_section_headers(newfile);

  free_filedata(filedata);
  
  return EXIT_SUCCESS;
}