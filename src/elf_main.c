#include "elf_main.h"
#include "elf_header.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"
#include "elf_reltab.h"
#include "read_section.h"

/*================================================================
    Global Variables for options
  ================================================================*/
int header = 0, sectHead = 0, section = 0, symtab = 0, reloc = 0;
char * name;


/*================================================================
    Functions to obtain good data
  ================================================================*/

/*
change_endian_16:
  changes a 16-bit integer from little-endian to big-endian (or vice-versa)
*/
uint16_t change_endian_16(uint16_t num){
  return (num >> 8) | (num << 8);
}

/*
change_endian_32:
  changes a 32-bit integer from little-endian to big-endian (or vice-versa)
*/
uint32_t change_endian_32(uint32_t num){
  return (num >> 24) | ((num >> 8) & 0x0000ff00) | ((num << 8) & 0x00ff0000) | (num << 24) ;
}

/*
Function: big_endian 
    Takes strings in 32 bits and performs the big endian operation 
*/
unsigned big_endian(unsigned char *field, int size){
    switch (size){
        case 1:
            return *field;
        case 2:
            return ((unsigned int)field[1] | ((int)field[0]) << 8);
        case 3:
            return ((uint32_t)field[2] | ((uint32_t)field[1]) << 8 | ((uint32_t)field[0]) << 16);
        case 4:
            return ((uint32_t)field[3] | ((uint32_t)field[2]) << 8 | ((uint32_t)field[1]) << 16 | ((uint32_t)field[0]) << 24);
        default:
            fprintf(stderr, "Unhandled data length %d\n", size);
            exit(EXIT_FAILURE);
    }
}

/*================================================================
    Functions to process file depending on options
  ================================================================*/

/*
Function: Process File Header
    Should print the same as option -h of readelf
*/
bool process_header(Filedata * filedata){
    if (!process_file_header(filedata)){
        fprintf(stderr,"Failed to File Header\n");
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Function: Process Sections' Header
    Should print the same as option -S of readelf
*/
bool process_sections_header(Filedata * filedata){
    if (!process_section_headers(filedata)){
        fprintf(stderr,"Failed to process section headers\n");
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Function: Process Section Name
    Should print the same as the option -x ".name of section" of readelf
*/
bool process_section_name(Filedata * filedata){
    if (!read_section(filedata, name)){
        fprintf(stderr,"Failed to process section %s\n", name);
        free_filedata(filedata);
        return false;
    }
    return true; 
}

/*
Function: Process Symbol Table
    Should print the same as the option -s of readelf
*/
bool process_symtab(Filedata * filedata){
    if (!process_symbol_table(filedata)){
        fprintf(stderr,"Failed to process symbol table\n");
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Function: Process Relocation
    Should print the same as the option -r of readelf
*/
bool process_reloc(Filedata * filedata){
    if (!process_rel_table(filedata)){
        fprintf(stderr,"Failed to process relocation\n");
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Function: Get filedata 
    Obtains and stores the values of the ELF file into
    the corresponding structure in filedata
*/
bool get_filedata(char *file_name, Filedata *filedata){
    char magic[EI_NIDENT];
    filedata->file_name = file_name;
    
    // Open and verify that file is readable
    filedata->file = fopen(file_name, "rb");
    if (filedata->file == NULL){
        fprintf(stderr,"Input file %s is not readble.\n", file_name);
        free_filedata(filedata);
        return false; 
    }

    // Read and store first 16 bytes of file 
    if (fread(magic, EI_NIDENT, 1, filedata->file) != 1){
        fprintf(stderr,"%s: Failed to read file's magic number\n", file_name);
        fclose (filedata->file);
        free_filedata(filedata);
        return false;
    }
    filedata->file_size = size_of_file(filedata->file);
    rewind(filedata->file);

    // Read and store file header 
    if (!get_file_header(filedata)){
        fprintf(stderr, "%s: Failed to read file header\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }

    // Read and store section headers
    if (!get_section_headers(filedata)){
        fprintf(stderr, "%s: Failed to read section headers\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }

    // Read and verify symbol table
    if (!get_symbol_table(filedata)){
        fprintf(stderr, "%s: Failed to read symbol table\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }

    // Read and store relocation table
    if (!get_rel_table(filedata)){
        fprintf(stderr, "%s: Failed to read relocation table\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Function: Process File 
    Prints the corresponding options
*/
bool process_file(Filedata * filedata){
    if (header){                                    // print file header
        if (!process_header(filedata))
            return false;
    }
    if (sectHead){                                  // print section headers
        rewind(filedata->file);
        if (!process_sections_header(filedata))
            return false;
    }
    if (section){                                   // print section
        if (!process_section_name(filedata))
            return false;
    }
    if (symtab){                                    // print symbol table
        rewind(filedata->file);
        if (!process_symtab(filedata))
            return false;
    }
    if (reloc){                                     // print relocation table
        rewind(filedata->file);
        if (!process_reloc(filedata))
            return false;
    }
    return true;
}

/*================================================================
    Functions for details of file
  ================================================================*/

// Find Size of File
long int size_of_file(FILE * filename){
    fseek(filename, 0L, SEEK_END);
    return ftell(filename);
}

/* 
Function: Free Filedata
    Used to free the memory allocated for filedata
    in order to prevent memory leak
*/
void free_filedata(Filedata * filedata){
    fclose(filedata->file);
    free(filedata->section_headers);
    free(filedata->string_table);
    free(filedata);
}

/*================================================================
    Help Menu in case of user error
  ================================================================*/

/*
Function: help
    Displays the options of ./readelf function in case 
    of misuse
*/
void help(){
    printf("\nUsage: ./readelf <option(s)> elf-file\n");
    printf("Options must be written seperately ie: -h -S\n");
    printf(" Display information about the contents of ELF format files\n Options are:\n");
    printf("  -a --all               Equivalent to: -h -S -s\n");
    printf("  -e --headers           Equivalent to: -h -S\n");
    printf("  -h --file-header       Display the ELF file header\n");
    printf("  -S --section-headers   Display the sections' header\n     --sections          An alias for --section-headers\n");
    printf("  -s --syms              Display the symbol table\n     --symbols           An alias for --syms\n");
    printf("  -x --hex-dump=<name>\n                         Dump the contents of section <name> as bytes\n");
    printf("  -r --relocs            Display the relocations (if present)\n");
}


/*================================================================
    MAIN FUNCTION
  ================================================================*/

/* 
Function: elf_main
    Called by readelf main to process the given ELF file
    depending on the option(s) given by the user.
    If no option is given, help is written in terminal. 
*/
int elf_main(int argc, char ** argv){
    // Verify that at least one option is given
    if (argc <= 2)
        goto exit;
    // Verify which options are given and if they are valid
    for (int i=1; i < argc-1; i++){
        if(!strcmp(argv[i], "-a"))                  // option all
            header = sectHead = symtab = reloc = 1;
        else if(!strcmp(argv[i], "-e"))             // option headers
            header = sectHead = 1;  
        else if(!strcmp(argv[i], "-h"))             // option file header
            header = 1; 
        else if(!strcmp(argv[i], "-S"))             // option section headers
            sectHead = 1;
        else if(!strcmp(argv[i], "-x")){            // option hex-dump
            i++;
            if (i == argc-1){
                fprintf(stderr, "No section name is given!\n");
                goto exit;
            }
            name = argv[i];
            if (strstr(name, ".") == 0){
                fprintf(stderr, "Section %s was not dumped because it does not exist!\n", name);
                goto exit;
            }
            section = 1;
        }
        else if(!strcmp(argv[i], "-s"))             // option  symbol table
            symtab = 1;
        else if(!strcmp(argv[i], "-r"))             // option relocation table
            reloc = 1;
        else{
            fprintf(stderr,"%s is not a valid option\n", argv[i]);
            goto exit;
        }
    }
    // Allocates memory for filedata
    Filedata * filedata = NULL;
    filedata = calloc(1, sizeof *filedata);
    if (filedata == NULL){
        fprintf(stderr, "Memory allocation failed for file data structure\n");
        return EXIT_FAILURE;
    }
    printf("%s\n", argv[argc-1]);
    // Verify that data in file is obtained without error
    if (! get_filedata(argv[argc-1], filedata)){
        goto exit;
    }
    // Verify that file data is processed without error
    if (! process_file(filedata)){
        goto exit;
    }
    free_filedata(filedata);
    return EXIT_SUCCESS;

    // Failed to produce proper output
    exit:
        help();
        return EXIT_FAILURE;
}