import os
import sys
import locale

Translations = {
    "ru": {
        "title": "Вычиселние мер центральности WBI",
        "quotas_file": "Файл квот:",
        "network_file": "Файл сети:",
        "output_folder": "Папка результата:",
        "select": "Выбрать",
        "start": "Запустить",
        "mode": "Режим:",
        "metric": "Мера:",
        "log": "Лог:",
        "mode_linear": "Линейный",
        "mode_parallel": "Параллельный",
        "mode_compare": "Сравнение",
        "metric_both": "Обе",
        "input_error": "Ошибка ввода",
        "error": "Ошибка",
        "done": "Готово",
        "done_message": "Вычисление завершено",
        "choose_quotas": "Выберите файл квот",
        "choose_network": "Выберите файл сети",
        "choose_output_folder": "Выберите папку результата",
        "k_must_be_integer": "K должен быть целым числом",
        "k_must_be_positive": "K должен быть положительным числом",
        "choose_file": "Выберите файл",
        "choose_folder": "Выберите папку",
    },
    "en": {
        "title": "WBI Graph Centrality Calculator",
        "quotas_file": "Quotas file:",
        "network_file": "Network file:",
        "output_folder": "Output folder:",
        "select": "Browse",
        "start": "Start",
        "mode": "Mode:",
        "metric": "Metric:",
        "log": "Log:",
        "mode_linear": "Linear",
        "mode_parallel": "Parallel",
        "mode_compare": "Compare",
        "metric_both": "Both",
        "input_error": "Input error",
        "error": "Error",
        "done": "Done",
        "done_message": "Calculation finished",
        "choose_quotas": "Choose quotas file",
        "choose_network": "Choose network file",
        "choose_output_folder": "Choose output folder",
        "k_must_be_integer": "K must be an integer",
        "k_must_be_positive": "K must be positive",
        "choose_file": "Choose file",
        "choose_folder": "Choose folder",
    },
}


def get_system_language():
    candidates = []

    if sys.platform.startswith("win"):
        try:
            import ctypes

            buffer = ctypes.create_unicode_buffer(85)

            result = ctypes.windll.kernel32.GetUserDefaultLocaleName(
                buffer,
                len(buffer),
            )

            if result:
                lang = buffer.value.lower()

                if lang.startswith("ru"):
                    return "ru"

                if lang.startswith("en"):
                    return "en"
        except Exception:
            pass

        try:
            import ctypes

            language_id = ctypes.windll.kernel32.GetUserDefaultUILangauge()
            win_language = locale.windows_locale.get(language_id)

            if win_language:
                candidates.append(win_language)
        except Exception:
            pass

    for name in ["LANGUAGE", "LC_ALL", "LC_MESSAGES", "LANG"]:
        val = os.environ.get(name)

        if val:
            candidates.append(val)

    if val in candidates:
        val = val.lower()

        if val.startswith("ru"):
            return "ru"

        if val.startswith("en"):
            return "en"

    return "en"


class translator:
    def __init__(self):
        self.language = get_system_language()

    def t(self, key: str):
        return Translations[self.language].get(
            key,
            Translations["en"].get(key, key),
        )
