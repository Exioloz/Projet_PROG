#include "readelf.h"
#include "elf_header.h"
#include "elf_shdrs.h"
#include "elf_symbol_table.h"
#include "elf_reltab.h"
#include "read_section.h"

/*================================================================
    Global Variables for options
  ================================================================*/
int header = 0, sectHead = 0, section = 0, symtab = 0;
char * name;


/*================================================================
    Functions to obtain good data
  ================================================================*/
uint16_t change_endian_16(uint16_t num){
  return (num >> 8) | (num << 8);
}

uint32_t change_endian_32(uint32_t num){
  return (num >> 24) | ((num >> 8) & 0x0000ff00) | ((num << 8) & 0x00ff0000) | (num << 24) ;
}

// big_endian for chars - only for 32 bits 
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
Process File Header
Should print the same as option -h of readelf
*/
bool process_header(Filedata * filedata){
    if (!get_file_header(filedata)){
        fprintf(stderr, "%s: Failed to read file header\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!process_file_header(filedata)){
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Process Sections' Header
Should print the same as option -S of readelf
*/
bool process_sections_header(Filedata * filedata){
    if (!get_file_header(filedata)){
        fprintf(stderr, "%s: Failed to read file header\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!get_section_headers(filedata)){
        fprintf(stderr, "%s: Failed to read section headers\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!process_section_headers(filedata)){
        fprintf(stderr,"Failed to process section headers\n");
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Process Section Name
Should print the same as the option -x ".name of section" of readelf
*/
bool process_section_name(Filedata * filedata){
    if (!get_file_header(filedata)){
        fprintf(stderr, "%s: Failed to read file header\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!get_section_headers(filedata)){
        fprintf(stderr, "%s: Failed to read section headers\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!read_section(filedata, name)){
        free_filedata(filedata);
        return false;
    }
    return true; 
}

/*
Process Symbol Table
Should print the same as the option -s of readelf
*/
bool process_symtab(Filedata * filedata){
    if (!get_file_header(filedata)){
        fprintf(stderr, "%s: Failed to read file header\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!get_section_headers(filedata)){
        fprintf(stderr, "%s: Failed to read section headers\n", filedata->file_name);
        free_filedata(filedata);
        return false;
    }
    if (!process_symbol_table(filedata)){
        free_filedata(filedata);
        return false;
    }
    return true;
}

/*
Process File - verifies that we can open and read the file
*/
bool process_file(char *file_name){
    char magic[EI_NIDENT];
    Filedata * filedata = NULL;
    filedata = calloc(1, sizeof *filedata);
    if (filedata == NULL){
        fprintf(stderr, "Memory allocation failed for file data structure\n");
        return EXIT_FAILURE;
    }
    filedata->file_name = file_name;
    filedata->file = fopen(file_name, "rb");
    if (filedata->file == NULL){
        fprintf(stderr,"Input file %s is not readble.\n", file_name);
        free_filedata(filedata);
        return false; 
    }
    if (fread(magic, EI_NIDENT, 1, filedata->file) != 1){
        fprintf(stderr,"%s: Failed to read file's magic number\n", file_name);
        fclose (filedata->file);
        free_filedata(filedata);
        return false;
    }
    filedata->file_size = size_of_file(filedata->file);
    rewind(filedata->file);

    if (header){
        if (!process_header(filedata))
            return false;
    }
    if (sectHead){
        rewind(filedata->file);
        if (!process_sections_header(filedata))
            return false;
    }
    if (section){
        if (!process_section_name(filedata))
            return false;
    }
    if (symtab){
        rewind(filedata->file);
        if (!process_symtab(filedata))
            return false;
    }
    
    fclose(filedata->file);
    free_filedata(filedata);
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

// Free Filedata
void free_filedata(Filedata * filedata){
    free(filedata->section_headers);
    free(filedata->string_table);
    free(filedata);
}

/*================================================================
    Help Menu in case of user error
  ================================================================*/

void help(){
    printf("\nUsage: ./readelf <option(s)> elf-file\n");
    printf(" Display information about the contents of ELF format files\n Options are:\n");
    printf("  -a --all               Equivalent to: -h -S -s\n");
    printf("  -h --file-header       Display the ELF file header\n");
    printf("  -S --section-headers   Display the sections' header\n     --sections          An alias for --section-headers\n");
    printf("  -s --syms              Display the symbol table\n     --symbols           An alias for --syms\n");
    printf("  -x --hex-dump=<name>\n                         Dump the contents of section <name> as bytes\n");
    printf("  -r --relocs            Display the relocations (if present)\n");
}


/*================================================================
    MAIN FUNCTION
  ================================================================*/

// To be changed if we want to read multiple files at once
int main(int argc, char ** argv){
    for (int i=1; i < argc-1; i++){
        if(!strcmp(argv[i], "-a"))
            header = sectHead = symtab = 1;
        else if(!strcmp(argv[i], "-e"))
            header = sectHead = 1;
        else if(!strcmp(argv[i], "-h"))
            header = 1;
        else if(!strcmp(argv[i], "-S"))
            sectHead = 1;
        else if(!strcmp(argv[i], "-x")){
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
        else if(!strcmp(argv[i], "-s"))
            symtab = 1;
        else{
            fprintf(stderr,"%s is not a valid option\n", argv[i]);
            goto exit;
        }
    }
    printf("%s\n", argv[argc-1]);
    if (! process_file(argv[argc-1])){
        goto exit;
    }
    return EXIT_SUCCESS;
    exit:
        help();
        return EXIT_FAILURE;
}