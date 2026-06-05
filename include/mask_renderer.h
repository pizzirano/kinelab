#pragma once

#include <TFT_eSPI.h>

class MaskRenderer {
public:
    static void draw(TFT_eSPI& tft, int offset);
};