from pathlib import Path

import pandas as pd

from converter import ExcelCsvConverter


def test_tc01_quotas_xlsx_to_csv_has_two_columns(tmp_path: Path):
    input_xlsx = tmp_path / "quotas.xlsx"
    output_csv = tmp_path / "quotas.csv"

    pd.DataFrame(
        {
            "Country": ["A", "B"],
            "Quota": [1, 2],
            "Extra": ["must", "be removed"],
        }
    ).to_excel(input_xlsx, index=False)

    converter = ExcelCsvConverter()
    converter.xlsx_to_csv(input_xlsx, output_csv, "quota")

    assert output_csv.exists()

    result = pd.read_csv(output_csv, sep=";")
    assert list(result.columns) == ["Country", "Quota"]
    assert len(result) == 2


def test_tc02_network_xlsx_to_csv_has_three_columns(tmp_path: Path):
    input_xlsx = tmp_path / "network.xlsx"
    output_csv = tmp_path / "network.csv"

    pd.DataFrame(
        {
            "from": ["A"],
            "to": ["B"],
            "weight": [10],
            "Extra": ["must be removed"],
        }
    ).to_excel(input_xlsx, index=False)

    converter = ExcelCsvConverter()
    converter.xlsx_to_csv(input_xlsx, output_csv, "network")

    assert output_csv.exists()

    result = pd.read_csv(output_csv, sep=";")
    assert list(result.columns) == ["from", "to", "weight"]
    assert len(result) == 1


def test_tc03_result_csv_to_xlsx_from_cpp_result(tmp_path: Path):
    input_csv = tmp_path / "result.csv"
    output_xlsx = tmp_path / "result.xlsx"

    # C++ модуль сейчас пишет result.csv через запятую.
    pd.DataFrame(
        {
            "Country": ["A", "B"],
            "wBI_1": [0.1, 0.2],
            "wBI_2": [0.3, 0.4],
        }
    ).to_csv(input_csv, index=False, sep=",")

    converter = ExcelCsvConverter()
    converter.csv_to_xlsx(input_csv, output_xlsx)

    assert output_xlsx.exists()

    result = pd.read_excel(output_xlsx)
    assert list(result.columns) == ["Country", "wBI_1", "wBI_2"]
    assert len(result) == 2
