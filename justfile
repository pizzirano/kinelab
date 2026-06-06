# =============================================================================
# KINELAB - COMANDOS DE DESENVOLVIMENTO
# =============================================================================

set shell := ["powershell.exe", "-NoProfile", "-Command"]

# Lista todos os comandos disponíveis
default:
    @just --list

# =============================================================================
# INSTALAÇÃO
# =============================================================================

# Instala o PlatformIO Core
install-platformio:
    Invoke-WebRequest -Uri "https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py" -OutFile "get-platformio.py"
    python get-platformio.py

# Verifica se o PlatformIO está instalado
check-platformio:
    pio --version

# =============================================================================
# BUILD E FLASH
# =============================================================================

# Limpa artefatos de compilação
clean:
    pio run -t clean

# Compila o firmware
build:
    pio run

# Compila sistema de arquivos
buildfs:
    pio run -t buildfs

# Envia firmware para o ESP32
upload:
    pio run -t upload

# Envia sistema de arquivos para o ESP32
uploadfs:
    pio run -t uploadfs

# Monitor serial
monitor:
    pio device monitor -b 115200

# Executa fluxo completo
all:
    just clean
    just build
    just buildfs
    just upload

# =============================================================================
# ESP WEB FLASHER
# =============================================================================

# Instruções para gravação via navegador
help-flasher:
    Write-Host ""
    Write-Host "=== ESP WEB FLASHER ==="
    Write-Host ""
    Write-Host "https://espressif.github.io/esptool-js/"
    Write-Host ""
    Write-Host "0x1000   -> bootloader.bin"
    Write-Host "0x8000   -> partitions.bin"
    Write-Host "0x10000  -> firmware.bin"
    Write-Host "0x290000 -> spiffs.bin"
    Write-Host ""
    Write-Host "Flash Size: 4MB"
    Write-Host "Baud: 115200"

# =============================================================================
# STATUS
# =============================================================================

status:
    Write-Host ""
    Write-Host "================================="
    Write-Host "KineLab - Status"
    Write-Host "================================="
    Write-Host ""

    pio --version

    Write-Host ""
    Write-Host "Arquivos do projeto:"
    Get-ChildItem