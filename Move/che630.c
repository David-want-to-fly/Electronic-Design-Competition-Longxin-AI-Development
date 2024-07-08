#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define testpin "38"


#define motorR1 45
#define mot0rR2 46
#define motorL1 47
#define motorL2 48

#define motorR1p "48"
#define motorR2p "47"
#define motorL1p "45"
#define motorL2p "46"
#define motorR1p_i "52"
#define motorR2p_i "53"

#define sensor1 37
#define sensor2 38
#define sensor3 40
#define sensor4 41

#define sensor1p "37"
#define sensor2p "38"
#define sensor3p "40"
#define sensor4p "41"

#define armctl0 49
#define armctl1 50

#define armctl0p "49"
#define armctl1p "50"


int Lcounter;
int Lcounter_reg;
int Rcounter;
int Rcounter_reg;
int stop_reg;



int readgpio(char *port);

int gpiovalue_set(char *id, char *value)
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
       // printf("op dir success");
    }
    len = strlen("out");
    ret = write(fd,"out",len);
    if(ret<0)
    {
        printf("write dir error");
        return -2;
    }else
    {
      //  printf("write dir success");
    }
    close(fd);
   

     fd = open(valpath,O_WRONLY);
    if(fd<0)
    {
        printf("open val error");
        return -1;
    }else
    {
       // printf("op val success");
    }
    len = strlen(value);
    ret = write(fd,value,len);
    if(ret<0)
    {
        printf("write val error");
        return -2;
    }else
    {
       // printf("write val success");
    }
    close(fd);
    return 0;
}

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
       // printf("opsuccess");
    }
    len = strlen(argv);
    ret = write(fd,argv,len);
    if(ret<0)
    {
        printf("write error");
        return -2;
    }else
    {
       // printf("writesuccess");
    }
    close(fd);
    return 0;
}

int gpiounexport(char *argv)
{
    int fd;
    int len;
    int ret;
    fd = open("/sys/class/gpio/unexport",O_WRONLY);
    if(fd<0)
    {
        printf("open error");
        return -1;
    }else
    {
       // printf("opsuccess");
    }
    len = strlen(argv);
    ret = write(fd,argv,len);
    if(ret<0)
    {
        printf("write error");
        return -2;
    }else
    {
      //  printf("writesuccess");
    }
    close(fd);
    return 0;
}


int initialize()
{
    gpioexport(motorR1p);
    gpioexport(motorR2p);
    gpioexport(motorL1p);
    gpioexport(motorL2p);
    gpioexport(motorR1p_i);
    gpioexport(motorR2p_i);

    gpioexport(sensor1p);
    gpioexport(sensor2p);
    gpioexport(sensor3p);
    gpioexport(sensor4p);
    gpioexport(armctl0p);
    gpioexport(armctl1p);
    gpiovalue_set(armctl0p,"1");
    gpiovalue_set(armctl1p,"1");

    Lcounter=0;
    Lcounter_reg=0;
    Rcounter=0;
    Rcounter_reg=0;
    stop_reg=0;

    return 0;
}

void forward()
{
   
    gpiovalue_set(motorR1p,"1");
    gpiovalue_set(motorR2p,"1");
    gpiovalue_set(motorL1p,"1");
    gpiovalue_set(motorL2p,"1");
    gpiovalue_set(motorR1p_i,"0");
    gpiovalue_set(motorR2p_i,"0");
    
}

void right()
{
    gpiovalue_set(motorR1p,"0");
    gpiovalue_set(motorR2p,"0");
    gpiovalue_set(motorL1p,"1");
    gpiovalue_set(motorL2p,"1");
    gpiovalue_set(motorR1p_i,"1");
    gpiovalue_set(motorR2p_i,"1");

}

void left()
{

    gpiovalue_set(motorR1p,"1");
    gpiovalue_set(motorR2p,"1");
    gpiovalue_set(motorL1p,"0");
    gpiovalue_set(motorL2p,"0");
    gpiovalue_set(motorR1p_i,"0");
    gpiovalue_set(motorR2p_i,"0");

}

void nomove()
{
    gpiovalue_set(motorR1p,"0");
    gpiovalue_set(motorR2p,"0");
    gpiovalue_set(motorL1p,"0");
    gpiovalue_set(motorL2p,"0");
    gpiovalue_set(motorR1p_i,"0");
    gpiovalue_set(motorR2p_i,"0");
}




int keepturning()
{
    while(readgpio(sensor2p)==0||readgpio(sensor3p)==0)
    {
        usleep(1000);
    }
    return 0;
}

void tracking()
{
    if(readgpio(sensor2p)==1&&readgpio(sensor3p)==1)
    {
        forward();
    }
    if(readgpio(sensor2p)==1&&readgpio(sensor3p)==0)
    {
        left();
    }
    if(readgpio(sensor2p)==0&&readgpio(sensor3p)==1)
    {
        right();
    }
    if(readgpio(sensor2p)==0&&readgpio(sensor3p)==0)
    {
        left();
    }
    usleep(1000);

}

void counter_L()
{   
    int val_now=readgpio(sensor1p);
    if(val_now==1&&Lcounter_reg==0)
    {
        Lcounter++;
	printf("Lcounter increase, now it is %d\n",Lcounter);
    }
    Lcounter_reg=val_now;
}

void counter_R()
{
     int val_now=readgpio(sensor4p);
    if(val_now==1&&Rcounter_reg==0)
    {
        Rcounter++;
	printf("Rcounter increase, now it is %d\n",Rcounter);
    }
    Rcounter_reg=val_now;
}
void forward_L()
{   
    int Ls=readgpio(sensor1p);
    int Rs=readgpio(sensor4p);
    counter_L();
    counter_R();
    if(Ls==0&&Rs==0)
    {
        tracking();
	stop_reg=0;
    }else if (Ls==1&&stop_reg==0)
    {
        if(Lcounter==1)
        {
            nomove();
  	    gpiovalue_set(armctl0p,"0");
            gpiovalue_set(armctl1p,"1");
            sleep(20);
            gpiovalue_set(armctl0p,"1");
            gpiovalue_set(armctl1p,"1");
	    
        }if(Lcounter==2)
        {
	    nomove();
            gpiovalue_set(armctl0p,"0");
            gpiovalue_set(armctl1p,"1");
            sleep(20);
            gpiovalue_set(armctl0p,"1");
            gpiovalue_set(armctl1p,"1");
        }if(Lcounter==3)
        {
           
	    nomove();
	    gpiovalue_set(armctl0p,"0");
            gpiovalue_set(armctl1p,"1");
            sleep(20);
            gpiovalue_set(armctl0p,"1");
            gpiovalue_set(armctl1p,"1");
        }if(Lcounter==4)
        {
		nomove();
            gpiovalue_set(armctl0p,"0");
            gpiovalue_set(armctl1p,"1");
            sleep(20);
            gpiovalue_set(armctl0p,"1");
            gpiovalue_set(armctl1p,"1");
        }
	stop_reg=1;

    }else if (Rs==1)
    {if(Rcounter<=2)
	{
        forward();
	usleep(300000);
	right();
        usleep(200000);
        keepturning();
	}
    else if(Rcounter==3)
    {
        forward();
	printf("to be end");
        sleep(1);
       
       // while(1)
       // {
       //      nomove();
       // }
    }
}
}




int readgpio(char* port)
{
char gpiopath[100];
char valpath[100];
int fd_r;
char buf[32];
sprintf(gpiopath,"/sys/class/gpio/gpio%s",port);
sprintf(valpath,"%s/value",gpiopath);
fd_r=open(valpath,O_RDWR);
if(fd_r<0)
{
    printf("read error, fd is %d",fd_r);
    return -1;
}

read(fd_r,buf,32);
close(fd_r);
//printf("buf is %s\n",&buf[0]);
if(*buf=='0')
return 0;
if(*buf=='1')
return 1;
}

void pintest(char *argv)
{
    gpioexport(argv);
    gpiovalue_set(argv,"0");
    int rea = readgpio(argv);
    printf("value is %d\n",rea);
    sleep(1);
    gpiovalue_set(argv,"1");
    rea = readgpio(argv);
    printf("value is %d\n",rea);
     sleep(1);
    gpiovalue_set(argv,"0");
    rea = readgpio(argv);
    printf("value is %d\n",rea);
}

void sensortest()
{
    int S1,S2,S3,S4;
    S1=readgpio(sensor1p);
    S2=readgpio(sensor2p);
    S3=readgpio(sensor3p);
    S4=readgpio(sensor4p);
    printf("S1 is %d\nS2 is %d\n S3 is %d\n S4 is %d\n",S1,S2,S3,S4);
}

int main()
{
    initialize();
    while(Rcounter<3)
    {
    forward_L();
    }
    forward();
    usleep(300000);
    nomove();
    return 0;
  // forward();

}
