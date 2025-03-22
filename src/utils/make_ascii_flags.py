#!/usr/bin/env python3
import os
from sys import argv
from PIL import Image

"""
This script converts flag images into ANSI-colored ascii text.
Unzip a set of flag images into this script' current directory.

Flags used: https://flagpedia.net/download/icons
"""
OUT_DIR = "ascii"
BLACKLIST = [argv[0], "flags.zip", OUT_DIR]
ASCII_WIDTH = 24 # Change how you want

def is_gray(r, g, b, threshold=15, brightness=60, max_gray=200):
	# Checks if the pixel is approximately gray and within brightness bounds
	return (
		abs(r - g) < threshold and
		abs(r - b) < threshold and
		abs(g - b) < threshold and
		brightness < r < max_gray
	)

def image_to_unicode_blocks(img_path, width=ASCII_WIDTH):
	from PIL import Image

	img = Image.open(img_path).convert('RGB')
	aspect_ratio = img.height / img.width
	target_height = int(width * aspect_ratio)
	img = img.resize((width, target_height))

	lines = []
	for y in range(0, img.height - 1, 2):
		line = ""
		for x in range(img.width):
			r1, g1, b1 = img.getpixel((x, y))
			r2, g2, b2 = img.getpixel((x, y + 1))

			fg = bg = ""

			if not is_gray(r1, g1, b1):
				fg = f"\x1b[38;2;{r1};{g1};{b1}m"
			if not is_gray(r2, g2, b2):
				bg = f"\x1b[48;2;{r2};{g2};{b2}m"

			if fg or bg:
				line += f"{fg}{bg}▀"
			else:
				line += " "  # Fully gray → print background space

		line += "\x1b[0m"
		lines.append(line)

	return "\n".join(lines)
	
def main():
	if not os.path.exists(OUT_DIR):
		os.makedirs(OUT_DIR, exist_ok=True)

	flags = [(flag.split(".")[0], flag) for flag in os.listdir(".") if flag not in BLACKLIST]

	flags_ascii = [(country_code.upper(), image_to_unicode_blocks(flag)) for country_code, flag in flags]

	for cd, flag in flags_ascii:
		outfile = os.path.join(OUT_DIR, cd + ".txt")
		with open(outfile, "w") as f:
			f.write(flag + '\n')

if __name__ == '__main__':
	main()