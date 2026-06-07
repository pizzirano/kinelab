#include <Arduino.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>

#include "image_loader.h"
#include "kinegram_config.h"
#include "kinegram_engine.h"

TFT_eSPI tft = TFT_eSPI();
KinegramEngine engine;

namespace {
String currentImage = "";
int stripeWidth = STRIPE_WIDTH;
int animationSpeed = ANIMATION_SPEED;
int frameCount = 1;
KinegramMode currentMode = MODE_AUTO;

void printModeName(KinegramMode mode)
{
    switch (mode) {
        case MODE_PREVIEW: Serial.println("[mode] preview"); break;
        case MODE_MANUAL: Serial.println("[mode] manual"); break;
        case MODE_PHYSICAL: Serial.println("[mode] physical"); break;
        default: Serial.println("[mode] auto"); break;
    }
}

void handleCommand(const String& command)
{
    if (command == "m0") {
        currentMode = MODE_PREVIEW;
        engine.setMode(currentMode);
        printModeName(currentMode);
    } else if (command == "m1") {
        currentMode = MODE_MANUAL;
        engine.setMode(currentMode);
        printModeName(currentMode);
    } else if (command == "m2") {
        currentMode = MODE_PHYSICAL;
        engine.setMode(currentMode);
        printModeName(currentMode);
    } else if (command == "m3") {
        currentMode = MODE_AUTO;
        engine.setMode(currentMode);
        printModeName(currentMode);
    } else if (command.startsWith("o+")) {
        int delta = command.substring(2).toInt();
        engine.setOffset(engine.getOffset() + delta);
        Serial.println("[offset] " + String(engine.getOffset()));
    } else if (command.startsWith("o-")) {
        int delta = command.substring(2).toInt();
        engine.setOffset(engine.getOffset() - delta);
        Serial.println("[offset] " + String(engine.getOffset()));
    } else if (command.startsWith("o=")) {
        engine.setOffset(command.substring(2).toInt());
        Serial.println("[offset] " + String(engine.getOffset()));
    } else if (command == "s?") {
        Serial.println("[settings] stripe=" + String(stripeWidth) + " speed=" + String(animationSpeed) + " frames=" + String(frameCount));
    } else if (command == "i?") {
        Serial.println("[image] " + currentImage);
    }
}
}

void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
    digitalWrite(TFT_BACKLIGHT_PIN, HIGH);

    tft.init();
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    Serial.println("=== kinegram-lab booting ===");

    if (!SPIFFS.begin(true)) {
        Serial.println("[SPIFFS] mount failed");
        return;
    }
    Serial.println("[SPIFFS] mounted OK");

    if (!ImageLoader::loadSettings(stripeWidth, animationSpeed, frameCount)) {
        Serial.println("[settings] using defaults");
    } else {
        Serial.println("[settings] loaded OK");
    }

    engine.begin();
    engine.setMode(currentMode);
    engine.setSpeed(animationSpeed);

    currentImage = ImageLoader::findFirstBmp();
    if (!currentImage.startsWith("/")) { currentImage = "/" + currentImage; }
    if (currentImage.length() == 0) {
        currentImage = "/test_anim.bmp";
    }

    Serial.println("[main] auto-selected image: " + currentImage);
    Serial.println("[Engine] ready");
    Serial.println("=== ready ===");

    tft.fillScreen(TFT_BLACK);
    engine.renderFrame(tft, currentImage.c_str());
}

void loop()
{
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        handleCommand(command);
    }

    engine.update();
    engine.renderFrame(tft, currentImage.c_str());
    delay(16);
}
