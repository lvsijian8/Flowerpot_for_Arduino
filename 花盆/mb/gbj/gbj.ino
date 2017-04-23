#include <reg51.h>
#include <intrins.h>
#define uchar   unsigned char  
#define uint    unsigned int
sbit SDA=P3^6;     //模拟I2C数据传送位SDA        
sbit SCL=P3^7;     //模拟I2C时钟控制位SCL
sbit INT=P3^2;      
sbit RESET=P3^3;
sbit led0=P1^0;
sbit led1=P1^1;
sbit led2=P1^2;
sbit led3=P1^3;
sbit led4=P1^4;
sbit led5=P1^5;
sbit led6=P1^6;
sbit led7=P1^7;
bit  ack;          //应答标志位

#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址
#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
#define DS3231_SALARM1ECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日
#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位)  
 

uchar code dis_code[11]={0xc0,0xf9,0xa4,0xb0,   // 0,1,2,3
            0x99,0x92,0x82,0xf8,0x80,0x90, 0xff};       // 4,5,6,7,8,9,off
uchar data dis_buf[8];
uchar data dis_index;
uchar data dis_digit;


uchar BCD2HEX(uchar val)    //BCD转换为Byte
{
    uchar temp;
    temp=val&0x0f;
    val>>=4;
    val&=0x0f;
    val*=10;
    temp+=val;
   
    return temp;
}

uchar HEX2BCD(uchar val)    //B码转换为BCD码
{
    uchar i,j,k;
    i=val/10;
    j=val;
    k=j+(i<<4);
    return k;
}

void delayus(uint us) 
{
    while (us--);
}

void Start_I2C()
{
    SDA=1;                  //发送起始条件的数据信号
    delayus(1);
    SCL=1;
    delayus(5);             //起始条件建立时间大于4.7us,延时
   
    SDA=0;                  //发送起始信号
    delayus(5);             // 起始条件锁定时间大于4μs
       
    SCL=0;                  //钳住I2C总线，准备发送或接收数据
    delayus(2);
}

void Stop_I2C()
{
    SDA=0;                  //发送结束条件的数据信号
    delayus(1);             //发送结束条件的时钟信号
    SCL=1;                  //结束条件建立时间大于4us
    delayus(5);
   
    SDA=1;                  //发送I2C总线结束信号
    delayus(4);
}

void SendByte(uchar c)
{
    uchar BitCnt;
   
    for(BitCnt=0;BitCnt<8;BitCnt++)         //要传送的数据长度为8位
    {
        if((c<<BitCnt)&0x80)
            SDA=1;                          //判断发送位
        else 
            SDA=0;               
          delayus(1);
          SCL=1;                            //置时钟线为高，通知被控器开始接收数据位
          delayus(5);                       //保证时钟高电平周期大于4μs   
          SCL=0;
    }
   
    delayus(2);
    SDA=1;                                  //8位发送完后释放数据线，准备接收应答位
    delayus(2);  
    SCL=1;
    delayus(3);
    if(SDA==1)
        ack=0;    
    else
        ack=1;                              //判断是否接收到应答信号
    SCL=0;
    delayus(2);
}

uchar RcvByte()
{
   uchar retc;
   uchar BitCnt;
 
   retc=0;
   SDA=1;                           //置数据线为输入方式
   for(BitCnt=0;BitCnt<8;BitCnt++)
   {
        delayus(1);  
        SCL=0;                      //置时钟线为低，准备接收数据位
       
        delayus(5);                 //时钟低电平周期大于4.7μs
      
        SCL=1;                      //置时钟线为高使数据线上数据有效
        delayus(3);
        retc=retc<<1;
        if(SDA==1)
            retc=retc+1;            //读数据位,接收的数据位放入retc中
        delayus(2);
   }
   SCL=0;
   delayus(2);
   return(retc);
}
 

void Ack_I2C(bit a)
{
 
    if(a==0)
        SDA=0;              //在此发出应答或非应答信号
    else
        SDA=1;
    delayus(3);     
    SCL=1;
   
    delayus(5);             //时钟低电平周期大于4μs
   
    SCL=0;                  //清时钟线，钳住I2C总线以便继续接收
    delayus(2);   
}
 

uchar write_byte(uchar addr, uchar write_data)
{
    Start_I2C();
    SendByte(DS3231_WriteAddress);
    if (ack == 0)
        return 0;
   
    SendByte(addr);   
    if (ack == 0)
        return 0;
   
    SendByte(write_data);
    if (ack == 0)
        return 0;
   
    Stop_I2C();
    delayus(10);      
    return 1;
}
 

uchar read_current()
{
    uchar read_data;
    Start_I2C();
    SendByte(DS3231_ReadAddress);
    if(ack==0)
        return(0);
   
    read_data = RcvByte();
    Ack_I2C(1);
    Stop_I2C();
    return read_data;
}
 

uchar read_random(uchar random_addr)
{
    Start_I2C();
    SendByte(DS3231_WriteAddress);
    if(ack==0)
        return(0);
   
    SendByte(random_addr);
    if(ack==0)
        return(0);
   
    return(read_current());
}
 

void ModifyTime(uchar yea,uchar mon,uchar da,uchar hou,uchar min,uchar sec)
{
    uchar temp=0;
   
    temp=HEX2BCD(yea);
    write_byte(DS3231_YEAR,temp);   //修改年
   
    temp=HEX2BCD(mon);
    write_byte(DS3231_MONTH,temp);  //修改月
   
    temp=HEX2BCD(da);
    write_byte(DS3231_DAY,temp);    //修改日
   
    temp=HEX2BCD(hou);
    write_byte(DS3231_HOUR,temp);   //修改时
   
    temp=HEX2BCD(min);
    write_byte(DS3231_MINUTE,temp); //修改分
   
    temp=HEX2BCD(sec);
    write_byte(DS3231_SECOND,temp); //修改秒
}
 

void TimeDisplay(uchar Dhour,uchar Dmin,uchar Dsec)
{
    dis_buf[7]=dis_code[Dhour / 10];        // 时十位
    dis_buf[6]=dis_code[Dhour % 10];        // 时个位
    dis_buf[4]=dis_code[Dmin / 10];         // 分十位
    dis_buf[3]=dis_code[Dmin % 10];         // 分个位
    dis_buf[1]=dis_code[Dsec / 10];         // 秒十位
    dis_buf[0]=dis_code[Dsec % 10];         // 秒个位
    dis_buf[2]=0xbf;                        // 显示"-"
    dis_buf[5]=0xbf; 
}
 


void DateDisplay(uchar Dyear,uchar Dmonth,uchar Dday)
{
    dis_buf[7]=dis_code[Dyear / 10];        // 年十位
    dis_buf[6]=dis_code[Dyear % 10];        // 年个位
    dis_buf[4]=dis_code[Dmonth / 10];       // 月十位
    dis_buf[3]=dis_code[Dmonth % 10];       // 月个位
    dis_buf[1]=dis_code[Dday / 10];         // 天十位
    dis_buf[0]=dis_code[Dday % 10];         // 天个位
    dis_buf[2]=0xbf;                        // 显示"-"
    dis_buf[5]=0xbf;
}
 


void get_show_time(void)
{
    uchar Htemp1,Htemp2,Mtemp1,Mtemp2,Stemp1,Stemp2; 
 
    Htemp1=read_random(DS3231_HOUR);    //时 24小时制
    Htemp1&=0x3f;                   
    Htemp2=BCD2HEX(Htemp1);
   
    Mtemp1=read_random(DS3231_MINUTE);  //分
    Mtemp2=BCD2HEX(Mtemp1);
   
    Stemp1=read_random(DS3231_SECOND);  //秒
    Stemp2=BCD2HEX(Stemp1);
   
    TimeDisplay(Htemp2,Mtemp2,Stemp2);
}
 

void get_show_date(void)
{
    uchar Ytemp1,Ytemp2,Mtemp1,Mtemp2,Dtemp1,Dtemp2; 
   
    Ytemp1=read_random(DS3231_YEAR);        //年
    Ytemp2=BCD2HEX(Ytemp1);
   
    Mtemp1=read_random(DS3231_MONTH);       //月
    Mtemp2=BCD2HEX(Mtemp1);
   
    Dtemp1=read_random(DS3231_DAY);         //日
    Dtemp2=BCD2HEX(Dtemp1);
   
    DateDisplay(Ytemp2,Mtemp2,Dtemp2);
}
 


void get_show_Temperature(void)
{
    uchar Ttemp1,Ttemp2,Ttemp3,Ttemp4; 
   
    Ttemp1=read_random(DS3231_TEMPERATUREH);    //温度 高字节
    Ttemp2=BCD2HEX(Ttemp1);
   
    Ttemp3=read_random(DS3231_TEMPERATUREL);    //温度低字节
    Ttemp4=BCD2HEX(Ttemp3);
    
    DateDisplay(0,Ttemp2,Ttemp4);
}
 
 
void timer0() interrupt 1
{
    TH0=0xFC;
    TL0=0x17;
   
    P2=0xff;                    // 先关闭所有数码管
    P0=dis_buf[dis_index];      // 显示代码传送到P0口
    P2=dis_digit;
   
    if (dis_digit & 0x80)
        dis_digit=(dis_digit << 1) | 0x1;
    else
        dis_digit=(dis_digit << 1);
   
    dis_index++;
    dis_index&=0x07;            // 8个数码管全部扫描完一遍之后，再回到第一个开始下一次扫描
}
 

void main()
{
    uint ii = 0;
   
   
    RESET=0x1;          //DS3231复位操作，正常操作下不需要每次都复位
    delayus(5000);
   
    led0=0;
    led1=0;
    led2=0;
    led3=0;
    led4=0;
   
    P0=0xff;
    P2=0xff;
   
    dis_digit=0xfe;
    dis_index=0;
   
    TimeDisplay(12, 5, 18);
   
   
    TMOD=0x11;          // 定时器0, 1工作模式1, 16位定时方式
    TH0=0xFC;
    TL0=0x17;
   
    TCON=0x01;
    IE=0x82;            // 使能timer0,1 中断
   
    TR0=1;
   
   
    if (write_byte(DS3231_CONTROL, 0x1C) == 0)
        led0=1;
   
    if (write_byte(DS3231_STATUS, 0x00) == 0)
        led1=1;
   
    ModifyTime(10,6,13,15,30,00);       //初始化时钟,2010/6/13,15/30/00
                                        //小时采用24小时制
    while(1)
    {
        //get_show_date();              //显示日期
        //get_show_Temperature();       //显示温度
        get_show_time();                //显示时间
        delayus(50000);
    }
   
}
