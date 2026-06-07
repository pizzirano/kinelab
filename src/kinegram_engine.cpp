#include "kinegram_engine.h"

#include <Arduino.h>

#include "image_loader.h"
#include "kinegram_config.h"
#include "mask_renderer.h"

void KinegramEngine::begin()
{
    offset = 0;
    lastUpdateMs = millis();
}

void KinegramEngine::update()
{
    if (millis() - lastUpdateMs < static_cast<unsigned long>(speed)) {
        return;
    }

    lastUpdateMs = millis();
    offset = (offset + 1) % (STRIPE_WIDTH * 2);
}

void KinegramEngine::setMode(KinegramMode mode)
{
    this->mode = mode;
}

void KinegramEngine::setOffset(int value)
{
    offset = value;
}

void KinegramEngine::setSpeed(int value)
{
    speed = (value > 0) ? value : 1;
}

int KinegramEngine::getOffset() const
{
    return offset;
}

KinegramMode KinegramEngine::getMode() const
{
    return mode;
}

void KinegramEngine::renderFrame(TFT_eSPI& tft, const char* imagePath)
{
    if (!imagePath || imagePath[0] == '\0') {
        ImageLoader::drawTestPattern(tft);
        return;
    }

    if (!ImageLoader::loadBmpToScreen(imagePath, tft)) {
        ImageLoader::drawTestPattern(tft);
    }

    MaskRenderer::draw(tft, offset);
}