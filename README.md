# KineLab

> Laboratório de animação óptica para experimentação com técnicas de ilusão de movimento utilizando ESP32, displays TFT e padrões visuais interativos.

---

## Índice

- [Visão Geral](#visão-geral)
- [Técnicas Suportadas](#técnicas-suportadas)
- [Requisitos](#requisitos)
- [Instalação no GitHub Codespaces](#instalação-no-github-codespaces)
- [Comandos Disponíveis](#comandos-disponíveis)
- [Gravação do ESP32](#gravação-do-esp32)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Tecnologias Utilizadas](#tecnologias-utilizadas)

---

# Visão Geral

O KineLab é um ambiente de experimentação voltado para animações ópticas e técnicas de ilusão de movimento.

O projeto permite gerar e exibir padrões animados utilizando:

- ESP32
- Displays TFT
- Arquivos SPIFFS
- Técnicas ópticas tradicionais

---

# Técnicas Suportadas

## Cinegramas (Kinegrams)

Animações produzidas através do movimento de uma grade sobre uma imagem intercalada.

## Scanimation

Sequências de quadros ocultas por uma máscara móvel.

## Barrier Grid Animation

Animações baseadas em grades de barreira.

## Efeitos Moiré

Padrões visuais produzidos pela sobreposição de grades.

## Pré-visualizações Lenticulares

Simulação digital de imagens lenticulares.

## Displays TFT com ESP32

Renderização direta em displays embarcados.

---

# Requisitos

## Software

- Git
- Python 3
- PlatformIO
- Just

## Hardware

- ESP32
- Display TFT compatível
- Cabo USB

---

# Instalação no GitHub Codespaces

## Instalar Just

```bash
curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | \
bash -s -- --to ~/.local/bin
```

## Configurar PATH

```bash
echo 'export PATH="$HOME/.local/bin:$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

## Instalar PlatformIO

```bash
curl -fsSL \
https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py \
-o get-platformio.py

python3 get-platformio.py
```

## Verificar Instalação

```bash
pio --version
just --version
```

---

# Comandos Disponíveis

## Compilação

### Build do Firmware

```bash
just build
```

Compila o firmware principal.

### Build do SPIFFS

```bash
just buildfs
```

Gera o arquivo:

```text
spiffs.bin
```

a partir do conteúdo da pasta:

```text
data/
```

---

## Upload

### Upload do Firmware

```bash
just upload
```

### Upload do SPIFFS

```bash
just uploadfs
```

### Upload Completo

```bash
just all
```

Executa:

```text
clean
↓
build
↓
upload
```

---

## Monitoramento

### Serial Monitor

```bash
just monitor
```

---

## Utilitários

### Limpeza

```bash
just clean
```

### Status do Projeto

```bash
just status
```

### Ajuda do Web Flasher

```bash
just help-flasher
```

---

# Gravação do ESP32

## ESP Web Flasher

Acesse:

https://espressif.github.io/esptool-js/

---

## Arquivos de Flash

| Endereço | Arquivo |
|-----------|----------|
| `0x1000` | `bootloader.bin` |
| `0x8000` | `partitions.bin` |
| `0x10000` | `firmware.bin` |
| `0x290000` | `spiffs.bin` |

---

## Configurações Recomendadas

| Configuração | Valor |
|-------------|--------|
| Flash Size | 4MB |
| Flash Mode | DIO |
| Flash Frequency | 40 MHz |
| Baud Rate | 115200 |

---

## Entrar em Modo de Programação

Caso o upload falhe:

1. Pressione e segure **BOOT**.
2. Pressione **EN**.
3. Solte **EN**.
4. Solte **BOOT**.
5. Inicie a gravação.

---

# Estrutura do Projeto

```text
KineLab/
├── data/
│   ├── images/
│   └── animations/
│
├── src/
│   ├── main.cpp
│   └── display/
│
├── include/
│
├── docs/
│
├── platformio.ini
├── justfile
└── README.md
```

---

# Tecnologias Utilizadas

- ESP32
- PlatformIO
- SPIFFS
- TFT_eSPI
- GitHub Codespaces
- Just

---

# Fluxo de Desenvolvimento

```text
Editar Código
      ↓
just build
      ↓
just upload
      ↓
just monitor
      ↓
Testar no Display
```


