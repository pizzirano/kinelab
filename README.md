# KineLab

> Laboratório open source de animação óptica para ESP32. Converte SVG, PNG, GIF e diretórios de frames em BMPs intercalados prontos para exibição em displays TFT via SPIFFS — sem Adobe Illustrator.

---

## Índice

- [Visão Geral](#visão-geral)
- [Pipeline](#pipeline)
- [Técnicas Suportadas](#técnicas-suportadas)
- [Requisitos](#requisitos)
- [Instalação](#instalação)
- [Tutorial: do SVG ao ESP32](#tutorial-do-svg-ao-esp32)
- [Referência de Comandos](#referência-de-comandos)
- [Gravação do ESP32](#gravação-do-esp32)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Tecnologias Utilizadas](#tecnologias-utilizadas)

---

## Visão Geral

O KineLab é um ambiente de experimentação para animações ópticas físicas do tipo kinegram (Barrier Grid Animation).

O projeto resolve o principal gargalo do fluxo tradicional:

```
❌ Antes:  AI → Illustrator → BMP → SPIFFS → ESP32
✅ Agora:  SVG/PNG/GIF → Python → BMP → SPIFFS → ESP32
```

Qualquer artista ou maker pode contribuir usando apenas ferramentas open source.

---

## Pipeline

```
assets/
  input.svg          ← SVG único
  input.gif          ← GIF animado
  frames/            ← diretório com frame_000.svg ... frame_N.svg
      │
      ▼
tools/build_kinegram.py
      │
      ├── svg_renderer.py     cairosvg → PNG por frame
      ├── frame_generator.py  carrega e normaliza frames
      ├── interlacer.py       intercala N frames em stripe columns
      └── bmp_exporter.py     exporta 24-bit BMP sem compressão
      │
      ▼
data/input.bmp        ← pronto para SPIFFS
      │
      ▼
just uploadfs         ← sobe para o ESP32
      │
      ▼
TFT 320×240 + máscara física → animação kinegram
```

---

## Técnicas Suportadas

| Técnica | Status |
|---------|--------|
| Kinegrams (Barrier Grid) | ✅ Implementado |
| Scanimation | ✅ Implementado |
| Interlacing vertical | ✅ Implementado |
| Interlacing horizontal | ✅ Implementado |
| N frames (2, 3, 4...) | ✅ Implementado |
| Efeitos Moiré | 🔜 Planejado |
| Lenticular (simulação) | 🔜 Planejado |
| GIF como fonte de frames | ✅ Implementado |
| Lottie como fonte | 🔜 Planejado |

---

## Requisitos

### Software

| Ferramenta | Versão mínima | Instalação |
|------------|---------------|------------|
| Python | 3.10+ | [python.org](https://python.org) |
| PlatformIO | qualquer | via script abaixo |
| Just | qualquer | via script abaixo |
| cairosvg | 2.x | `pip install cairosvg` |
| Pillow | 10.x | `pip install pillow` |

**Cairo nativo obrigatório para cairosvg:**

```bash
# Ubuntu / Debian / Codespaces
sudo apt install libcairo2-dev pkg-config python3-dev

# macOS
brew install cairo pango

# Windows → usar WSL
```

### Hardware

- ESP32 DevKit (qualquer variante com 4 MB de flash)
- Display TFT compatível com TFT_eSPI (ILI9341, ST7789, ILI9488)
- Cabo USB

---

## Instalação

### 1. Clonar o repositório

```bash
git clone https://github.com/seu-usuario/kinelab.git
cd kinelab
```

### 2. Instalar dependências Python

```bash
pip install cairosvg pillow
```

Verificar:

```bash
python - -c "import cairosvg, PIL; print('OK')"
```

### 3. Criar o `__init__.py` (obrigatório)

```bash
touch tools/__init__.py
```

Sem esse arquivo os imports entre módulos quebram com `ModuleNotFoundError`.

### 4. Criar pastas de trabalho

```bash
mkdir -p assets data temp
```

### 5. Instalar Just

```bash
curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | \
bash -s -- --to ~/.local/bin
```

### 6. Configurar PATH

```bash
echo 'export PATH="$HOME/.local/bin:$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### 7. Instalar PlatformIO

```bash
curl -fsSL \
  https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py \
  -o get-platformio.py
python3 get-platformio.py
```

### 8. Verificar tudo

```bash
python --version        # 3.10+
pio --version
just --version
```

---

## Tutorial: do SVG ao ESP32

### Passo 1 — Criar um SVG de teste

Salve como `assets/test_circle.svg`:

```xml
<svg xmlns="http://www.w3.org/2000/svg" width="240" height="320" viewBox="0 0 240 320">
  <rect width="240" height="320" fill="#1a1a2e"/>
  <circle cx="120" cy="160" r="80" fill="#e94560"/>
  <text x="120" y="168" text-anchor="middle" fill="white" font-size="24">frame A</text>
</svg>
```

### Passo 2 — Rodar o pipeline

```bash
python tools/build_kinegram.py assets/test_circle.svg
```

Saída esperada:

```
✓ data/test_circle.bmp (240×320, 1 frames, stripe=4)
```

### Passo 3 — Kinegram real com 2 frames

Crie um segundo SVG em `assets/test_b.svg` (mesma estrutura, imagem diferente) e um diretório:

```bash
mkdir -p assets/minha_anim
cp assets/test_circle.svg assets/minha_anim/frame_000.svg
# copie ou crie frame_001.svg com a segunda imagem
```

Rode com parâmetros de interlacing:

```bash
python tools/build_kinegram.py assets/minha_anim/ \
  --stripe-width 4 \
  --output data/minha_anim.bmp
```

### Passo 4 — Testar com GIF

```bash
python tools/build_kinegram.py assets/animacao.gif \
  --stripe-width 4 \
  --verbose
```

### Passo 5 — Validar o BMP antes de subir

```bash
python - <<'EOF'
from PIL import Image
from pathlib import Path

for bmp in Path("data").glob("*.bmp"):
    img = Image.open(bmp)
    size_kb = bmp.stat().st_size / 1024
    print(f"{bmp.name}: {img.size} {img.mode} {size_kb:.1f} KB")
    assert img.mode == "RGB"
    assert img.size == (240, 320)

print("Todos os BMPs prontos para ESP32")
EOF
```

### Passo 6 — Subir para o ESP32

```bash
just uploadfs
```

### Passo 7 — Compilar e gravar o firmware

```bash
just build
just upload
just monitor
```

Ou tudo de uma vez:

```bash
just all
```

---

## Referência de Comandos

### Pipeline Python

| Comando | Descrição |
|---------|-----------|
| `python tools/build_kinegram.py <source>` | Roda o pipeline completo |
| `--output <path>` | Destino do BMP (padrão: `data/<stem>.bmp`) |
| `--width <n>` | Largura em px (padrão: 240) |
| `--height <n>` | Altura em px (padrão: 320) |
| `--stripe-width <n>` | Largura de cada stripe (padrão: 4) |
| `--orientation vertical\|horizontal` | Direção do interlacing |
| `--frames <n>` | Limita o número de frames usados |
| `--temp-dir <path>` | Diretório para frames intermediários |
| `--verbose` | Ativa log DEBUG |

**Exemplos:**

```bash
# SVG único
python tools/build_kinegram.py assets/roda.svg

# Diretório de frames SVG, 3 frames, stripe de 3px
python tools/build_kinegram.py assets/wheel/ --stripe-width 3 --frames 3

# GIF com interlacing horizontal
python tools/build_kinegram.py assets/anim.gif --orientation horizontal

# Resolução customizada com log completo
python tools/build_kinegram.py assets/roda.svg --width 320 --height 240 --verbose
```

### Just (automação)

| Comando | Descrição |
|---------|-----------|
| `just build` | Compila o firmware |
| `just upload` | Grava o firmware no ESP32 |
| `just uploadfs` | Sobe os arquivos SPIFFS (pasta `data/`) |
| `just monitor` | Abre o monitor serial (115200 baud) |
| `just buildfs` | Gera `spiffs.bin` sem subir |
| `just all` | `clean` → `build` → `upload` |
| `just clean` | Remove build e arquivos temporários |
| `just build-kinegram <arquivo>` | Atalho para o pipeline Python |
| `just status` | Status do projeto |

---

## Gravação do ESP32

### Via Just (recomendado)

```bash
just upload
just uploadfs
```

### Via ESP Web Flasher

Acesse [espressif.github.io/esptool-js](https://espressif.github.io/esptool-js) e grave os arquivos na ordem:

| Endereço | Arquivo |
|----------|---------|
| `0x1000` | `bootloader.bin` |
| `0x8000` | `partitions.bin` |
| `0x10000` | `firmware.bin` |
| `0x290000` | `spiffs.bin` |

### Configurações recomendadas

| Parâmetro | Valor |
|-----------|-------|
| Flash Size | 4 MB |
| Flash Mode | DIO |
| Flash Frequency | 40 MHz |
| Baud Rate | 115200 |

### Entrar em modo de programação

Se o upload falhar:

1. Pressione e segure **BOOT**
2. Pressione **EN**
3. Solte **EN**
4. Solte **BOOT**
5. Inicie a gravação

---

## Estrutura do Projeto

```
KineLab/
│
├── assets/                    ← arquivos de entrada (SVGs, GIFs, PNGs)
│   └── wheel/
│       ├── frame_000.svg
│       ├── frame_001.svg
│       └── frame_002.svg
│
├── data/                      ← BMPs gerados → upload para SPIFFS
│   └── *.bmp
│
├── temp/                      ← frames intermediários (gerado automaticamente)
│
├── src/                       ← firmware ESP32
│   ├── main.cpp
│   ├── kinegram_engine.cpp
│   ├── mask_renderer.cpp
│   └── image_loader.cpp
│
├── include/                   ← headers do firmware
│   ├── kinegram_config.h
│   ├── kinegram_engine.h
│   ├── image_loader.h
│   └── mask_renderer.h
│
├── tools/                     ← pipeline Python
│   ├── __init__.py            ← obrigatório
│   ├── build_kinegram.py      ← CLI principal
│   ├── svg_renderer.py        ← cairosvg → PNG
│   ├── frame_generator.py     ← carrega frames de qualquer fonte
│   ├── interlacer.py          ← algoritmo de interlacing
│   └── bmp_exporter.py        ← exporta BMP 24-bit para ESP32
│
├── docs/
│
├── platformio.ini
├── justfile
└── README.md
```

---

## Fluxo de Desenvolvimento

```
Criar ou editar SVG/GIF em assets/
          │
          ▼
just build-kinegram assets/input.svg
          │
          ▼
just uploadfs          ← sobe data/*.bmp para SPIFFS
          │
          ▼
just monitor           ← verifica Serial
          │
          ▼
Testar no Display TFT + máscara física
```

---

## Tecnologias Utilizadas

- **ESP32** — microcontrolador principal
- **TFT_eSPI** — driver de display
- **SPIFFS / LittleFS** — sistema de arquivos embarcado
- **PlatformIO** — build system para firmware
- **Python 3** — pipeline de geração de BMPs
- **cairosvg** — renderização de SVG
- **Pillow** — manipulação de imagens
- **Just** — automação de tarefas
- **GitHub Codespaces** — ambiente de desenvolvimento na nuvem