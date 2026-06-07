# =============================================================================
# KINELAB - COMANDOS DE DESENVOLVIMENTO
# =============================================================================

set shell := ["sh", "-cu"]

# Lista todos os comandos disponíveis.
default:
    @just --list

# =============================================================================
# INSTALAÇÃO
# =============================================================================

# Instala o PlatformIO Core localmente.
install-platformio:
    curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -o get-platformio.py
    python get-platformio.py

# Verifica se o PlatformIO está instalado.
check-platformio:
    pio --version

# =============================================================================
# BUILD E FLASH
# =============================================================================

# Limpa artefatos de compilação e arquivos temporários.
clean:
    pio run -t clean
    rm -rf temp
    rm -f data/*.bmp

# Compila o firmware principal.
build:
    pio run

# Compila o sistema de arquivos SPIFFS.
buildfs:
    pio run -t buildfs

# Envia o firmware para o ESP32.
upload:
    pio run --target upload

# Envia o sistema de arquivos para o ESP32.
uploadfs:
    pio run --target uploadfs

# Abre o monitor serial com baud rate 115200.
monitor:
    pio device monitor --baud 115200

# Gera um kinegram a partir de um arquivo SVG/PNG/GIF.
build-kinegram FILE="assets/input.svg":
    python tools/build_kinegram.py "{{FILE}}"

# Gera o BMP, envia o SPIFFS e abre o monitor serial.
test-image FILE="assets/input.svg":
    just build-kinegram "{{FILE}}"
    just uploadfs
    just monitor

# Executa o fluxo completo de build e upload.
all:
    just clean
    just build
    just buildfs
    just upload

# =============================================================================
# ESP WEB FLASHER
# =============================================================================

# Instruções para gravação via navegador.
help-flasher:
    echo ""
    echo "=== ESP WEB FLASHER ==="
    echo ""
    echo "https://espressif.github.io/esptool-js/"
    echo ""
    echo "0x1000   -> bootloader.bin"
    echo "0x8000   -> partitions.bin"
    echo "0x10000  -> firmware.bin"
    echo "0x290000 -> spiffs.bin"
    echo ""
    echo "Flash Size: 4MB"
    echo "Baud: 115200"

# =============================================================================
# STATUS
# =============================================================================

# Mostra status básico do ambiente e estrutura do projeto.
status:
    echo ""
    echo "================================="
    echo "KineLab - Status"
    echo "================================="
    echo ""

    pio --version

    echo ""
    echo "Arquivos do projeto:"
    find . -maxdepth 2 -mindepth 1 | sort