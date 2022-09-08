#include "screen.h"
#include <Arduino.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  

  int valueBlock[5000];
  int timeBlock[5000];
  int locationBlock[5000];
  int valuePos;
  int blockPos;
  int value;
  int chk;

  
// Editable Variables
  bool proDebug = 0;

  uint16_t graphColor = BLUE;
  uint16_t pointColor = BLACK;
  uint16_t lineColor = GREEN;

  String graphName = "Time Graph";

  int graphRange = 2000;
  int markSize = 0;
  
// Calculate Values
  const int numberOfMarks = 250;
  const int originX = 45;
  const int originY = 200;
  const int sizeX = 270;
  const int sizeY = 150;
  const int deviation = 10;
  
  int boxSize = (sizeX / numberOfMarks);
  int mark[numberOfMarks] ={0};
  // {(boxSize + deviation), ((boxSize * 2) + deviation), ((boxSize * 3) + deviation), ((boxSize * 4) + deviation), ((boxSize * 5) + deviation), ((boxSize * 6) + deviation), ((boxSize * 7) + deviation), ((boxSize * 8) + deviation)};

  const int minorSizeY = (originY + 10);
  const int minorSizeX = (originX - 10);

  int numberSize = (sizeY / 6);
  int number[] = {numberSize, (numberSize * 2), (numberSize * 3), (numberSize * 4), (numberSize * 5), (numberSize * 6)};

  int numberValue = (graphRange / 6);
  int val[] = {5000, 6000,7000,8000,9000,10000};

void draw_Frame()
{
  // draw title
  tft.begin();
  tft.fillScreen(BLACK);
  tft.setRotation(1);
  vTaskDelay(500);

  tft.setCursor(10, 10); // set the cursor
  tft.setTextColor(BLUE); // set the colour of the text
  tft.setTextSize(4); // set the size of the text
  tft.println(graphName);
  
  for(int i = 0; i<numberOfMarks;i++){
  mark[i]= boxSize*i+deviation;
  }
  // draw outline
  tft.drawLine(originX, originY, (originX + sizeX), originY, graphColor); //x - axis
  tft.drawLine(originX, originY, originX, (originY - sizeY), graphColor); //y - axis

  // // draw lables
  // for(int i = 0; i < numberOfMarks; i++)
  // {
  //   tft.drawLine(mark[i], originY, mark[i], minorSizeY, graphColor);
  // }

  // draw numbers
  for(int i = 0; i < 6; i++)
  {
    tft.drawLine(originX, (originY - number[i]), minorSizeX, (originY - number[i]), graphColor);
  }

  // draw number values
  for(int i = 0; i < 6; i++)
  {
    tft.setCursor((minorSizeX - 30), (number[i] + numberSize));
    tft.setTextColor(graphColor);
    tft.setTextSize(1);
    tft.println(val[i]);
  }
  tft.fillRect((originX + 2), (originY-sizeY ),sizeX, 140, WHITE);
}

void draw_Graph(int16_t value)
{
  valueBlock[valuePos] = value;
  
  if(proDebug)
  {
    Serial.println(timeBlock[valuePos]);
  }
  
 if(blockPos < 250)
  {
    // print the time
    tft.setCursor((mark[valuePos] - 5), (originY + 16));
    tft.setTextColor(graphColor, WHITE);
    tft.setTextSize(1);
    // tft.println(timeBlock[valuePos]);
    
    // map the value
    locationBlock[valuePos] = map(value, 5000,10000, originY-5, (originY - sizeY)); //map value to y -coordinare on geaph

    if(valuePos != 0)
    {
      tft.drawLine(mark[valuePos]+50, locationBlock[valuePos], mark[(valuePos - 1)]+50, locationBlock[(valuePos - 1)], lineColor);
      tft.drawLine(mark[valuePos]+50+1, locationBlock[valuePos], mark[(valuePos - 1)]+50+1, locationBlock[(valuePos - 1)], lineColor);
      tft.drawLine(mark[valuePos]+50+2, locationBlock[valuePos], mark[(valuePos - 1)]+50+2, locationBlock[(valuePos - 1)], lineColor);
    }

    blockPos= blockPos+1;
  }else
    // map the value - current point
    {
    tft.fillRect((originX + 2), (originY - sizeY), sizeX, sizeY, WHITE);  
    locationBlock[valuePos] = map(value, 5000, 10000, originY-5, (originY - sizeY));

    // draw point - current point
    tft.fillRect((mark[249]), (locationBlock[valuePos] - 1), markSize, markSize, pointColor);

    // draw all points
    // for(int i = 0; i < 100; i++)
    // {
    //   tft.fillRect((mark[(blockPos - (i + 1))] - 1), (locationBlock[(valuePos - i)] - 1), markSize, markSize, pointColor);
    // }

    // draw all the lines
    for(int i = 0; i < 249; i++)
    {
      tft.drawLine(mark[blockPos - (i + 1)]+50, locationBlock[valuePos - i], mark[blockPos - (i + 2)]+50, locationBlock[valuePos - (i + 1)], lineColor);
      tft.drawLine(mark[blockPos - (i + 1)]+50+1, locationBlock[valuePos - i], mark[blockPos - (i + 2)]+50+1, locationBlock[valuePos - (i + 1)], lineColor);
      tft.drawLine(mark[blockPos - (i + 1)]+50+2, locationBlock[valuePos - i], mark[blockPos - (i + 2)]+50+2, locationBlock[valuePos - (i + 1)], lineColor);
    }
    
  }
  valuePos++;
  if(valuePos==4999){
    valuePos = 0;
    blockPos =0;
    tft.fillRect((originX + 2), (originY - sizeY), sizeX, sizeY, WHITE);
  }
}
