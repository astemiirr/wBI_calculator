from pathlib import Path
import sys


def find_wbi_cpp() -> Path:
    if sys.platform.startswith("win"):
        exe_name = "wbi.exe"
    else:
        exe_name = "wbi"

    candidates = []

    if getattr(sys, "frozen", False):
        app_dir = Path(sys.executable).resolve().parent

        candidates.append(app_dir / "bin" / exe_name)
        candidates.append(app_dir / "_internal" / "bin" / exe_name)

        if hasattr(sys, "_MEIPASS"):
            candidates.append(Path(sys._MEIPASS) / "bin" / exe_name)

    source_dir = Path(__file__).resolve().parent
    candidates.append(source_dir / "bin" / exe_name)

    for path in candidates:
        if path.exists():
            return path

    raise FileNotFoundError(
        "Cannot find wbi_cpp executable. Expected it in bin/wbi.exe"
    )
