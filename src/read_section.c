#include <stdio.h>
#include <memory.h>
#include <stddef.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <elf.h>

int main(int argc, char* argv[]) {
    // 参数错误
    if (argc < 3){
        printf("invalid arguments\n");
        exit(0);
    }

    // 打开文件
    FILE *fp;
    fp = fopen(argv[1], "r");
    if (NULL == fp){
        printf("fail to open the file");
        exit(0);
    }
    // 解析head
    Elf64_Ehdr elf_head;
    int shnum, a;

    // 读取 head 到elf_head
    a = fread(&elf_head, sizeof(Elf64_Ehdr), 1, fp);   //fread参数1：读取内容存储地址，参数2：读取内容大小，参数3：读取次数，参数4：文件读取引擎
    if (0 == a){
        printf("fail to read head\n");
        exit(0);
    }

    // 判断elf文件类型
    if (elf_head.e_ident[0] != 0x7F ||
        elf_head.e_ident[1] != 'E' ||
        elf_head.e_ident[2] != 'L' ||
        elf_head.e_ident[3] != 'F'){
        printf("Not an ELF file\n");
        exit(0);
    }


    // 解析section 分配内存 section * 数量
    Elf64_Shdr *shdr = (Elf64_Shdr*)malloc(sizeof(Elf64_Shdr) * elf_head.e_shnum);
    if (NULL == shdr){
        printf("shdr malloc failed\n");
        exit(0);
    }

    // 设置fp偏移量 offset，e_shoff--This member holds the section header table's file offset in bytes. If the file has no
    //section header table, this member holds zero.
    a = fseek(fp, elf_head.e_shoff, SEEK_SET); //fseek调整指针的位置，采用参考位置+偏移量
    if (0 != a){
        printf("\nfail to fseek\n");
        exit(0);
    }

    // 读取section 到 shdr, 大小为shdr * 数量
    a = fread(shdr, sizeof(Elf64_Shdr) * elf_head.e_shnum, 1, fp); //从fp也就是section header table的头开始往后读shdr * 数量的内容
    if (0 == a){
        printf("\nfail to read section\n");
        exit(0);
    }

    // 重置指针位置到文件流开头
    rewind(fp);

    // 将fp指针移到 字符串表偏移位置处
    fseek(fp, shdr[elf_head.e_shstrndx].sh_offset, SEEK_SET);

    // 第e_shstrndx项是字符串表 定义 字节 长度 char类型 数组
    char shstrtab[shdr[elf_head.e_shstrndx].sh_size];
    char *temp = shstrtab;

    // 读取内容
    a = fread(shstrtab, shdr[elf_head.e_shstrndx].sh_size, 1, fp);
    if (0 == a){
        printf("\nfail to read\n");
    }

    // 遍历
    for (int i = 0; i < elf_head.e_shnum; i++){
        temp = shstrtab;
        temp = temp + shdr[i].sh_name;
        if (strcmp(temp, argv[2]) != 0) continue;//该section?~P~M?
        printf("\n");
        printf("Vidange hexadecimale de la section <<  %s>> :\n", temp);
        printf("Offset: %x\n", shdr[i].sh_offset);
        printf("La taille: %x\n", shdr[i].sh_size);
        char  *sign_data=(char*)malloc(sizeof(char)*shdr[i].sh_size);
        fseek(fp, shdr[i].sh_offset, SEEK_SET);
        fread(sign_data, sizeof(char)*shdr[i].sh_size, 1, fp);
        char *p = sign_data;
        int j = 0;
        int c = 0;
        int addr = shdr[i].sh_addr;
        printf("0x%08x",addr);
        printf(" ");
        for (j=0; j<shdr[i].sh_size; j++){
            printf("%02x", *p);
            p++;
            c++;
            if(c%4 == 0 && c != 16){
                printf(" ");
            }
            if(c == 16){
                printf(" ");
                p = p-c;
                for(int k=0; k<c; k++){if(*p!=0 && *p!=11 && *p!=18){
                        printf("%c",*p);
                    }else{
                        printf(".");
                    }
                    p++;
                }
                printf("\n");
                addr=addr+16;
                printf("0x%08x",addr);
                printf(" ");
                c=0;
            }

        }
        if(c!=0){
            for(int y=0; y<2*(16-c)+2; y++){
                printf(" ");
            }
            p = p-c;
            for(int k=0; k<c; k++){
                if(*p!=0 && *p!=11 && *p!=18){
                    printf("%c",*p);
                }else{
                    printf(".");
                }
                p++;
            }
            printf("\n");
            printf(" ");
            c=0;

        }
        printf("\n");
    }



    return 0;
}
