#include <Arduino.h>
#include <TFT_eSPI.h>

#include "image_loader.h"
#include "mask_renderer.h"
#include "kinegram_config.h"

TFT_eSPI tft = TFT_eSPI();

int maskOffset = 0;

void setup()
{
    Serial.begin(115200);

    tft.init();
    tft.setRotation(0);

    Serial.println("KineLab started");
}

void loop()
{
    tft.fillScreen(TFT_WHITE);

    ImageLoader::drawTestPattern(tft);

    MaskRenderer::draw(tft, maskOffset);

    maskOffset++;

    if (maskOffset >= STRIPE_WIDTH * 2)
        maskOffset = 0;

    delay(ANIMATION_SPEED);
}