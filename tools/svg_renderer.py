"""Utilities for rendering single SVG files to PNG images."""

import logging
from pathlib import Path

import cairosvg
from PIL import Image

logger = logging.getLogger(__name__)


class SvgRenderError(Exception):
    """Raised when an SVG file cannot be rendered to PNG."""


def render_svg(input_path: Path, output_path: Path, width: int = 240, height: int = 320) -> Image.Image:
    """Render a single SVG file to a PNG file and return the resulting image.

    Args:
        input_path: Path to the source SVG file.
        output_path: Path where the rendered PNG should be written.
        width: Target width in pixels for the rendered image.
        height: Target height in pixels for the rendered image.

    Returns:
        A Pillow Image object opened from the generated PNG file.

    Raises:
        SvgRenderError: If the input path is invalid, the dimensions are invalid,
            or CairoSVG fails to render the SVG.
    """
    if not isinstance(input_path, Path):
        input_path = Path(input_path)
    if not isinstance(output_path, Path):
        output_path = Path(output_path)

    if not input_path.exists():
        raise SvgRenderError(f"Input SVG file does not exist: {input_path}")

    if not input_path.is_file():
        raise SvgRenderError(f"Input path is not a file: {input_path}")

    if width <= 0 or height <= 0:
        raise SvgRenderError(f"Invalid render size: width={width}, height={height}")

    try:
        output_path.parent.mkdir(parents=True, exist_ok=True)
        logger.info("Rendering SVG %s to %s (%dx%d)", input_path, output_path, width, height)
        cairosvg.svg2png(
            url=str(input_path),
            write_to=str(output_path),
            output_width=width,
            output_height=height,
        )

        rendered_image = Image.open(output_path)
        rendered_image.load()
        logger.info("SVG rendered successfully to %s", output_path)
        return rendered_image
    except Exception as exc:
        raise SvgRenderError(f"Failed to render SVG '{input_path}' to '{output_path}': {exc}") from exc
