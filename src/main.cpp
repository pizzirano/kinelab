#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int x = 30;
int dir = 1;

void setup()
{
    Serial.begin(115200);
    delay(500);
    
    Serial.println("\n\n=== KineLab Iniciado ===");

    // Backlight da placa CYD
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    Serial.println("Backlight ligado (pino 21)");

    // Inicializa a tela
    tft.init();
    
    // === TESTE DE ROTAÇÃO - Mude o número e teste uma por uma ===
    tft.setRotation(1);        // ← Teste 0, 1, 2 ou 3

    tft.fillScreen(TFT_BLACK);
    
    Serial.println("TFT OK - Rotação: " + String(tft.getRotation()));
    Serial.println("Resolução: " + String(tft.width()) + "x" + String(tft.height()));
}

void loop()
{
    tft.fillScreen(TFT_BLACK);

    // Desenho de teste
    tft.fillCircle(x, 160, 35, TFT_BLUE);
    tft.drawString("Teste CYD", 40, 80, 4);
    tft.drawString("Rotacao: " + String(tft.getRotation()), 40, 220, 2);

    // Movimento do círculo
    x += dir * 10;
    if (x > 200) dir = -1;
    if (x < 40)  dir = 1;

    delay(40);
}