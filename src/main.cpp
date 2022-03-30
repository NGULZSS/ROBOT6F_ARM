#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
// #include "m_servo.h"
#include <STM32FreeRTOS.h>
#include "OneButton.h"
#include <Arduino.h>
#include "DriverStepMotor.h"
TFT_eSPI tft = TFT_eSPI(); // Invoke library, pins defined in User_Setup.h
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
#define KEY3 A4 //按键3
OneButton button1(KEY1, true);
OneButton button2(KEY2, true);
OneButton button3(KEY3, true);
void ClickRight();
void ClickLeft();
void ClickCenter();
#pragma endregion

 void setup(void) {
  pinMode(PA0,OUTPUT);
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
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
	while(1)
	{
        SMC.GetActualAngel();
        tft.drawNumber(SMC.Getdata[0][1],20,30);
        tft.drawNumber(SMC.Getdata[0][2],20,50);
        tft.drawNumber(SMC.Getdata[0][3],20,70);
        vTaskDelay(100);			//延时500ms
        tft.drawNumber(data[4],50,60);
	}
}
//控制线程
void ConturlThread(void *pvParameters)
{
  
	while(1)
	{
     
     vTaskDelay(15);			//延时500ms
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
    digitalWrite(PA0,LOW);
    
}
void ClickLeft()
{
  static int as=10;
}
void ClickCenter()
{
  
}


