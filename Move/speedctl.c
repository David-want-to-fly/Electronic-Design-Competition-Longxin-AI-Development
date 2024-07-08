#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define pctlpin "51"
int gpioexport(char *argv)
{   int fd;
    int len;
    int ret;

    fd = open("/sys/class/gpio/export",O_WRONLY);
    if(fd<0)
    {
        printf("open error");
        return -1;
    }else
    {
        printf("opsuccess");
    }
    len = strlen(argv);
    ret = write(fd,argv,len);
    if(ret<0)
    {
        printf("write error");
        return -2;
    }else
    {
        printf("writesuccess");
    }
    close(fd);
    return 0;
}
int gpiospeed_set(char *id, int duty)
{   
    int fd;
    int len;
    int ret;
    char gpiopath[100];
    char dirpath[100];
    char valpath[100];
    sprintf(gpiopath,"/sys/class/gpio/gpio%s",id);
    sprintf(dirpath,"%s/direction",gpiopath);
    sprintf(valpath,"%s/value",gpiopath);
    fd = open(dirpath,O_WRONLY);
    if(fd<0)
    {
        printf("open direction error");
        return -1;
    }else
    {
        printf("op dir success");
    }
    len = strlen("out");
    ret = write(fd,"out",len);
    if(ret<0)
    {
        printf("write dir error");
        return -2;
    }else
    {
        printf("write dir success");
    }
    close(fd);
   

     fd = open(valpath,O_WRONLY);
    if(fd<0)
    {
        printf("open val error");
        return -1;
    }else
    {
        printf("op val success");
    }
    while(1)
    {
    len = strlen("0");
    ret = write(fd,"0",len);
    if(ret<0)
    {
        printf("write 0 error");
        return -2;
    }else
    {
        printf("write 0 success");
    }
    usleep(2000-duty);
    len = strlen("1");
    ret = write(fd,"1",len);
    if(ret<0)
    {
        printf("write 1 error");
        return -2;
    }else
    {
        printf("write 1 success");
    }
    usleep(duty);
    }
   close(fd);
}


void main()
{
    gpioexport(pctlpin);
    gpiospeed_set(pctlpin,600);
}