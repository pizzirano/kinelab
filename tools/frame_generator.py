"""Utilities for loading a source image or SVG collection into Pillow frames."""

import logging
from pathlib import Path
from typing import List

from PIL import Image

from tools.svg_renderer import SvgRenderError, render_svg

logger = logging.getLogger(__name__)


class FrameLoadError(Exception):
    """Raised when frames cannot be loaded from the requested source."""


def _resize_frame(frame: Image.Image, width: int, height: int) -> Image.Image:
    """Resize a frame to the requested dimensions and convert it to RGB."""
    resized_frame = frame.resize((width, height), Image.LANCZOS)
    return resized_frame.convert("RGB")


def _load_single_frame(path: Path, width: int, height: int) -> Image.Image:
    """Load one SVG or PNG file into a normalized Pillow frame."""
    suffix = path.suffix.lower()
    if suffix == ".svg":
        temp_output = path.with_name(f".{path.stem}.tmp-render.png")
        try:
            frame = render_svg(path, temp_output, width=width, height=height)
            frame.load()
            logger.debug("Loaded frame from %s", path)
            return _resize_frame(frame, width, height)
        except SvgRenderError as exc:
            raise FrameLoadError(f"Failed to render SVG '{path}': {exc}") from exc
        finally:
            if temp_output.exists():
                temp_output.unlink()

    if suffix == ".png":
        try:
            with Image.open(path) as frame:
                frame.load()
                logger.debug("Loaded frame from %s", path)
                return _resize_frame(frame, width, height)
        except Exception as exc:
            raise FrameLoadError(f"Failed to load PNG '{path}': {exc}") from exc

    raise FrameLoadError(f"Unsupported source file type: {path}")


def load_frames(source: Path, width: int = 240, height: int = 320) -> List[Image.Image]:
    """Load one image file or a directory of image files into Pillow frames.

    Args:
        source: Path to a single SVG, a single PNG, or a directory containing SVG/PNG files.
        width: Desired output width in pixels.
        height: Desired output height in pixels.

    Returns:
        An ordered list of normalized Pillow Image objects, all converted to RGB.

    Raises:
        FrameLoadError: If the source path is invalid, unsupported, or contains no files.
    """
    if not isinstance(source, Path):
        source = Path(source)

    if not source.exists():
        raise FrameLoadError(f"Source path does not exist: {source}")

    if source.is_file():
        if source.suffix.lower() not in {".svg", ".png"}:
            raise FrameLoadError(f"Unsupported source file type: {source}")
        frames = [_load_single_frame(source, width, height)]
    elif source.is_dir():
        candidates = sorted(
            [path for path in source.iterdir() if path.is_file() and path.suffix.lower() in {".svg", ".png"}],
            key=lambda path: path.name,
        )
        if not candidates:
            raise FrameLoadError(f"Directory contains no supported SVG/PNG files: {source}")
        frames = [_load_single_frame(path, width, height) for path in candidates]
    else:
        raise FrameLoadError(f"Source path is neither a file nor a directory: {source}")

    logger.info("Loaded %d frame(s) from %s", len(frames), source)
    return frames
