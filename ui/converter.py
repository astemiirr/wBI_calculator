from pathlib import Path
import pandas as pd


class ExcelCsvConverter:
    def xlsx_to_csv(self, xlsx_file: Path, output_csv: Path, file_type: str):
        f = pd.read_excel(xlsx_file)

        if file_type == "quota":
            f.iloc[:, :2]
        elif file_type == "network":
            f.iloc[:, :3]
        else:
            raise ValueError("Type of file must be quota or network")

        output_csv.parent.mkdir(parents=True, exist_ok=True)
        f.to_csv(output_csv, index=False, sep=";", encoding="utf-8-sig")

    def csv_to_xlsx(self, csv_file: Path, output_xlsx: Path):
        f = pd.read_csv(csv_file)
        output_xlsx.parent.mkdir(parents=True, exist_ok=True)
        f.to_excel(output_xlsx, index=False)
