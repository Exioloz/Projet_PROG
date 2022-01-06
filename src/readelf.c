#include "readelf.h"
#include "elf_header.h"
#include "elf_shdrs.h"
#include "elf_symtab.h"
#include "elf_reltab.h"

uint16_t change_endian_16(uint16_t num){
  return (num >> 8) | (num << 8);
}

uint32_t change_endian_32(uint32_t num){
  return (num >> 24) | ((num >> 8) & 0x0000ff00) | ((num << 8) & 0x00ff0000) | (num << 24) ;
}

/*
Free Filedata (to not have repetitive code)
*/
void free_filedata(Filedata * filedata){
    free(filedata->program_headers);
    free(filedata->section_headers);
    free(filedata);
}

/*
big_endian - only for 32 bits
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

/*
Find Size of File
*/
long int size_of_file(FILE * filename){
    fseek(filename, 0L, SEEK_END);
    return ftell(filename);
}

/*
Process Object File - verifies that the file is in the correct 
format of an objects file
*/
static bool process_object_file(Filedata * filedata){
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

    if (!process_file_header(filedata)){
        free_filedata(filedata);
        return false;
    }
    if (!process_section_headers(filedata)){
        free_filedata(filedata);
        return false;
    }

    //testing
    //get_rel_table(filedata);
    //process_rel_table(filedata);

    return true;
}

/*
Process File - verifies that we can open and read the file
*/
static bool process_file(char *file_name){
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
    if (!process_object_file(filedata))
        return false;
    fclose(filedata->file);
    free_filedata(filedata);
    return true;
}

// To be changed if we want to read multiple files at once
int main(int argc, char ** argv){
    bool error = false;
    printf("%s\n", argv[1]);
    if (! process_file(argv[1]))
        error = true;
    return error ? EXIT_FAILURE : EXIT_SUCCESS;
}