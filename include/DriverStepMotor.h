#ifndef DRIVERSTEPMOTOR_H_
#define DRIVERSTEPMOTOR_H_
#include <Arduino.h>
#include "HardwareSerial.h"
#include "vector"
#include "MathAlgorithm.h"
#define ROBOTArm 6
#define Joint1 0
#define Joint2 1
#define Joint3 2
#define wrist1 3
#define wrist2 4
#define wrist3 5
#define MaxNum 16777216
#define SixTeen2 256
#define SixTeen3 4096
#define SixTeen4 65536
#define SixTeen5 1048576
#define MaxSpeed 127
#define PosationAccaracy 0.01
#define StepTime 0.01
#define SumMAX 20
#define SumMAX 20

class StepMotorControl
{
    
    public:
    void SendData(char ModorID,char kk);
    StepMotorControl(int SerialRate);
    std::array<float, 6> GetActualAngel();          //获取一圈内的角度   0~360度
    std::array<float, 6> GetActualMoveAngel();      //获取实际运动的角度   
    std::array<int, 6> GetActualPulseNum();        //获取累计脉冲数
    std::array<float, 6> GetActualAngelError();    //获取角度误差
    std::array<char, 6> GetActualMoveEN();         //获取驱动板使能状态
    std::array<char, 6> GetMotorStall();            //获取电机堵转标准位
    std::array<float, 6> GetMotorCurrent();       //获取电机电流

    void initss();
    bool SetMotorSubdivision(char FineFraction);                //设置电机细分指令
    bool MotorEnOnOrOff(bool EN[6]);                //使能或者关闭驱动板
    bool MotorRunSpeed(std::array<float, 6> speed);        //电机速度驱动
    
    bool StopMotorSpeed(int NumMotor);        //停止电机正反转
    bool SaveSpeed();                            //保存电机速度
    bool ClearSpeed();                            //清除电机速度
    bool MovePosition(float location[6],float speed[6]);   //电机位置控制  
    bool MovePositionloop(float location[6],float speed[6]);   //电机闭环位置控制  
    int Getdata[6][5] =   {{0xe0, 0, 0, 0,0},
                          {0xe1, 0, 0, 0,0},
                          {0xe2, 0, 0, 0,0},
                          {0xe3, 0, 0, 0,0},
                          {0xe4, 0, 0, 0,0},
                          {0xe5, 0, 0, 0,0}};
    private:
    float MP=2;
    float MI=0.2;
    float MD=0;
    int Subdivision=16;
    int Sentdata[6][5] = {{0xe0, 0, 0, 0,0},
                          {0xe1, 0, 0, 0,0},
                          {0xe2, 0, 0, 0,0},
                          {0xe3, 0, 0, 0,0},
                          {0xe4, 0, 0, 0,0},
                          {0xe5, 0, 0, 0,0}};

    int Getdatas[6][5] =   {{0xe0, 0, 0, 0,0},
                          {0xe1, 0, 0, 0,0},
                          {0xe2, 0, 0, 0,0},
                          {0xe3, 0, 0, 0,0},
                          {0xe4, 0, 0, 0,0},
                          {0xe5, 0, 0, 0,0}};
    
};



#endif