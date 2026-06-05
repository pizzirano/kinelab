from PIL import Image

WIDTH = 240
HEIGHT = 240

img = Image.new("RGB", (WIDTH, HEIGHT))

for x in range(WIDTH):
    for y in range(HEIGHT):

        if (x // 4) % 2 == 0:
            img.putpixel((x, y), (255, 0, 0))
        else:
            img.putpixel((x, y), (0, 0, 255))

img.save("test_pattern.bmp")

print("BMP gerado")