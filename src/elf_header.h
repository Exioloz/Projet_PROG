#include "readelf.h"

/*
Free Filedata (to not have repetitive code)
*/
void free_filedata(Filedata * filedata);

/*
Get Elf Class
*/
const char * get_elf_class(unsigned int elf_class);

/*
Get Elf Data
*/
const char * get_elf_data(unsigned int elf_data);

/*
Get File Type
*/
char * get_file_type(unsigned e_type);

/*
Get Machine Name
*/
char * get_machine_name(unsigned e_machine);

/*
Get File Header

*/
bool get_file_header(Filedata *filedata);

/*
Process File Header
Input : filedata
Output : returns a bool to determine whether file header was processed properly
Decodes the data in the file header and prints as readelf
*/
bool process_file_header(Filedata * filedata);



