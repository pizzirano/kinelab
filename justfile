# KineLab - Justfile

# ====================== INSTALAÇÃO ======================
# Instalar PlatformIO (rode apenas uma vez)
install-platformio:
    curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -o get-platformio.py
    python3 get-platformio.py

# Configurar PATH do PlatformIO
setup-path:
    echo 'export PATH="$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
    source ~/.bashrc
    echo "✅ PlatformIO configurado!"

# ====================== COMANDOS BÁSICOS ======================

# Limpar build
clean:
    pio run -t clean

# Build completo (firmware + spiffs)
build:
    pio run

# Build apenas do sistema de arquivos (quando altera imagens na pasta data/)
buildfs:
    pio run -t buildfs

# Upload completo para o ESP32
upload:
    pio run -t upload

# Upload apenas do filesystem (mais rápido)
uploadfs:
    pio run -t uploadfs

# Monitor Serial
monitor:
    pio device monitor -b 115200

# Limpar + Build completo + Upload
all: clean build buildfs upload

# ====================== ESP WEB FLASHER ======================

# Mensagem de ajuda para Web Flasher
help-flasher:
    @echo "=== ESP Web Flasher ==="
    @echo "1. Abra: https://espressif.github.io/esptool-js/"
    @echo "2. Conecte o ESP32"
    @echo "3. Configure 4 linhas:"
    @echo "   0x1000     → bootloader.bin"
    @echo "   0x8000     → partitions.bin"
    @echo "   0x10000    → firmware.bin"
    @echo "   0x290000   → spiffs.bin"
    @echo "4. Flash Size = 4MB | Baud = 115200"
    @echo "5. Segure BOOT + aperte EN antes de Program"

# ====================== STATUS ======================
status:
    @echo "KineLab - Status do Projeto"
    pio run --target check