#include<reg51.h>
#define port P1
#define cont_port P0
#define dataport P2  //Data port for LCD
#define m_sec 10
sbit rs = cont_port^1;
sbit rw = cont_port^2;
sbit en = cont_port^0;
sbit dig_hr1=port^0;
sbit dig_min1=port^1;
sbit start=port^2;
sbit am_pm=port^3;
sbit alarm_set=port^4;
sbit alarm=port^7;
int hr ,hr1=0,alarm_hr=0;
int min,min1=0,alarm_min=0;
int sec,sec1=0,dig_am_pm=0,alarm_am_pm=0;

void delay(unsigned int msec) // Time delay function
{
int i,j ;
for(i=0;i<msec;i++)
  for(j=0;j<1275;j++);
}

                
void lcd_cmd(unsigned char item) //Function to send command to LCD
{
dataport = item;
rs= 0;
rw=0;
en=1;
delay(1);
en=0;
return;
}

void lcd_data(unsigned char item) // Function to send data to LCD
{
dataport = item;
rs= 1;
rw=0;
en=1;
delay(1);
en=0;
return;
}

void lcd_data_string(unsigned char *str) // Function to send string to LCD
{
int i=0;
while(str[i]!='\0')
{
  lcd_data(str[i]);
  i++;
  delay(1);
}
return;
}

lcd_data_int(int time_val) // Function to send number to LCD
{
int int_amt;
int_amt=time_val/10;
lcd_data(int_amt+48);
int_amt=time_val%10; 
lcd_data(int_amt+48);
}

void lcd(unsigned char str1[10])
{
lcd_cmd(0x38); 
lcd_cmd(0x0e); 
delay(1);
lcd_data_string(str1);
}

void set_hr1() // // Function to set hours digit of clock
{
hr1++;
if(hr1>11)
hr1=0;
lcd_cmd(0xc3);
lcd_data_int(hr1);
lcd_data(':');
}

void set_min1()  // Function to set minutes digit of clock
{
min1++;
if(min1>59)
min1=0;
lcd_cmd(0xc6);
lcd_data_int(min1);
}

void set_alarm_hr1()  // Function to set hours digit of alarm
{
alarm_hr++;
if(alarm_hr>11)
alarm_hr=0;
lcd_cmd(0xc3);
lcd_data_int(alarm_hr);
lcd_data(':');
}

void set_alarm_min1() // Function to set minutes digit of Alarm
{
alarm_min++;
if(alarm_min>59)
alarm_min=0;
lcd_cmd(0xc6);
lcd_data_int(alarm_min);
}

void alarm_check()   // Function to check alarm
{
if(hr==alarm_hr)
{
  if(min==alarm_min)
  {
   if(dig_am_pm==alarm_am_pm)
   {
   alarm=1;
   lcd_cmd(0xc5);
   lcd("ALARM");
   delay(10);
   alarm = 0;
   }
  }
} 
}

void main()
{
int k;
start=1;
dig_hr1=1;
dig_min1=1;
alarm_set=1;
alarm=0;
lcd_cmd(0x83);
lcd("SET ALARM");
lcd_cmd(0xc3);
lcd_data_int(hr1);
lcd_data(':');
lcd_data_int(min1);

while(alarm_set==0)
{
  delay(10);
  if(dig_hr1==0)
  set_alarm_hr1();
  if(dig_min1==0)
  set_alarm_min1();
}

if(am_pm==0)
{
  lcd_cmd(0xc8);
  lcd_data_string("am");
  alarm_am_pm=0;
}

if(am_pm==1)
{
  lcd_cmd(0xc8);
  lcd_data_string("pm");
  alarm_am_pm=1;
}
delay(200);
lcd_cmd(0x01);
lcd_cmd(0x83);
lcd("SET TIMING");
lcd_cmd(0xc3);
lcd_data_int(hr1);
lcd_data(':');
lcd_data_int(min1);

while(start==0)
{
  delay(10);
  if(dig_hr1==0)
  set_hr1();
  if(dig_min1==0)
  set_min1(); 
}

if(am_pm==0)
{
  lcd_cmd(0xc8);
  lcd_data_string("am");
  dig_am_pm=0;
}

if(am_pm==1)
{
  lcd_cmd(0xc8);
  lcd_data_string("pm");
  dig_am_pm=1;
}
delay(200);
lcd_cmd(0x01);
while(1)
{
  for(k=0;k<2;k++)
  {
   for(hr=hr1;hr<12;hr++)
   {
    for(min=min1;min<60;min++)
    {
     for(sec=0;sec<60;sec++)
     {
      lcd_cmd(0x82);
      delay(1);
      lcd_data_int(hr);
      lcd_data(':');
      lcd_data_int(min);
      lcd_data(':');
      lcd_data_int(sec);
      if(dig_am_pm==0)
      {
       lcd("am");
      }
      else
      {
       lcd("pm");
      }
      delay(80);
      alarm_check();
      lcd_data_string("    ");
     }
    }
   min1=0;
   }
  if(dig_am_pm==0)
  dig_am_pm=1;
  else
  dig_am_pm=0;
  hr1=0;
  }
}
}