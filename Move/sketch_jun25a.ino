#include <Servo.h>


#define mys1 2
#define mys2 3
#define mys3 4
#define mys4 5
#define mys5 6
#define mys6 7
Servo myservo[6];

int i=20;
int iod = 1; 

int C_01;
int C_10;
int C_Action;
int C_Action_reg;

int Serrx_reg;
int Sertx_reg;

int servoreg[6];


void setup() {
  // put your setup code here, to run once:
  servoinit();
}

void loop() 
{
  // put your main code here, to run repeatedly:
 /* pick_1();
  init_position();
  pick_2();
  init_position();
  putback_1();
  init_position();
  putback_2();
  init_position();
  */
  counter_01and10();
  Action_ctl();
  
  

}

void servoinit()
{
  myservo[0].attach(mys1,500,2500);  
	myservo[1].attach(mys2,500,2500);
	myservo[2].attach(mys3,500,2500);
	myservo[3].attach(mys4,500,2500);
	myservo[4].attach(mys5,500,2500);
  myservo[5].attach(mys6,500,2500);
  for(i=0;i<6;i++)
  {
    servoreg[i]=90;
  }
  pinMode(0,INPUT);
  pinMode(1,INPUT);
  Serrx_reg=1;
  Sertx_reg=1;
  C_Action=0;
  C_Action_reg=0;
  C_01=0;
  C_10=0;  
}


void Action_ctl()
{
  if(C_Action!=C_Action_reg)
  {
    switch(C_Action)
    {
      case 1:
        pick_1();        
      break;
      case 2:
        pick_2();
      break;
      case 3:
        putback_1();
      break;
      case 4:
        putback_2();

      break;
      
      default:
      break;
    }
    C_Action_reg=C_Action;
    init_position();

  }
}



void counter_01and10()
{
  int rxnow= digitalRead(0);
  int txnow= digitalRead(1);
  if(rxnow==1&&txnow==0&&Sertx_reg==1&&Serrx_reg==1)
  {
    C_10++;
    C_Action++;
  }
    if(rxnow==0&&txnow==1&&Sertx_reg==1&&Serrx_reg==1)
  {
    C_01++;
    C_Action++;
  }
  Sertx_reg=txnow;
  Serrx_reg=rxnow;
}


void init_position()
{
  
  servoctl(1,90,1000);
  servoctl(2,90,1000);
  servoctl(3,90,1000);
  servoctl(4,90,1000);
  servoctl(5,90,1000);
  servoctl(0,70,1000);
}

void pick_1v1()
{

  servoctl(0,10,1000);
  servoctl(1,90,500);
  servoctl(2,10,1000);
  servoctl(3,70,500);
  servoctl(5,150,1000);
  servoctl(4,133,500);
  servoctl(0,80,1000);

  
  servoctl(4,90,1000);
  servoctl(3,10,500);
  servoctl(5,0,1000);
  servoctl(0,10,500);

}

void pick_1()
{

  servoctl(0,10,1000);
  servoctl(1,90,500);
  servoctl(2,15,1000);
  servoctl(3,70,500);
  servoctl(5,180,1000);
  servoctl(4,133,500);
  servoctl(0,80,1000);

  
  servoctl(4,87,1000);
  servoctl(3,5,500);
  servoctl(5,0,1000);
  servoctl(0,10,500);

}


void pick_2()
{

  servoctl(0,10,1000);
  servoctl(1,90,500);
  servoctl(2,15,1000);
  servoctl(3,70,500);
  servoctl(5,180,1000);
  servoctl(4,133,500);
  servoctl(0,80,1000);


  servoctl(4,100,1000);
  servoctl(3,40,500);
  servoctl(2,0,1000);
  servoctl(5,0,1000);
  servoctl(0,30,500);

}
void pick_2v1()
{
  servoctl(0,10,1000);
  servoctl(1,90,500);
  servoctl(2,10,1000);
  servoctl(3,70,500);
  servoctl(5,150,1000);
  servoctl(4,133,500);
  servoctl(0,80,1000);

  
  servoctl(4,100,1000);
  servoctl(3,40,500);
  servoctl(2,0,1000);
  servoctl(5,0,1000);
  servoctl(0,30,500);
}

void putback_1()
{
  servoctl(5,0,1000);
  servoctl(0,10,1000);
  servoctl(2,12,1000);
  servoctl(3,10,500);
  servoctl(4,90,1000);
  servoctl(0,80,1000);

  servoctl(5,90,1000);
   servoctl(2,45,1000);
    servoctl(3,45,500);
  servoctl(4,120,1000);
 
 
  servoctl(5,180,1000);
  servoctl(0,10,1000);
}

void putback_2()
{
  servoctl(5,0,1000);
  servoctl(0,10,1000);
  servoctl(2,14,1000);
  servoctl(3,30,500);
  servoctl(4,100,1000);
  servoctl(0,80,1000);

  servoctl(5,90,1000);
   servoctl(2,55,1000);
     servoctl(3,45,500);
  servoctl(4,120,1000);

 
  servoctl(5,170,1000);
  servoctl(0,10,1000);
}


void servoctl(int id, int aim, int time)
{
 
 if((servoreg[id]-aim>90)||(servoreg[id]-aim<-90))
{
  int aim1 = aim/2 + servoreg[id]/2;
  int time1 = time/2;
  servoctl(id,aim1,time1);
  servoctl(id,aim,time-time1);
}
else
{
 int difference = (aim - servoreg[id])/10;
 int restone = aim - difference*10;
 int time_distance = time/10;
 int i=0;

 for(i=0;i<=10;i++)
 {
  myservo[id].write(servoreg[id]+difference*i);
  delay(time_distance);
 }
 myservo[id].write(aim);

 servoreg[id] = aim;
}

}