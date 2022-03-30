#include "DriverStepMotor.h"
HardwareSerial Serial1xx(PA3, PA2);
StepMotorControl::StepMotorControl(int SerialRate)
{
   // HardwareSerial Serial1xx(PA3, PA2);
   Serial1xx.begin(SerialRate);
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
      long ass=DataPara[i][0]*4096+DataPara[i][1]*256+DataPara[i][2]*16+DataPara[i][3];
      Angel[i]=float(3.6*ass/655.35);
   }
   return Angel;
}

//电机位置控制    
bool StepMotorControl::MovePosition(float location[ROBOTArm],float speed[ROBOTArm])   
{
   int Pulse[ROBOTArm]={0};//发送的脉冲数
   int RotateFlag[ROBOTArm]={0};//正反转标志位

    for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xfd;
      RotateFlag[i]=location[i];
      Pulse[i]=int(fabs(location[i])*Subdivision/1.8);
   }
   
   return false;
}