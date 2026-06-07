#pragma once

#include <TFT_eSPI.h>

enum KinegramMode {
    MODE_PREVIEW = 0,
    MODE_MANUAL = 1,
    MODE_PHYSICAL = 2,
    MODE_AUTO = 3
};

class KinegramEngine {
public:
    void begin();
    void update();
    void setMode(KinegramMode mode);
    void setOffset(int value);
    void setSpeed(int value);
    int getOffset() const;
    KinegramMode getMode() const;
    void renderFrame(TFT_eSPI& tft, const char* imagePath);

private:
    KinegramMode mode = MODE_AUTO;
    int offset = 0;
    int speed = 40;
    unsigned long lastUpdateMs = 0;
};