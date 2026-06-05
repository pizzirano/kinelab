#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup()
{
    Serial.begin(115200);

    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    tft.init();

    // TESTE: trocamos de 1 para 3
    tft.setRotation(3);

    Serial.println("DISPLAY TEST");

    // Tela vermelha
    tft.fillScreen(TFT_RED);
    delay(1000);

    // Tela verde
    tft.fillScreen(TFT_GREEN);
    delay(1000);

    // Tela azul
    tft.fillScreen(TFT_BLUE);
    delay(1000);

    // Tela preta
    tft.fillScreen(TFT_BLACK);

    // Borda branca
    tft.drawRect(0, 0, tft.width(), tft.height(), TFT_WHITE);

    // Cruz diagonal
    tft.drawLine(0, 0, tft.width(), tft.height(), TFT_YELLOW);
    tft.drawLine(tft.width(), 0, 0, tft.height(), TFT_YELLOW);

    // Texto
    tft.setTextColor(TFT_GREEN);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.println("KINELAB");

    Serial.println("DISPLAY READY");
}

void loop()
{
}