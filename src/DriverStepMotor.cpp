#include "DriverStepMotor.h"
HardwareSerial Serial1xx(PA3, PA2);
StepMotorControl::StepMotorControl(int SerialRate)
{
   // HardwareSerial Serial1xx(PA3, PA2);
   Serial1xx.begin(SerialRate);
}
void StepMotorControl::SendData(char ModorID, char kk)
{
   // for (int j = 0; j < ROBOTArm; j++)
   // {
   //    for (int i = 0; i < kk; i++)
   //    {
   //    Serial1xx.write(Sentdata[j][i]);
   //    }
   //  }
   for (int i = 0; i < kk; i++)
   {
      Serial1xx.write(Sentdata[ModorID][i]);
   }
}
std::array<float, 6> StepMotorControl::GetActualAngel()
{
   int u_numm = 0;
   std::array<float, 6> Angel = {0};
   for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0x30;
   }
   for (int i = 0; i <= 6; i++)
   {
      u_numm = 0;
      SendData(i, 2);
      while (Serial1xx.available() > 0)
      {
         Getdata[i][u_numm] = (int)Serial1xx.read();
         u_numm++;
      }
   }
   // for (int i = 0; i < 6; i++)
   // {
   //    Angel[i]=Getdata[i][1]*
   // }
   return Angel;
}

//电机位置控制    
bool StepMotorControl::MovePosition(float location[6],float speed[6])   
{
   int Pulse[6]={0};//发送的脉冲数
   
    for (int i = 0; i < ROBOTArm; i++)
   {
      Sentdata[i][1] = 0xfd;
      Pulse[i]=int(fabs(location[i])*Subdivision/1.8);
      
   }

   
}