"""Interlace multiple frames into a single kinegram-style image.

The idea is simple: split each source frame into narrow slices and place those slices
from different frames back together in order. This creates the striped pattern that
makes a kinegram appear to animate when viewed through a moving barrier or mask.
"""

import logging
from typing import List, Literal

from PIL import Image

logger = logging.getLogger(__name__)


class InterlaceError(Exception):
    """Raised when frames cannot be interlaced into a single image."""


def interlace_frames(
    frames: list[Image.Image],
    stripe_width: int = 4,
    orientation: Literal["vertical", "horizontal"] = "vertical",
) -> Image.Image:
    """Interleave columns or rows from a sequence of frames into one image.

    Args:
        frames: Ordered list of Pillow images to combine.
        stripe_width: Width or height of each stripe group in pixels.
        orientation: Interlacing direction: "vertical" or "horizontal".

    Returns:
        A new Pillow image containing the interlaced result.

    Raises:
        InterlaceError: If the input frames are empty, have mismatched sizes,
            or use an invalid stripe width or orientation.
    """
    if not frames:
        raise InterlaceError("Cannot interlace an empty frame list.")

    if stripe_width <= 0:
        raise InterlaceError(f"stripe_width must be positive, got {stripe_width}.")

    if orientation not in {"vertical", "horizontal"}:
        raise InterlaceError(f"Unsupported orientation: {orientation}")

    normalized_frames = [frame.convert("RGB") if frame.mode != "RGB" else frame.copy() for frame in frames]

    sizes = {frame.size for frame in normalized_frames}
    if len(sizes) != 1:
        raise InterlaceError(
            "All input frames must have identical dimensions; got "
            f"{sorted(sizes, key=lambda item: (item[0], item[1]))}."
        )

    width, height = normalized_frames[0].size
    if orientation == "vertical":
        result = Image.new("RGB", (width, height))
        for start in range(0, width, stripe_width):
            end = min(start + stripe_width, width)
            frame_index = (start // stripe_width) % len(normalized_frames)
            source_frame = normalized_frames[frame_index]
            stripe = source_frame.crop((start, 0, end, height))
            result.paste(stripe, (start, 0))
    else:
        result = Image.new("RGB", (width, height))
        for start in range(0, height, stripe_width):
            end = min(start + stripe_width, height)
            frame_index = (start // stripe_width) % len(normalized_frames)
            source_frame = normalized_frames[frame_index]
            stripe = source_frame.crop((0, start, width, end))
            result.paste(stripe, (0, start))

    logger.debug(
        "Interlacing frames: stripe_width=%d, orientation=%s, frame_count=%d, output_size=%s",
        stripe_width,
        orientation,
        len(normalized_frames),
        result.size,
    )
    return result
