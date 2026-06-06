# KineLab

**Laboratório de animação óptica** para experimentação com técnicas de ilusão de movimento.

### Técnicas suportadas
- Cinegramas (Kinegrams)
- Scanimation
- Animação de grade de barreira
- Efeitos Moiré
- Pré-visualizações lenticulares
- Displays TFT com ESP32

---

## Como usar no GitHub Codespaces

### 1. Instalação Inicial (executar uma única vez)

```bash
# Instalar Just
curl --proto '=https' --tlsv1.2 -sSf https://just.systems/install.sh | bash -s -- --to ~/.local/bin

# Configurar PATH (Just + PlatformIO)
echo 'export PATH="$HOME/.local/bin:$HOME/.platformio/penv/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc

# Instalar PlatformIO
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -o get-platformio.py
python3 get-platformio.py

2. Comandos Principais (com Just)

Comando,Descrição
just build,Compilar o firmware
just buildfs,Gerar spiffs.bin (imagens da pasta data/)
just upload,Gravar firmware no ESP32
just uploadfs,Gravar apenas o sistema de arquivos
just monitor,Abrir Monitor Serial
just clean,Limpar build anterior
just all,Limpar + Build + Upload (completo)
just help-flasher,Mostrar guia do ESP Web Flasher
just status,Verificar estado do projeto

Gravar no ESP32 usando ESP Web Flasher

Acesse: https://espressif.github.io/esptool-js/
Clique em Connect e selecione a porta do seu ESP32.
Configure as 4 linhas exatamente assim:




































LinhaEndereçoArquivoObservação10x1000bootloader.binBootloader20x8000partitions.binTabela de partições30x10000firmware.binFirmware principal40x290000spiffs.binArquivos da pasta data/
Configurações recomendadas:

Flash Size: 4MB
Flash Mode: dio
Flash Frequency: 40m
Baud Rate: 115200

Dica: Segure o botão BOOT + pressione EN antes de clicar em Program.
