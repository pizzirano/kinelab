#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int x = 20;
int dir = 1;

void setup()
{
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    tft.init();
    tft.setRotation(3);
}

void loop()
{
    tft.fillScreen(TFT_BLACK);

    tft.fillCircle(x, 120, 20, TFT_RED);

    x += dir * 8;

    if (x > 220)
        dir = -1;

    if (x < 20)
        dir = 1;

    delay(30);
}