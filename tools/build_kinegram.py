"""CLI entry point for building a single interlaced kinegram BMP file."""

import argparse
import logging
import sys
from pathlib import Path
from typing import Optional

from PIL import Image, ImageSequence

REPO_ROOT = Path(__file__).resolve().parents[1]
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))

from tools.bmp_exporter import export_bmp
from tools.frame_generator import load_frames
from tools.interlacer import interlace_frames

logger = logging.getLogger(__name__)


def _configure_logging(verbose: bool) -> None:
    """Configure the root logger for CLI output."""
    level = logging.DEBUG if verbose else logging.INFO
    logging.basicConfig(level=level, format="%(levelname)s:%(name)s:%(message)s")


def _default_output_path(source: Path) -> Path:
    """Return the default BMP output path under the data/ directory."""
    return Path("data") / f"{source.stem}.bmp"


def _extract_gif_frames(source: Path, temp_dir: Path) -> Path:
    """Extract a GIF into a temporary directory of PNG frames."""
    temp_dir.mkdir(parents=True, exist_ok=True)

    try:
        with Image.open(source) as gif_image:
            frame_count = 0
            for frame in ImageSequence.Iterator(gif_image):
                frame = frame.convert("RGB")
                frame_path = temp_dir / f"frame_{frame_count:04d}.png"
                frame.save(frame_path, format="PNG")
                frame_count += 1

            if frame_count == 0:
                raise ValueError(f"GIF contains no frames: {source}")

            logger.info("Extracted %d frame(s) from GIF %s", frame_count, source)
            return temp_dir
    except Exception as exc:
        raise RuntimeError(f"Failed to extract GIF frames from '{source}': {exc}") from exc


def _resolve_source_for_pipeline(source: Path, temp_dir: Path) -> Path:
    """Return a source path that can be passed to load_frames()."""
    if source.suffix.lower() == ".gif":
        return _extract_gif_frames(source, temp_dir)
    return source


def build_kinegram(
    source: Path,
    output: Optional[Path],
    width: int,
    height: int,
    stripe_width: int,
    orientation: str,
    temp_dir: Path,
    frames_limit: Optional[int],
) -> tuple[Path, int]:
    """Build a single interlaced BMP file from a source SVG, PNG, directory, or GIF."""
    source_path = Path(source)
    if not source_path.exists():
        raise FileNotFoundError(f"Source path does not exist: {source_path}")

    resolved_source = _resolve_source_for_pipeline(source_path, temp_dir)
    frame_list = load_frames(resolved_source, width=width, height=height)

    if frames_limit is not None:
        if frames_limit <= 0:
            raise ValueError("--frames must be a positive integer")
        frame_list = frame_list[:frames_limit]

    interlaced = interlace_frames(frame_list, stripe_width=stripe_width, orientation=orientation)

    output_path = Path(output) if output is not None else _default_output_path(source_path)
    bmp_path = export_bmp(interlaced, output_path, width=width, height=height)
    return bmp_path, len(frame_list)


def parse_args(argv: Optional[list[str]] = None) -> argparse.Namespace:
    """Parse CLI arguments for the kinegram build pipeline."""
    parser = argparse.ArgumentParser(description="Build an interlaced BMP kinegram from SVG/PNG/GIF input.")
    parser.add_argument("source", help="Path to an SVG, PNG, directory of frames, or GIF file")
    parser.add_argument("--output", default=None, help="Output BMP path (default: data/<source_stem>.bmp)")
    parser.add_argument("--width", type=int, default=240, help="Output width in pixels")
    parser.add_argument("--height", type=int, default=320, help="Output height in pixels")
    parser.add_argument("--stripe-width", type=int, default=4, help="Interlacing stripe width")
    parser.add_argument("--orientation", choices=("vertical", "horizontal"), default="vertical")
    parser.add_argument("--temp-dir", default="temp", help="Directory for intermediate frame extraction")
    parser.add_argument("--frames", type=int, default=None, help="Limit the number of frames used")
    parser.add_argument("--verbose", action="store_true", help="Enable DEBUG logging")
    return parser.parse_args(argv)


def main(argv: Optional[list[str]] = None) -> int:
    """Run the CLI and return an exit code."""
    args = parse_args(argv)
    _configure_logging(args.verbose)

    try:
        bmp_path, frame_count = build_kinegram(
            source=Path(args.source),
            output=Path(args.output) if args.output else None,
            width=args.width,
            height=args.height,
            stripe_width=args.stripe_width,
            orientation=args.orientation,
            temp_dir=Path(args.temp_dir),
            frames_limit=args.frames,
        )

        print(f"✓ {bmp_path} ({args.width}×{args.height}, {frame_count} frames, stripe={args.stripe_width})")
        return 0
    except Exception as exc:
        print(f"Error: {exc}", file=sys.stderr)
        logger.exception("Build failed")
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
