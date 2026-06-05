# KineLab

Optical animation laboratory for experimenting with:

* Kinegrams
* Scanimation
* Barrier-grid animation
* Moiré effects
* Lenticular previews
* ESP32 TFT displays

The goal of this project is to create a reusable playground for studying how optical animations work before moving to physical media such as lenticular sheets, microtube viewers, postcards, and future integrations with Giph5Lens.

---

## Features

Current development goals:

* ESP32 + TFT display support
* Virtual scanimation mask renderer
* Kinegram preview mode
* Physical mask testing mode
* Frame interlacing experiments
* Optical calibration utilities
* BMP asset loading
* SD card support (future)

---

## Project Structure

```text
kinelab/
│
├── platformio.ini
│
├── include/
│   ├── kinegram_config.h
│   ├── kinegram_engine.h
│   ├── image_loader.h
│   └── mask_renderer.h
│
├── src/
│   ├── main.cpp
│   ├── kinegram_engine.cpp
│   ├── image_loader.cpp
│   └── mask_renderer.cpp
│
├── data/
│   └── config/
│       └── settings.json
│
└── scripts/
    └── generate_test_bmps.py
```

---

## Requirements

### Hardware

* ESP32 development board
* TFT display (ST7789 or compatible)
* USB cable

### Software

* PlatformIO
* VS Code (recommended)
* Python 3.x

---

## Build

Compile firmware:

```bash
pio run
```

Expected output:

```text
Successfully created esp32 image.
```

Generated firmware:

```text
.pio/build/esp32dev/firmware.bin
```

---

## Upload to ESP32

### Option 1 — PlatformIO

```bash
pio run -t upload
```

### Option 2 — ESP Flash Download Tool

Flash addresses:

```text
bootloader.bin  -> 0x1000
partitions.bin  -> 0x8000
firmware.bin    -> 0x10000
```

Uploading a new firmware replaces any existing firmware on the device (including Marauder).

---

## Current Prototype

Current firmware renders:

* test stripe pattern
* virtual moving mask
* basic kinegram simulation

This allows rapid testing of:

* stripe width
* mask offset
* animation speed
* interlacing concepts

before printing physical scanimation masks.

---

## Roadmap

### Phase 1

* Virtual mask simulation
* BMP rendering
* Configurable stripe width

### Phase 2

* SD card asset loading
* Multiple animation sets
* Runtime settings

### Phase 3

* Physical scanimation calibration
* Lenticular preview mode
* Interlacing generator

### Phase 4

* Giph5Lens integration
* Cartridge/refill system
* Microtube optical viewer

### COMMANDS

*pio device list
## Verifica conexão com esp
pio device monitor -p COM6 -b 115200

## limpar tudo 
pio run -t clean

## recompilar 
pio run

## Gravar na tela 
pio run -t upload --upload-port COM6

---

## Vision

KineLab is intended to become the experimental optical engine behind future Giph5Lens projects, enabling rapid prototyping of optical animation techniques before physical production.
