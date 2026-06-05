#include <TFT_eSPI.h>

#include "image_loader.h"
#include "kinegram_config.h"

void ImageLoader::drawTestPattern(TFT_eSPI& tft)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        uint16_t color;

        if ((x / 4) % 2 == 0)
            color = TFT_RED;
        else
            color = TFT_BLUE;

        tft.drawFastVLine(
            x,
            0,
            SCREEN_HEIGHT,
            color
        );
    }
}