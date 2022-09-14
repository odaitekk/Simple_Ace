#include "screen.h"
#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include <SHT2x.h>
#include <SimpleKalmanFilter.h>

TFT_eSPI tft = TFT_eSPI();
SHT20 sht;
/** Sprite needed for graph scrolling */
TFT_eSprite graph1 = TFT_eSprite(&tft);
SimpleKalmanFilter kf = SimpleKalmanFilter(0.5, 0.5, 0.01);


int rangeL = 2000;
int rangeH = 18000;

//   int valueBlock[5000];
//   int timeBlock[5000];
//   int locationBlock[5000];
//   int valuePos;
//   int blockPos;
//   int value;
//   int chk;

  
// // Editable Variables
//   bool proDebug = 0;

//   uint16_t graphColor = BLUE;
//   uint16_t pointColor = BLACK;
//   uint16_t lineColor = GREEN;

//   String graphName = "Time Graph";

//   int graphRange = 2000;
//   int markSize = 0;
  
// // Calculate Values
//   const int numberOfMarks = 250;
//   const int originX = 45;
//   const int originY = 200;
//   const int sizeX = 270;
//   const int sizeY = 150;
//   const int deviation = 10;
  
//   int boxSize = (sizeX / numberOfMarks);
//   int mark[numberOfMarks] ={0};
//   // {(boxSize + deviation), ((boxSize * 2) + deviation), ((boxSize * 3) + deviation), ((boxSize * 4) + deviation), ((boxSize * 5) + deviation), ((boxSize * 6) + deviation), ((boxSize * 7) + deviation), ((boxSize * 8) + deviation)};

//   const int minorSizeY = (originY + 10);
//   const int minorSizeX = (originX - 10);

//   int numberSize = (sizeY / 6);
//   int number[] = {numberSize, (numberSize * 2), (numberSize * 3), (numberSize * 4), (numberSize * 5), (numberSize * 6)};

//   int numberValue = (graphRange / 6);
//   int val[] = {5000, 6000,7000,8000,9000,10000};

void draw_Frame()
{

  tft.init();
  tft.fillScreen(TFT_BLACK);
  graph1.createSprite(200, 100);
}

QueueHandle_t Sensor_buff;
void draw_Graph(double value){
  sht.read();
  float H = sht.getHumidity();
  float F = kf.updateEstimate(H);

//  if(T>2000){
  graph1.scroll(-1); 
  // if(xQueueReceive(Sensor_buff, &(value),( TickType_t ) 10) ==pdTRUE){
    value = (double) (value-rangeL)*(150*3.14-0)/(rangeH-rangeL)+0;
    // printf("%lf\n",value);
    graph1.drawFastVLine(199, 100 - 100*(sin((value/150)-(3.14/2))+1),2, TFT_YELLOW);
    graph1.pushSprite(20, 32);
  //  }
    tft.drawString("Humidity",1, 5,2);
    tft.drawString("100",1, 32);
    tft.drawString("0",10, 132);
    // graph2.pushSprite(0, 96);
    tft.drawFloat(float(H), 1, 70, 180, 6);
    tft.drawFloat(float(value), 1, 70, 260, 1);
    
    tft.drawString("King's ",1, 250,2);
    tft.drawString("Technologies",1,270,1);
    tft.drawString("Phase",1,280,2);
  // }
}

void draw_result(double value){
  tft.drawString("Acetone",180, 270,1);
  tft.drawFloat((float)value,180, 290,1);
  vTaskDelay(100);
  tft.fillRect(270,170,40,30,TFT_BLACK);
}
