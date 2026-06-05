#include <Arduino.h>
#include <TFT_eSPI.h>

#include "mask_renderer.h"
#include "kinegram_config.h"

void MaskRenderer::draw(TFT_eSPI& tft, int offset)
{
    for (int x = offset; x < SCREEN_WIDTH; x += STRIPE_WIDTH * 2)
    {
        tft.drawFastVLine(
            x,
            0,
            SCREEN_HEIGHT,
            TFT_BLACK
        );
    }
}