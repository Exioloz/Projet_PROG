#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "elf_header.h"
#include "readelf.h"
#include <elf.h>
#include "elf_shdrs.h"
#include <ctype.h>

void read_symbol_table(char filename, Filedata * filedata);

void read_symbol_table(char filename, Filedata * filedata) {

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (NULL == fp){
        printf("fail to open the file");
        exit(0);
    }


    // 第e_shstrndx项?~X??~W符串表 ?~Z?~I ?~W?~J~B ?~U?度 char类?~^~K ?~U??~D
    char shstrtab[filedata->section_headers[filedata->file_header.e_shstrndx].sh_size];
    char *temp = shstrtab;

    // 读?~O~V?~F~E容
    a = fread(shstrtab, shdr[elf_head.e_shstrndx].sh_size, 1, fp);
    if (0 == a){
        printf("\nfail to read\n");
    }
//    int index_strtab;
//    for (int i=0; i < filedata->file_header.e_shnum; i++){
//        temp=shstrtab;
//        temp=temp+filedata->section_headers[i].sh_name;
//        if(strcmp(temp, ".strtab")!=0) continue;
//        index_strtab=i;
//    }
//    rewind(fp);
//    fseek(fp, filedata->section_headers[index_strtab].sh_offset, SEEK_SET);
//    char strtab[filedata->section_headers[index_strtab].sh_size];
//    char *strtemp=strtab;
//    a = fread(strtab, filedata->section_headers[index_strtab].sh_size, 1, fp);
//    if (0 == a) {
//        printf("\nfail to read\n");
//    }
    char *strtemp=filedata->string_table;

    // ?~A~M?~N~F

    for (int i = 0; i < filedata->file_header.e_shnum; i++){
        temp = shstrtab;
        temp = temp + filedata->section_headers[i].sh_name;
        if (strcmp(temp, ".symtab") != 0) continue;
        int number_sym = filedata->section_headers[i].sh_size / filedata->section_headers[i].sh_entsize;
        printf("La table de symboles << .symtab >> contient %d entrees :\n", number_sym);
        Elf32_Sym *symtable=(Elf32_Sym*)malloc(sizeof(Elf32_Sym) * number_dy);
        fseek(fp, shdr[i].sh_offset, SEEK_SET); //?~I??~H?表?~Z~D?~M置
        printf("Size:%d\n",shdr[i].sh_size);
        fread(symtable, sizeof(Elf32_Sym)*number_sym, 1, fp);
        printf(" Num:\tValeur\t\t\tTail\tType\tLien\t\tVis\t\tNdx\tNom\n");
        for (int j=0; j<number_sym; j++) {
            temp=shstrtab;
            printf(" %d\t", j);
            printf("%016x\t", symtable[j].st_value);
            printf("%d\t", symtable[j].st_size);
            switch (ELF32_ST_TYPE(symtable[j].st_info)) {
                case 0:
                    printf("NOTYPE\t");
                    break;
                case 1:
                    printf("OBJECT\t");
                    break;
                case 2:
                    printf("FUNC\t");
                    break;
                case 3:
                    printf("SECTION\t");
                    break;
                case 4:
                    printf("FILE\t");
                    break;
                case 13:
                    printf("LOPROC\t");
                    break;
                case 15:
                    printf("HIPROC\t");
                    break;

                default:break;
            }
            switch(ELF32_ST_BIND(symtable[j].st_info))
            {
                case 0: printf("LOCAL\t\t");
                    break;
                case 1: printf("GLOBAL\t\t");
                    break;
                case 2: printf("WEAK\t\t");
                    break;
                case 3: printf("NUM\t\t");
                    break;
                case 13: printf("LOPROC\t\t");
                    break;
                case 15: printf("HIPROC\t\t");
                    break;

                default:
                    break;
            }
            switch(symtable[j].st_other){
                case 0: printf("DEFAULT\t\t");
                    break;
                case 2: printf("HIDDEN\t\t");
                    break;
                default:
                    printf("%hu\t", symtable[j].st_other);
                    break;
            }
            switch(symtable[j].st_shndx){
                case 0: printf("UND\t");
                    break;
                case 0xfff1: printf("ABS\t\t");
                    break;
                case 0xfff2: printf("COMMON\t\t");
                    break;
                default: printf("%hu\t", symtable[j].st_shndx);
                    break;

            }
            strtemp=filedata->string_table;;
            strtemp=strtemp+symtable[j].st_name;
            printf(" %s\n", strtemp);
        }

    }
    return 0;
}