#include <ctype.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>

#include "image_loader.h"
#include "kinegram_config.h"

namespace {
int readIntValue(const String& text, const char* key) {
    const String needle = String(key);
    int pos = text.indexOf(needle);
    if (pos < 0) {
        return -1;
    }

    pos = text.indexOf(':', pos);
    if (pos < 0) {
        return -1;
    }

    ++pos;
    while (pos < text.length() && (text[pos] == ' ' || text[pos] == '\t' || text[pos] == '\n' || text[pos] == '\r')) {
        ++pos;
    }

    int sign = 1;
    if (text[pos] == '-') {
        sign = -1;
        ++pos;
    }

    int value = 0;
    bool hasDigits = false;
    while (pos < text.length() && isdigit(static_cast<unsigned char>(text[pos]))) {
        value = value * 10 + (text[pos] - '0');
        hasDigits = true;
        ++pos;
    }

    return hasDigits ? value * sign : -1;
}
}

void ImageLoader::drawTestPattern(TFT_eSPI& tft)
{
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        uint16_t color = ((x / STRIPE_WIDTH) % 2 == 0) ? TFT_RED : TFT_BLUE;
        tft.drawFastVLine(x, 0, SCREEN_HEIGHT, color);
    }
}

String ImageLoader::findFirstBmp()
{
    if (!SPIFFS.begin(true)) {
        return "";
    }

    File root = SPIFFS.open(BMP_ROOT_PATH);
    if (!root || !root.isDirectory()) {
        return "";
    }

    File entry = root.openNextFile();
    while (entry) {
        String name = String(entry.name());
        if (name.endsWith(BMP_FILE_EXT) || name.endsWith(".BMP")) {
            entry.close();
            root.close();
            return name;
        }
        entry = root.openNextFile();
    }

    root.close();
    return "";
}

bool ImageLoader::loadSettings(int& stripeWidth, int& animationSpeed, int& frameCount)
{
    File file = SPIFFS.open(SETTINGS_PATH, "r");
    if (!file) {
        stripeWidth = STRIPE_WIDTH;
        animationSpeed = ANIMATION_SPEED;
        frameCount = 1;
        return false;
    }

    String text = file.readString();
    file.close();

    int value = readIntValue(text, "stripe_width");
    if (value > 0) {
        stripeWidth = value;
    }
    value = readIntValue(text, "animation_speed");
    if (value > 0) {
        animationSpeed = value;
    }
    value = readIntValue(text, "frame_count");
    if (value > 0) {
        frameCount = value;
    }

    return true;
}

bool ImageLoader::loadBmpToScreen(const char* path, TFT_eSPI& tft)
{
    File file = SPIFFS.open(path, "r");
    if (!file) {
        return false;
    }

    uint8_t header[54];
    if (file.read(header, 54) != 54) {
        file.close();
        return false;
    }

    const uint32_t dataOffset = header[10] | (header[11] << 8) | (header[12] << 16) | (header[13] << 24);
    const uint32_t width = header[18] | (header[19] << 8) | (header[20] << 16) | (header[21] << 24);
    const uint32_t height = header[22] | (header[23] << 8) | (header[24] << 16) | (header[25] << 24);

    file.seek(dataOffset);

    tft.fillScreen(TFT_BLACK);
    for (uint32_t y = 0; y < height && y < SCREEN_HEIGHT; ++y) {
        for (uint32_t x = 0; x < width && x < SCREEN_WIDTH; ++x) {
            uint8_t b = file.read();
            uint8_t g = file.read();
            uint8_t r = file.read();
            tft.drawPixel(x, y, tft.color565(r, g, b));
        }
        if ((width * 3) % 4 != 0) {
            file.seek(file.position() + (4 - ((width * 3) % 4)) % 4);
        }
    }

    file.close();
    return true;
}