#include "DriverStepMotor.h"
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
HardwareSerial Serial1xx(PA3, PA2);
 TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
StepMotorControl::StepMotorControl(int SerialRate)
{
   // HardwareSerial Serial1xx(PA3, PA2);
  Serial1xx.begin(SerialRate);
}

void StepMotorControl::initss()
{
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
}


void StepMotorControl::SendData(char ModorID, char kk)
{
   for (int i = 0; i < kk; i++)
   {
      Serial1xx.write(Sentdata[ModorID][i]);
   }
}


std::array<float, ROBOTArm> StepMotorControl::GetActualMoveAngel()      //获取实际运动的角度 
{
   int u_numm = 0;
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][6]={0};
   int DataTwo[2]={0};
   
   std::array<float, ROBOTArm> Angel = {0};
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x36;
      Getdata[i][1]=0;
      Getdata[i][2]=0;
      Getdata[i][3]=0;
      Getdata[i][4]=0;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
      
   }
   
   for(int i = 0; i < ROBOTArm; i++)
   {
      mathAlgorithm::Turn10to16(Getdata[i][2],DataTwo);
      DataPara[i][0]=DataTwo[0];
      DataPara[i][1]=DataTwo[1];
      mathAlgorithm::Turn10to16(Getdata[i][3],DataTwo);
      DataPara[i][2]=DataTwo[0];
      DataPara[i][3]=DataTwo[1];
      mathAlgorithm::Turn10to16(Getdata[i][4],DataTwo);
      DataPara[i][4]=DataTwo[0];
      DataPara[i][5]=DataTwo[1];
   }
     for (int i = 0; i <= ROBOTArm; i++)
     {
       Para[i]=DataPara[i][0]*SixTeen5+DataPara[i][1]*SixTeen4+DataPara[i][2]*SixTeen3+DataPara[i][3]
       *SixTeen2+DataPara[i][4]*16+DataPara[i][5];
     }

    for (int i = 0; i <= ROBOTArm; i++)
    {
       
       if(Getdata[i][1]||0x7f==0xff)//为负值
       {
         Angel[i]=3.6*(Para[i]-MaxNum)/655.35;
       }
       else
       {
           Angel[i]=3.6*Para[i]/655.35;
       }
    }
     tft.drawFloat(Angel[0],2,20,70);
   return Angel;
}

std::array<float, 6> StepMotorControl::GetActualAngel()
{
   int u_numm = 0;
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][4]={0};
   int DataTwo[2]={0};
   std::array<float, ROBOTArm> Angel = {0};
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x30;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdatas[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   for(int i = 0; i < ROBOTArm; i++)
   {
      mathAlgorithm::Turn10to16(Getdatas[i][1],DataTwo);
      DataPara[i][0]=DataTwo[0];
      DataPara[i][1]=DataTwo[1];
      mathAlgorithm::Turn10to16(Getdatas[i][2],DataTwo);
      DataPara[i][2]=DataTwo[0];
      DataPara[i][3]=DataTwo[1];
   }

   for (int i = 0; i < 6; i++)
   {
      long ass=DataPara[i][0]*4096+DataPara[i][1]*256+DataPara[i][2]*16+DataPara[i][3];
      Angel[i]=float(3.6*ass/655.35);
   }
   return Angel;
}




std::array<int, 6> StepMotorControl::GetActualPulseNum()        //获取累计脉冲数
{
   int u_numm = 0;
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][6]={0};
   int DataTwo[2]={0};
   
   std::array<int, ROBOTArm> PulseNum = {0};
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x33;
   }

   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      } 
   }
   for(int i = 0; i < ROBOTArm; i++)
   {
      mathAlgorithm::Turn10to16(Getdata[i][2],DataTwo);
      DataPara[i][0]=DataTwo[0];
      DataPara[i][1]=DataTwo[1];
      mathAlgorithm::Turn10to16(Getdata[i][3],DataTwo);
      DataPara[i][2]=DataTwo[0];
      DataPara[i][3]=DataTwo[1];
      mathAlgorithm::Turn10to16(Getdata[i][4],DataTwo);
      DataPara[i][4]=DataTwo[0];
      DataPara[i][5]=DataTwo[1];
   }
   for (int i = 0; i <= ROBOTArm; i++)
     {
       Para[i]=DataPara[i][0]*SixTeen5+DataPara[i][1]*SixTeen4+DataPara[i][2]*SixTeen3+DataPara[i][3]
       *SixTeen2+DataPara[i][4]*16+DataPara[i][5];
     }

    for (int i = 0; i <= ROBOTArm; i++)
    {
       
       if(Getdata[i][1]||0x7f==0xff)//为负值
       {
        PulseNum[i]=Para[i]-MaxNum;
       }
       else
       {
           PulseNum[i]=Para[i];
       }
    }
    return PulseNum;
}

std::array<float, 6> StepMotorControl::GetActualAngelError()    //获取角度误差
{
   int u_numm = 0;
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][4]={0};
   int DataTwo[2]={0};
   std::array<float, 6> AngleError{0};
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x30;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   for(int i = 0; i < ROBOTArm; i++)
   {
      mathAlgorithm::Turn10to16(Getdata[i][1],DataTwo);
      DataPara[i][0]=DataTwo[0];
      DataPara[i][1]=DataTwo[1];
      mathAlgorithm::Turn10to16(Getdata[i][2],DataTwo);
      DataPara[i][2]=DataTwo[0];
      DataPara[i][3]=DataTwo[1];
   }
    for (int i = 0; i < 6; i++)
   {
      long ass=DataPara[i][0]*SixTeen3+DataPara[i][1]*SixTeen2+DataPara[i][2]*16+DataPara[i][3];
      AngleError[i]=float(ass/182.44);
   }
   return AngleError;
}

 //获取驱动板使能状态    1  使能   0没使能   -1  错误
std::array<char, 6> StepMotorControl::GetActualMoveEN()        
{
   int u_numm = 0;
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][4]={0};
   int DataTwo[2]={0};
   std::array<char, 6> MoveEN={0};
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x3a;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }

   for(int i = 0; i < ROBOTArm; i++)
   {
      mathAlgorithm::Turn10to16(Getdata[i][1],DataTwo);
      DataPara[i][0]=DataTwo[0];
      DataPara[i][1]=DataTwo[1];
   }

      for(int i = 0; i < ROBOTArm; i++)
   {
      if (DataPara[i][1]==1)
      MoveEN[i]=1;
      else if (DataPara[i][1]==2)
      MoveEN[i]=0;
      else
      MoveEN[i]=-1;
   }
   return MoveEN;

}


//电机位置控制    
bool StepMotorControl::MovePosition(float location[ROBOTArm],float speed[ROBOTArm])   
{

   int Pulse[ROBOTArm]={0};//发送的脉冲数
   int RotateFlag[ROBOTArm]={0};//正反转标志位
   std::array<float, 6> NowAngle{0};
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][2]={0};
   int DataTwo[2]={0};
   int u_numm = 0;
   float loca=0;
   float loca1=0;
   NowAngle=GetActualMoveAngel();
   //  tft.drawFloat(NowAngle[0],2,50,100);
    for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xfd;
      RotateFlag[i]=location[i]-NowAngle[i]>0?1:-1;
      Pulse[i]=int(fabs(location[i]-NowAngle[i])*Subdivision/1.8);
      if(location[i]-NowAngle[i]>0)
         RotateFlag[i]=1;
         else  RotateFlag[i]=-1;
   }
    for (int i = 0; i < ROBOTArm; i++)
    {
       if(speed[i]>MaxSpeed)
        Sentdata[i][2]=MaxSpeed;
        else Sentdata[i][2]=speed[i];
       if(RotateFlag[i]<0)
       {
          Sentdata[i][2]=Sentdata[i][2]&0x7f;
       }
       else
       {
          Sentdata[i][2]=Sentdata[i][2]|0x80;
       }
       int data1=Pulse[i]/SixTeen3;
       int data2=(Pulse[i]-data1*SixTeen3)/SixTeen2;
       int data3=(Pulse[i]-data1*SixTeen3-data2*SixTeen2)/16;
       int data4=Pulse[i]-data1*SixTeen3-data2*SixTeen2-data3*16;
       Sentdata[i][3]=data1*16+data2;
       Sentdata[i][4]=data3*16+data4;
    }
    for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 5);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   if (Getdata[0][1]==1)
   {
      return true;
   }
   else return false;
}



bool StepMotorControl::SetMotorSubdivision(char FineFraction)                //设置电机细分指令
{
   int u_numm = 0;
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x84;
      Sentdata[i][2] = FineFraction;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 3);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   return true;

}

bool StepMotorControl::MotorEnOnOrOff(bool EN[6])            //使能或者关闭驱动板
{
   int u_numm = 0;
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xf3;
      Sentdata[i][2] = EN[i];
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   return true;
}

bool StepMotorControl::MotorRunSpeed(std::array<float, 6> speed)        //电机速度驱动
{
   int u_numm = 0;
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][4]={0};
   int DataTwo[2]={0};
   std::array<float, 6> NowAngle{0};
   NowAngle=GetActualMoveAngel();
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xf6;
   }
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][2]=fabs(speed[i]);
    if(speed[i]<0)
       {
          Sentdata[i][2]=Sentdata[i][2]&0x7f;
       }
       else if(speed[i]>0)
       {
          Sentdata[i][2]=Sentdata[i][2]|0x80;
       }
       else Sentdata[i][2]=0;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      if(speed[i]==1000.0)
      {
         Sentdata[i][1]=0xF7;
         tft.drawFloat(34,2,20,40);
         SendData(i, 2);
      }
      else
      SendData(i, 3);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   return true;
}



 bool StepMotorControl::StopMotorSpeed(int NumMotor)        //停止对应电机正反转
 {
   int u_numm = 0;
   Sentdata[NumMotor][1] = 0xF7;
   u_numm = 0;
   SendData(NumMotor, 2);
   while (Serial1xx.available() > 0)
   {
      Getdata[NumMotor][u_numm] = (int)Serial1xx.read();
      u_numm++;
   }
   return true;
} 

 bool StepMotorControl::SaveSpeed()                            //保存电机速度
 {
    int u_numm = 0;
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xff;
      Sentdata[i][1] = 0xc8;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 3);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   return true;
 }


  bool StepMotorControl::ClearSpeed()                            //保存电机速度
 {
    int u_numm = 0;
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xff;
      Sentdata[i][1] = 0xca;
   }
   for (int i = 0; i <= ROBOTArm; i++)
   {
      u_numm = 0;
      SendData(i, 3);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   return true;
 }


 bool StepMotorControl::MovePositionloop(float location[6],float speed[6])  //电机闭环位置控制  
{
   int Pulse[ROBOTArm]={0};//发送的脉冲数
   int RotateFlag[ROBOTArm]={0};//正反转标志位
   std::array<float, 6> NowAngle{0};
   static std::array<float, 6> AngleError{0};
   static std::array<float, 6> AngleErrorLast{0};
   static std::array<float, 6> SumAngleError{0};
   std::array<float, 6> OutSpeed{0};
   long Para[ROBOTArm]={0};
   int DataPara[ROBOTArm][2]={0};
   int DataTwo[2]={0};
   int u_numm = 0;
   float loca=0;
   float loca1=0;
   NowAngle=GetActualMoveAngel();
   for(int i=0;i<6;i++)
   {
      AngleError[i]=location[i]-NowAngle[i];
      if ((AngleError[i] > -PosationAccaracy) && (AngleError[i] < PosationAccaracy))
    {
        AngleError[i] = 0;
        AngleErrorLast[i] = 0;
        StopMotorSpeed(i);
    }
    // outpid += Spid.Skp * (error - errorLast1)+Spid.Ski * error;//PID算法  返回值为outpid
    if (AngleError[i] > -10 && AngleError[i] < 10)
    {
        SumAngleError[i] += AngleError[i] * StepTime;
        /*积分范围限定，防止积分饱和*/
        if (SumAngleError[i] > SumMAX)
        {
            SumAngleError[i] = SumMAX;
        }
        else if (SumAngleError[i] < -SumMAX)
        {
            SumAngleError[i] = -SumMAX;
        }
    }
   }
    for(int i=0;i<6;i++)
    {
       OutSpeed[i]=MP*AngleError[i]+MI*SumAngleError[i]+MD*(AngleError[i]-AngleErrorLast[i]);
       if(OutSpeed[i]>=speed[i])
       {
          OutSpeed[i]=speed[i];
       }
       else if (OutSpeed[i]<-speed[i])
       OutSpeed[i]=-speed[i];

       if ((AngleError[i] > -PosationAccaracy) && (AngleError[i] < PosationAccaracy))
       {
          OutSpeed[i]=1000.0;
       }
    }
    MotorRunSpeed(OutSpeed); 
    for(int i=0;i<6;i++)
    {
       AngleErrorLast[i]=AngleError[i];
    }
    return true;
    
}