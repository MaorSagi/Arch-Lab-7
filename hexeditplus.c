#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int unit_size;
char* filename;
char* data_pointer;

void set_file_name_handler(){
    char tmp[100];
    printf("Please enter filename:\n");
    fgets(tmp,100,stdin);
    while(tmp[0]=='\t'||tmp[0]==' '){
            strncpy(tmp,tmp+1,strlen(tmp));
    }
    if(filename==NULL)
        filename=malloc(sizeof(char)*strlen(tmp));
    else{
        free(filename);
        filename=malloc(sizeof(char)*strlen(tmp));
    }
    strncpy(filename,tmp,strlen(tmp)-1);
}
void set_unit_size_handler(){
    char tmp[100];
    printf("Please enter size (only 1,2,4):\n");
    fgets(tmp,100,stdin);
    while(tmp[0]=='\t'||tmp[0]==' '){
            strncpy(tmp,tmp+1,strlen(tmp));
    }
    if(tmp[0]!='1'&&tmp[0]!='2'&&tmp[0]!='4'){
        printf("Invalid input\n");
    }
    else{
        switch(tmp[0]){
            case '1':
                unit_size=1;
                break;
            case '2':
                unit_size=2;
                break;
            case '4':
                unit_size=4;
                break;
            
        }
    }
    
}


void print_hex(char* buf, int length){
     printf("%s\n","Hexadecimal Representation:");
    for(int i=0 ; i<unit_size*length;i=i+unit_size){
        switch(unit_size){
            case 1:
                printf("%02x ",(unsigned char)buf[i]);
                break;
            case 2:
                printf("%02x%02x ",(unsigned char)buf[i+1],(unsigned char)buf[i]);
                break;
            case 4:
                printf("%02x%02x%02x%02x ",(unsigned char)buf[i+3],(unsigned char)buf[i+2],(unsigned char)buf[i+1],(unsigned char)buf[i]);
                break;
        }
    }
    printf("\n");
}
void print_dec(char* buf, int length){
    printf("%s\n","Decimal Representation:"); 
    for(int i=0 ; i<length; i++){
        switch(unit_size){
            case 1:
                 printf("%hu ",*((unsigned char*)buf+i));
                break;
            case 2:
                printf("%hu ",*((unsigned short*)buf+i));
                break;
            case 4:
                printf("%u ",*((unsigned int*)buf+i));
                break;
        }
       
    }
    printf("\n");
}





void file_display_handler(){
    if(filename==NULL){
        printf("Error: enter filename first\n");
        return;
    }
    int location,length,fd;
    if((fd = open(filename,O_RDONLY,0))<0){
        printf("Error: open file\n");
        return;
    }
    char input[100];
    printf("Please enter <location> <length>:\n"); 
    fgets(input,100,stdin);
    sscanf(input,"%x %d",&location,&length);
    data_pointer = malloc(unit_size*length);
    lseek(fd,location,SEEK_SET);
    read(fd,data_pointer,unit_size*length);
    close(fd);
    print_hex(data_pointer,length);
    print_dec(data_pointer,length);
    free(data_pointer);
}
void file_modify_handler(){
     if(filename==NULL){
        printf("Error: enter filename first\n");
        return;
    }
    int location,val,fd;
    if((fd = open(filename,O_RDWR,0644))<0){
        printf("Error: open file\n");
        return;
    }
    char input[100];
    printf("Please enter <location> <val>:\n");
    fgets(input,100,stdin);
    sscanf(input,"%x %x",&location,&val);
    lseek(fd,location,SEEK_SET);
    write(fd,&val,unit_size*sizeof(val));
    close(fd);
    
}
void copy_from_file_handler(){
     if(filename==NULL){
        printf("Error: enter filename first\n");
        return;
    }

    int fdd,fds,src_offset,dst_offset,length;
    char src_file[100];
    if((fdd = open(filename,O_RDWR,0644))<0){
        printf("Error: open file\n");
        return;
    }
    char input[100];
    printf("Please enter <src_file> <src_offset> <dst_offset> <length>:\n");
    fgets(input,100,stdin);
    sscanf(input,"%s %x %x %d",src_file,&src_offset,&dst_offset,&length);
    if(src_file==NULL){
        printf("Error: enter src_file first\n");
        return;
    }
    if((fds = open(src_file,O_RDWR,0644))<0){
        printf("Error: open src file\n");
        return;
    }

   data_pointer = malloc(unit_size*length);
    lseek(fds,src_offset,SEEK_SET);
    read(fds,data_pointer,unit_size*length);
    lseek(fdd,dst_offset,SEEK_SET);
    write(fdd,data_pointer,unit_size*length);
    close(fdd);
    close(fds);
        
    printf("Copied %d bytes FROM %s at %02x TO %s at %02x \n",length,src_file,src_offset,filename,dst_offset);
    free(data_pointer);
}

int main(int argc, char **argv) {
    data_pointer=NULL;
    filename=NULL;
    unit_size=1;
     void (*func_ptr_arr[])()={set_file_name_handler,set_unit_size_handler,file_display_handler,file_modify_handler,copy_from_file_handler};
    char input[2048];
    printf("------------\nChoose action:\n1-Set File Name\n2-Set Unit Size\n3-File Display\n4-File Modify\n5-Copy From File\n6-Quit\n------------\n");
    fgets(input,2048,stdin);
    while(input[0]!='6'){
        if(input[0]=='\n')
            printf("Invalid input\n");
        if(input[0]=='\t'||input[0]==' '){
            strncpy(input,input+1,strlen(input));
            continue;
        }
        if(input[0]!='1'&&input[0]!='2'&&input[0]!='3'&&input[0]!='4'&&input[0]!='5'&&input[0]!='6')
            printf("Invalid input\n");
        else{
         
        (*func_ptr_arr[atoi(input)-1])();  
        }
        printf("------------\nChoose action:\n1-Set File Name\n2-Set Unit Size\n3-File Display\n4-File Modify\n5-Copy From File\n6-Quit\n------------\n");
        fgets(input,2048,stdin);
    }
    
    
	
  
    exit(0);
    return 0;
}