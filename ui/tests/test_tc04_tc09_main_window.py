from pathlib import Path
import tkinter as tk

import pytest

from conftest import fill_valid_form


def test_tc04_read_settings_success(app, tmp_path: Path):
    _, _, output_dir, _ = fill_valid_form(app, tmp_path)

    settings = app.read_settings()

    assert settings.k == 2
    assert settings.mode == "parallel"
    assert settings.metric == "both"
    assert settings.quotas_xlsx.name == "quotas.xlsx"
    assert settings.network_xlsx.name == "network.xlsx"
    assert settings.output_xlsx == output_dir / "result.xlsx"


def test_tc05_empty_k_raises_error(app, tmp_path: Path):
    fill_valid_form(app, tmp_path)
    app.k_var.set("")

    with pytest.raises(ValueError):
        app.read_settings()


def test_tc06_negative_k_raises_error(app, tmp_path: Path):
    fill_valid_form(app, tmp_path)
    app.k_var.set("-1")

    with pytest.raises(ValueError):
        app.read_settings()


def test_tc07_missing_quotas_file_raises_error(app, tmp_path: Path):
    fill_valid_form(app, tmp_path)
    app.quotas_xlsx_var.set("")

    with pytest.raises(ValueError):
        app.read_settings()


def test_tc08_browse_file_sets_variable(app, tmp_path: Path, monkeypatch):
    import main_window

    selected_file = tmp_path / "quotas.xlsx"
    selected_file.write_text("fake", encoding="utf-8")

    monkeypatch.setattr(
        main_window.filedialog,
        "askopenfilename",
        lambda **kwargs: str(selected_file),
    )

    variable = tk.StringVar(master=app)

    try:
        app.browse_file(variable, False)
    except TypeError:
        app.browse_file(variable)

    assert variable.get() == str(selected_file)


def test_tc09_output_location_selector_sets_variable(app, tmp_path: Path, monkeypatch):
    import main_window

    variable = tk.StringVar(master=app)

    selected_dir = tmp_path / "output"
    selected_dir.mkdir()

    monkeypatch.setattr(
        main_window.filedialog,
        "askdirectory",
        lambda **kwargs: str(selected_dir),
    )

    app.browse_file(variable, True)

    assert variable.get() == str(selected_dir)
