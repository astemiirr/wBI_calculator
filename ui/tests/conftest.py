from __future__ import annotations

from dataclasses import dataclass, fields
from pathlib import Path
import sys
import tkinter as tk

import pandas as pd
import pytest


@dataclass
class FakeCppResult:
    return_code: int = 0
    seconds: float = 0.1
    stdout: str = "ok"
    stderr: str = ""


class FakeCppRunner:
    """Имитирует C++ ядро без запуска внешнего процесса.

    В текущем C++ коде result.csv записывается через запятую, поэтому fake-результат
    тоже создается с разделителем ','. Если тест падает на чтении result.csv, нужно
    согласовать разделитель результата между C++ и Python converter.py.
    """

    def run(self, exe_path, quotas_csv, network_csv, output_csv, k, mode, metric, log):
        assert Path(quotas_csv).exists()
        assert Path(network_csv).exists()
        assert mode in {"linear", "parallel"}
        assert metric in {"wbi1", "wbi2", "both"}

        data = {
            "Country": ["A", "B"],
            "wBI_1": [0.1, 0.2],
            "wBI_2": [0.3, 0.4],
        }

        if metric == "wbi1":
            data = {"Country": ["A", "B"], "wBI_1": [0.1, 0.2]}
        elif metric == "wbi2":
            data = {"Country": ["A", "B"], "wBI_2": [0.3, 0.4]}

        pd.DataFrame(data).to_csv(output_csv, index=False, sep=",")

        seconds = 1.0 if mode == "linear" else 0.5
        log(f"fake C++ executed: {mode}, {metric}")
        return FakeCppResult(seconds=seconds)


class ImmediateThread:
    """Подмена Thread: запускает target сразу, чтобы UI-тест был синхронным."""

    def __init__(self, target, args=(), kwargs=None, daemon=None):
        self.target = target
        self.args = args
        self.kwargs = kwargs or {}
        self.daemon = daemon

    def start(self):
        self.target(*self.args, **self.kwargs)


class FakePipeline:
    def __init__(self):
        self.called = False
        self.settings = None

    def run(self, settings, log):
        self.called = True
        self.settings = settings
        log("fake pipeline executed")


class ErrorPipeline:
    def run(self, settings, log):
        raise RuntimeError("fake pipeline error")


def create_input_xlsx(tmp_path: Path):
    quotas_xlsx = tmp_path / "quotas.xlsx"
    network_xlsx = tmp_path / "network.xlsx"

    pd.DataFrame(
        {
            "Country": ["A", "B"],
            "Quota": [1, 2],
            "Extra": ["x", "y"],
        }
    ).to_excel(quotas_xlsx, index=False)

    pd.DataFrame(
        {
            "from": ["A"],
            "to": ["B"],
            "weight": [10],
            "Extra": ["z"],
        }
    ).to_excel(network_xlsx, index=False)

    return quotas_xlsx, network_xlsx


def make_settings(tmp_path: Path, mode: str = "parallel", metric: str = "both"):
    from settings import RunSettings

    quotas_xlsx, network_xlsx = create_input_xlsx(tmp_path)
    output_xlsx = tmp_path / f"result_{mode}.xlsx"
    cpp_file = tmp_path / ("wbi.exe" if sys.platform.startswith("win") else "wbi")
    cpp_file.write_text("fake executable", encoding="utf-8")

    field_names = {field.name for field in fields(RunSettings)}
    kwargs = {
        "quotas_xlsx": quotas_xlsx,
        "network_xlsx": network_xlsx,
        "output_xlsx": output_xlsx,
        "k": 2,
        "mode": mode,
        "metric": metric,
    }

    if "cpp" in field_names:
        kwargs["cpp"] = cpp_file
    elif "cpp_exe" in field_names:
        kwargs["cpp_exe"] = cpp_file

    if "keep_temp" in field_names:
        kwargs["keep_temp"] = False

    return RunSettings(**kwargs)


@pytest.fixture
def app(tmp_path, monkeypatch):
    import main_window

    fake_cpp = tmp_path / ("wbi.exe" if sys.platform.startswith("win") else "wbi")
    fake_cpp.write_text("fake executable", encoding="utf-8")

    if hasattr(main_window, "find_wbi_cpp"):
        monkeypatch.setattr(main_window, "find_wbi_cpp", lambda: fake_cpp)

    try:
        import paths

        if hasattr(paths, "find_wbi_cpp"):
            monkeypatch.setattr(paths, "find_wbi_cpp", lambda: fake_cpp)
    except ImportError:
        pass

    try:
        window = main_window.MainWindow()
    except tk.TclError as error:
        pytest.skip(f"Tkinter GUI is not available: {error}")

    window.withdraw()
    yield window
    window.destroy()


def _set_combobox_code(app, var_name: str, mapping_name: str, code: str):
    variable = getattr(app, var_name)

    if hasattr(app, mapping_name):
        mapping = getattr(app, mapping_name)
        label = next(label for label, value in mapping.items() if value == code)
        variable.set(label)
    else:
        variable.set(code)


def fill_valid_form(app, tmp_path: Path):
    quotas_xlsx, network_xlsx = create_input_xlsx(tmp_path)
    output_dir = tmp_path / "output"
    output_dir.mkdir()
    cpp_file = tmp_path / ("wbi.exe" if sys.platform.startswith("win") else "wbi")
    cpp_file.write_text("fake executable", encoding="utf-8")

    if hasattr(app, "cpp_var"):
        app.cpp_var.set(str(cpp_file))

    app.quotas_xlsx_var.set(str(quotas_xlsx))
    app.network_xlsx_var.set(str(network_xlsx))

    if hasattr(app, "output_dir_var"):
        app.output_dir_var.set(str(output_dir))
    elif hasattr(app, "output_xlsx_var"):
        app.output_xlsx_var.set(str(output_dir / "result.xlsx"))
    else:
        raise AssertionError("MainWindow must have output_dir_var or output_xlsx_var")

    app.k_var.set("2")
    _set_combobox_code(app, "mode_var", "mode_to_code", "parallel")
    _set_combobox_code(app, "metric_var", "metric_to_code", "both")

    return quotas_xlsx, network_xlsx, output_dir, cpp_file
