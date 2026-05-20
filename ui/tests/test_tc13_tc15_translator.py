import pytest

translator = pytest.importorskip("translator")


def _get_language_function():

    if hasattr(translator, "get_system_language"):
        return translator.get_system_language

    raise AssertionError("translator.py must contain get_system_language()")


def _force_linux_env(monkeypatch, lang_value: str):
    if hasattr(translator, "sys"):
        monkeypatch.setattr(translator.sys, "platform", "linux")

    monkeypatch.setenv("LANG", lang_value)
    monkeypatch.delenv("LANGUAGE", raising=False)
    monkeypatch.delenv("LC_ALL", raising=False)
    monkeypatch.delenv("LC_MESSAGES", raising=False)


def test_tc13_russian_system_language_selects_russian(monkeypatch):
    language_function = _get_language_function()

    _force_linux_env(monkeypatch, "ru_RU.UTF-8")

    assert language_function() == "ru"


def test_tc14_english_system_language_selects_english(monkeypatch):
    language_function = _get_language_function()

    _force_linux_env(monkeypatch, "en_US.UTF-8")

    assert language_function() == "en"


def test_tc15_unknown_system_language_fallbacks_to_english(monkeypatch):
    language_function = _get_language_function()

    _force_linux_env(monkeypatch, "de_DE.UTF-8")

    assert language_function() == "en"
