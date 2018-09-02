#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "pru_ioctl.h"
 
#define DATA_INSTRUCTION_LENGHT 8192

int main(int argc, char *argv[])
{
    char * file_name = "/dev/pru_ctrl";
    char * i_name    = "instructions.bin";
    char * d_name    = "data.bin";
    
    unsigned char ibuf[DATA_INSTRUCTION_LENGHT];
    unsigned char dbuf[DATA_INSTRUCTION_LENGHT];
    
    FILE * fd;
    FILE * fdbi;
    FILE * fdbd;
    
    int tmp = 0;
    
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("could not open device driver");
        return 2;
    }
    

    fdbi = fopen(i_name, "rb");
    if (fdbi == -1)
    {
        perror("could not open instructions binary");
        return 2;
    }
    
    fdbd = fopen(d_name, "rb");
    if (fdbd == -1)
    {
        perror("could not open data binary");
        return 2;
    }

    fread(ibuf,sizeof(ibuf),1,fdbi);
    fread(dbuf,sizeof(dbuf),1,fdbd);
 
    int q;

    printf("Resettign PRU..\n"); 

    ioctl(fd, PRU_RESET, &q);

    printf("Copying Instructions\n"); 

    ioctl(fd, PRU_COPY_INSTRUCTIONS, &ibuf);
    
    printf("Copying Data\n"); 

    ioctl(fd, PRU_COPY_DATA, &dbuf);
    
    printf("Enabling PRU0 UART pins and clocks\n"); 

    ioctl(fd, PRU_UART_CONFIGURE_PINS, &q);

    close(fd);
    fclose(fdbi);
    fclose(fdbd);
    
    return 0;
}
