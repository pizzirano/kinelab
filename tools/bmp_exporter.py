"""Export Pillow images to 24-bit BMP files for ESP32/TFT_eSPI workflows."""

import logging
from pathlib import Path

from PIL import Image

logger = logging.getLogger(__name__)


class BmpExportError(Exception):
    """Raised when a Pillow image cannot be exported to a BMP file."""


def export_bmp(image: Image.Image, output_path: Path, width: int = 240, height: int = 320) -> Path:
    """Export a Pillow image to a 24-bit BMP file.

    Args:
        image: Source image to export.
        output_path: Destination BMP file path.
        width: Desired width in pixels for the exported image.
        height: Desired height in pixels for the exported image.

    Returns:
        The output path as provided, after a successful export.

    Raises:
        BmpExportError: If the image cannot be resized, saved, or verified.
    """
    if not isinstance(output_path, Path):
        output_path = Path(output_path)

    if width <= 0 or height <= 0:
        raise BmpExportError(f"Invalid export size: width={width}, height={height}")

    try:
        working_image = image.convert("RGB") if image.mode != "RGB" else image.copy()
        if working_image.size != (width, height):
            working_image = working_image.resize((width, height), Image.LANCZOS)

        output_path.parent.mkdir(parents=True, exist_ok=True)
        logger.debug("Exporting BMP to %s at %dx%d", output_path, width, height)

        working_image.save(output_path, format="BMP")

        if not output_path.exists() or output_path.stat().st_size <= 0:
            raise BmpExportError(f"BMP export did not produce a valid file: {output_path}")

        logger.info("BMP exported successfully to %s", output_path)
        return output_path
    except Exception as exc:
        raise BmpExportError(f"Failed to export BMP to '{output_path}': {exc}") from exc
