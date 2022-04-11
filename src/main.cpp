#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
// #include "m_servo.h"
#include <STM32FreeRTOS.h>
#include "OneButton.h"
#include <Arduino.h>
#include "DriverStepMotor.h"
 //TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
StepMotorControl SMC(19200);
int data[5]={0};
#pragma region 线程创建
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

TaskHandle_t ShowThread_Handler;
#define ShowThread_PRIO		3
void ShowThread(void *pvParameters);

TaskHandle_t ConturlThread_Handler;
#define ConturlThread_PRIO		2
void ConturlThread(void *pvParameters);

TaskHandle_t CommunicationThread_Handler;
#define CommunicationThread_PRIO		3
void CommunicationThread(void *pvParameters);
char InfoBuffer[1000];//信息存取
QueueHandle_t Speedqueue;
QueueHandle_t Positionqueue;
#pragma endregion

#pragma region 按键定义并初始化
#define KEY1 PC13 //按键1
#define KEY2 PC14 //按键2
#define KEY3 PB9 //按键3
OneButton button1(KEY1, true);
OneButton button2(KEY2, true);
OneButton button3(KEY3, true);
void ClickRight();
void ClickLeft();
void ClickCenter();
#pragma endregion


bool flag=false;
 void setup(void) {
  pinMode(PA0,OUTPUT);
  SMC.initss();
  // tft.init();
  // tft.setRotation(0);
  // tft.fillScreen(TFT_BLACK);
  // tft.setTextColor(TFT_GREEN, TFT_BLACK);
  button1.attachClick(ClickRight);
  button2.attachClick(ClickLeft);
  button3.attachClick(ClickCenter);
  delay(1000);
  xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )128,                   //任务堆栈大小
                (void*          )NULL,                  //传递给任务参数
                (UBaseType_t    )1,                     //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄            
    vTaskStartScheduler();          //¿ªÆôÈÎÎñµ÷¶È
 }

 void loop() {
  
  button1.tick();
  button2.tick();
  button3.tick();

 }


void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
    Speedqueue = xQueueCreate( 50, sizeof( float ) );
    Positionqueue = xQueueCreate( 50, sizeof( float ) );
    //显示线程创建
    xTaskCreate((TaskFunction_t )ShowThread,             
                (const char*    )"ShowThread",           
                (uint16_t       )128,        
                (void*          )NULL,                  
                (UBaseType_t    )ShowThread_PRIO,        
                (TaskHandle_t*  )&ShowThread_Handler);   
    //控制线程创建
    xTaskCreate((TaskFunction_t )ConturlThread,     
                (const char*    )"ConturlThread",   
                (uint16_t       )128,
                (void*          )NULL,
                (UBaseType_t    )ConturlThread_PRIO,
                (TaskHandle_t*  )&ConturlThread_Handler); 
     //通信线程创建
    xTaskCreate((TaskFunction_t )CommunicationThread,     
                (const char*    )"CommunicationThread",   
                (uint16_t       )128,
                (void*          )NULL,
                (UBaseType_t    )CommunicationThread_PRIO,
                (TaskHandle_t*  )&CommunicationThread_Handler); 
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//显示线程
void ShowThread(void *pvParameters)
{
  std::array<float, ROBOTArm> Angel = {0};
  std::array<float, ROBOTArm> Angelac = {0};
  std::array<float, 6> error={0};
  double saaa;
  int DataTwo[2]={0};
  int DataPara[4]={0};
   //Angelac=SMC.GetActualAngel();   
	while(1)
	{
        //tft.drawFloat(10.2,2,50,60);
        taskENTER_CRITICAL();           //进入临界区
        //Angel=SMC.GetActualMoveAngel();
        taskEXIT_CRITICAL();      //退出临界区    此段代码不可以被打断
         vTaskDelay(100);			//延时500ms
        //tft.drawFloat(Angel[0],2,20,70);
      
        
	}
}
//控制线程
void ConturlThread(void *pvParameters)
{
  std::array<float, ROBOTArm> Angelac = {0};
  float Angelac1[6] = {360,60,60,60,60,60};
    //float Speed[6] = {20,20,20,20,20,20};
    std::array<float, ROBOTArm> Speed = {-120,20,20,20,20,20};
    double count=0;
	while(1)
	{
     //Angelac=SMC.GetActualAngel();   
     if(flag==true)
     {
        //SMC.MovePositionloop(Angelac1,Speed);
        Speed[0]=-60*sin(count*PI/180)+-60;
        SMC.MotorRunSpeed(Speed);
        count++;
        if(count==360)
        {
          count=0;
        }

     }
     vTaskDelay(15);			//延时500ms
     //tft.drawFloat(Angelac[0],2,20,50);
	}
}
//通信线程
void CommunicationThread(void *pvParameters)
{
	while(1)
	{
    digitalWrite(PA0,LOW);
    vTaskDelay(200);	
    digitalWrite(PA0,HIGH);
    vTaskDelay(200);	
	}
	}

void ClickRight()
{
    //mservo.set_angle(1,90,100);
    float Angelac1[6] = {60,60,60,60,60,60};
    float Speed[6] = {100,20,20,20,20,20};
    std::array<float, ROBOTArm> Angel = {0};
    SMC.MovePosition(Angelac1,Speed);
    //tft.drawFloat(0.4,2,70,110);
    // Angel=SMC.GetActualMoveAngel();
    // tft.drawFloat(Angel[0],2,20,70);
}
void ClickLeft()
{
  static int as=10;
  //tft.drawFloat(0.2,2,110,50);
}
void ClickCenter()
{
  float Angelac1[6] = {-50,60,60,60,60,60};
  std::array<float, ROBOTArm> Speed = {-120,20,20,20,20,20};
   //SMC.MovePosition(Angelac1,Speed);
   //SMC.MotorRunSpeed(Speed);
  flag=true;
  //   std::array<float, ROBOTArm> Angelac = {0};
  //  Angelac=SMC.GetActualAngel();  
  //  tft.drawFloat(Angelac[0],2,20,50); 
  //SMC.SetMotorSubdivision();
  //tft.drawFloat(0.2,2,110,80);
}


