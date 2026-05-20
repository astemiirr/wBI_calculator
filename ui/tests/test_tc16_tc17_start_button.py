from pathlib import Path
import tkinter as tk

from conftest import ErrorPipeline, FakePipeline, ImmediateThread, fill_valid_form


def test_tc16_start_click_calls_pipeline_and_unlocks_button(app, tmp_path: Path, monkeypatch):
    import main_window

    fill_valid_form(app, tmp_path)

    fake_pipeline = FakePipeline()
    app.pipeline = fake_pipeline

    monkeypatch.setattr(main_window, "Thread", ImmediateThread)
    monkeypatch.setattr(main_window.messagebox, "showinfo", lambda *args, **kwargs: None)
    monkeypatch.setattr(main_window.messagebox, "showerror", lambda *args, **kwargs: None)

    app.start_click()
    app.update()

    assert fake_pipeline.called is True
    assert fake_pipeline.settings.k == 2
    assert str(app.start_button["state"]) == "normal"
    assert "fake pipeline executed" in app.log_text.get("1.0", tk.END)


def test_tc17_pipeline_error_shows_message_and_unlocks_button(app, tmp_path: Path, monkeypatch):
    import main_window

    fill_valid_form(app, tmp_path)

    errors = []
    app.pipeline = ErrorPipeline()

    monkeypatch.setattr(main_window, "Thread", ImmediateThread)
    monkeypatch.setattr(main_window.messagebox, "showinfo", lambda *args, **kwargs: None)
    monkeypatch.setattr(
        main_window.messagebox,
        "showerror",
        lambda title, message: errors.append((title, message)),
    )

    app.start_click()
    app.update()

    assert str(app.start_button["state"]) == "normal"
    assert errors
    assert "fake pipeline error" in errors[0][1]
