#include "read_section.h"

// add Section '.data' has no data to dump. if size == 0
int read_section(Filedata * filedata, char * section_name) {

    rewind(filedata->file);

    fseek(filedata->file, filedata->section_headers[filedata->file_header.e_shstrndx].sh_offset, SEEK_SET);

    char shstrtab[filedata->section_headers[filedata->file_header.e_shstrndx].sh_size];
    char *temp = shstrtab;
    size_t a = fread(shstrtab, filedata->section_headers[filedata->file_header.e_shstrndx].sh_size, 1, filedata->file);
    if (0 == a){
        printf("\nfail to read\n");
    }

    for (int i = 0; i < filedata->file_header.e_shnum; i++){
        temp = shstrtab + filedata->section_headers[i].sh_name;
        if (strcmp(temp, section_name) != 0) continue;//该section?~P~M?
        printf("\n");
        printf("Vidange hexadecimale de la section <<  %s>> :\n", temp);
        printf("Offset: %x\n", filedata->section_headers[i].sh_offset);
        printf("La taille: %x\n", filedata->section_headers[i].sh_size);
        if (filedata->section_headers[i].sh_size == 0){
            printf("Section '%s' has no data to dump\n", temp);
            return false;
        }
        unsigned char  *sign_data=(unsigned char*)malloc(sizeof(unsigned char)*filedata->section_headers[i].sh_size);
        if (sign_data == NULL){
            fprintf(stderr, "Memory allocation for sign data failed\n");
            return false;
        }

        fseek(filedata->file, filedata->section_headers[i].sh_offset, SEEK_SET);


        if (fread(sign_data, sizeof(unsigned char)*(filedata->section_headers[i].sh_size), 1, filedata->file)){
            fprintf(stderr, "Unable to read file\n");
        }
        unsigned char *p = sign_data;
        int c = 0;
        int addr = filedata->section_headers[i].sh_addr;
        printf("0x%08x ", addr);
        for (int j=0; j<filedata->section_headers[i].sh_size; j++){
            printf("%02x", *p);
            p++;
            c++;
            if(c%4 == 0 && c < 16){
                printf(" ");
            }
            if(c == 16){
                printf(" ");
                p = p-c;
                for(int k=0; k<c; k++){
                    if(*p>33 &&  *p<127){
                        printf("%c",*p);
                    }else{
                        printf(".");
                    }
                    p++;
                }
                printf("\n");
                if (j<filedata->section_headers[i].sh_size-1){
                    addr=addr+16;
                    printf("0x%08x ", addr);
                }
                c=0;
            }

        }
        if(c<5){
            for(int y=0; y<2*(16-c)+4; y++){
                printf(" ");
            }
            p = p-c;
            for(int k=0; k<c; k++){
                if(*p>33 &&  *p<127){
                    printf("%c",*p);
                }else{
                    printf(".");
                }
                p++;
            }
            printf("\n");
            printf(" ");
            c=0;
        }else if(c<9){
            for(int y=0; y<2*(16-c)+2; y++){
                printf(" ");
            }
            p = p-c;
            for(int k=0; k<c; k++){
                if(*p>33 &&  *p<127){
                    printf("%c",*p);
                }else{
                    printf(".");
                }
                p++;
            }
            printf("\n");
            printf(" ");
            c=0;
        }else if(c<13){
            for(int y=0; y<2*(16-c)+1; y++){
                printf(" ");
            }
            p = p-c;
            for(int k=0; k<c; k++){
                if(*p>33 &&  *p<127){
                    printf("%c",*p);
                }else{
                    printf(".");
                }
                p++;
            }
            printf("\n");
            printf(" ");
            c=0;
        }else{
            for(int y=0; y<2*(16-c)+1; y++){
                printf(" ");
            }
            p = p-c;
            for(int k=0; k<c; k++){
                if(*p>33 &&  *p<127){
                    printf("%c",*p);
                }else{
                    printf(".");}
                p++;
            }
            printf("\n");
            printf(" ");
            c=0;
        }
        printf("\n");
    }
    return true;
}
