# KineLab

Laboratório de animação óptica para experimentação com:
- Cinegramas
- Scanimation
- Animação de grade de barreira
- Efeitos Moiré
- Pré-visualizações lenticulares
- Displays TFT ESP32

## Como usar no Codespaces / GitHub

Instale o Just manualmente
curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to ~/.local/bin

# Atualize o PATH
echo 'export PATH="$HOME/.local/bin:$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc

# Verifique se instalou
just --version

# Instalar PlatformIO (uma vez)
install-platformio:
    curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -o get-platformio.py
    python3 get-platformio.py

# Configurar PATH (PlatformIO + Just)
setup:
    @echo 'export PATH="$HOME/.local/bin:$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
    source ~/.bashrc
    @echo "Caminhos configurados!"

# ====================== COMANDOS PRINCIPAIS ======================

# Limpar build anterior
clean:
    pio run -t clean

# Build completo do firmware
build:
    pio run

# Build apenas do sistema de arquivos (imagens BMP)
buildfs:
    pio run -t buildfs

# Upload firmware + spiffs
upload:
    pio run -t upload

# Upload apenas do filesystem (rápido)
uploadfs:
    pio run -t uploadfs

# Monitor Serial
monitor:
    pio device monitor -b 115200

# Tudo de uma vez (limpar + build + upload)
all: clean build buildfs upload

# ====================== AJUDA ======================

# Mostrar ajuda do Web Flasher
help-flasher:
    @echo "=== ESP Web Flasher ==="
    @echo "1. Abra → https://espressif.github.io/esptool-js/"
    @echo "2. Connect no ESP32"
    @echo "3. Configure as 4 linhas:"
    @echo "   0x1000     → bootloader.bin"
    @echo "   0x8000     → partitions.bin"
    @echo "   0x10000    → firmware.bin"
    @echo "   0x290000   → spiffs.bin"
    @echo "4. Flash Size = 4MB | Baud Rate = 115200"

# Status do projeto
status:
    pio run --target check