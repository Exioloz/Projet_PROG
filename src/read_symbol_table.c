#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <elf.h>

int main(int argc, char* argv[]) {
    if (argc < 2){
        printf("invalid arguments\n");
        exit(0);
    }

    FILE *fp;
    fp = fopen(argv[1], "r");
    if (NULL == fp){
        printf("fail to open the file");
        exit(0);
    }
    Elf32_Ehdr elf_head;
    int shnum, a;


    a = fread(&elf_head, sizeof(Elf32_Ehdr), 1, fp);
    if (0 == a){
        printf("fail to read head\n");
        exit(0);
    }

    if (elf_head.e_ident[0] != 0x7F ||
        elf_head.e_ident[1] != 'E' ||
        elf_head.e_ident[2] != 'L' ||
        elf_head.e_ident[3] != 'F'){
        printf("Not an ELF file\n");
        exit(0);
    }


    Elf32_Shdr *shdr = (Elf32_Shdr*)malloc(sizeof(Elf32_Shdr) * elf_head.e_shnum);
    if (NULL == shdr){
        printf("shdr malloc failed\n");
        exit(0);
    }


    a = fseek(fp, elf_head.e_shoff, SEEK_SET);
    if (0 != a){
        printf("\nfail to fseek\n");
        exit(0);
    }

    a = fread(shdr, sizeof(Elf32_Shdr) * elf_head.e_shnum, 1, fp); //?~Nfp?~_就?~X?section header table?~Z~D头?~@?~K?~@?~P~N读shdr * ?~U??~G~O?~Z~D?~F~E容
    if (0 == a){printf("\nfail to read section\n");
        exit(0);
    }


    rewind(fp);


    fseek(fp, shdr[elf_head.e_shstrndx].sh_offset, SEEK_SET);

    char shstrtab[shdr[elf_head.e_shstrndx].sh_size];
    char *temp = shstrtab;

    a = fread(shstrtab, shdr[elf_head.e_shstrndx].sh_size, 1, fp);
    if (0 == a){
        printf("\nfail to read\n");
    }
    int index_strtab;
    for (int i=0; i < elf_head.e_shnum; i++){
        temp=shstrtab;
        temp=temp+shdr[i].sh_name;
        if(strcmp(temp, ".strtab")!=0) continue;
        index_strtab=i;
    }
    rewind(fp);
    fseek(fp, shdr[index_strtab].sh_offset, SEEK_SET);
    char strtab[shdr[index_strtab].sh_size];
    char *strtemp=strtab;
    a = fread(strtab, shdr[index_strtab].sh_size, 1, fp);
    if (0 == a) {
        printf("\nfail to read\n");
    }


    for (int i = 0; i < elf_head.e_shnum; i++){
        temp = shstrtab;
        temp = temp + shdr[i].sh_name;
        if (strcmp(temp, ".symtab") != 0) continue;
        int number_sym = shdr[i].sh_size / shdr[i].sh_entsize;
        printf("La table de symboles << .symtab >> contient %d entrees :\n", number_sym);
        Elf32_Sym *symtable=(Elf32_Sym*)malloc(sizeof(Elf32_Sym) * number_sym);
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
            strtemp=strtab;
            strtemp=strtemp+symtable[j].st_name;
            printf(" %s\n", strtemp);
    }

}
    return 0;
}
