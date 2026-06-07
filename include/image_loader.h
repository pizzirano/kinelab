#pragma once

#include <FS.h>
#include <TFT_eSPI.h>

class ImageLoader {
public:
    static void drawTestPattern(TFT_eSPI& tft);
    static String findFirstBmp();
    static bool loadSettings(int& stripeWidth, int& animationSpeed, int& frameCount);
    static bool loadBmpToScreen(const char* path, TFT_eSPI& tft);
};