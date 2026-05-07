from pathlib import Path
import pandas as pd


class ExcelCsvConverter:
    def xlsx_to_csv(self, xlsx_file: Path, output_csv: Path, file_type: str):
        f = pd.read_excel(xlsx_file)

        if file_type == "quota":
            f = f.iloc[:, :2]
        elif file_type == "network":
            f = f.iloc[:, :3]
        else:
            raise ValueError("Type of file must be quota or network")

        output_csv.parent.mkdir(parents=True, exist_ok=True)
        f.to_csv(output_csv, index=False, sep=";", encoding="utf-8-sig")

    def csv_to_xlsx(self, csv_file: Path, output_xlsx: Path):
        f = pd.read_csv(csv_file)
        output_xlsx.parent.mkdir(parents=True, exist_ok=True)
        f.to_excel(output_xlsx, index=False)

    def compare_csv_to_xlsx(
        self,
        linear_csv: Path,
        parallel_csv: Path,
        output_xlsx: Path,
        linear_seconds: float,
        parallel_seconds: float,
        k: int,
        metric: str,
    ):
        linear_df = pd.read_csv(linear_csv, sep=";")
        parallel_df = pd.read_csv(parallel_csv, sep=";")

        linear_df = linear_df.iloc[:, :3].copy()
        parallel_df = parallel_df.iloc[:, :3].copy()

        while len(linear_df.columns) < 3:
            linear_df[f"empty_{len(linear_df.columns)}"] = ""

        while len(parallel_df.columns) < 3:
            parallel_df[f"empty_{len(parallel_df.columns)}"] = ""

        linear_df.columns = [
            "linear_Country",
            "linear_wBI_1",
            "linear_wBI_2",
        ]

        parallel_df.columns = [
            "parallel_Country",
            "parallel_wBI_1",
            "parallel_wBI_2",
        ]

        max_rows = max(len(linear_df), len(parallel_df))

        linear_df = linear_df.reindex(range(max_rows))
        parallel_df = parallel_df.reindex(range(max_rows))

        comparison_df = pd.concat(
            [parallel_df, linear_df],
            axis=1,
        )

        diff_wbi_1 = (
            pd.to_numeric(comparison_df["parallel_wBI_1"], errors="coerce")
            - pd.to_numeric(comparison_df["linear_wBI_1"], errors="coerce")
        ).abs()

        diff_wbi_2 = (
            pd.to_numeric(comparison_df["parallel_wBI_2"], errors="coerce")
            - pd.to_numeric(comparison_df["linear_wBI_2"], errors="coerce")
        ).abs()

        if parallel_seconds > 0:
            speedup = linear_seconds / parallel_seconds
        else:
            speedup = None

        stats_df = pd.DataFrame(
            [
                ["K", k],
                ["metric", metric],
                ["linear_seconds", linear_seconds],
                ["parallel_seconds", parallel_seconds],
                ["speedup", speedup],
                ["max_abs_diff_wBI_1", diff_wbi_1.max()],
                ["max_abs_diff_wBI_2", diff_wbi_2.max()],
                ["linear_rows", len(linear_df)],
                ["parallel_rows", len(parallel_df)],
            ],
            columns=["Parameter", "Value"],
        )

        output_xlsx.parent.mkdir(parents=True, exist_ok=True)

        with pd.ExcelWriter(output_xlsx, engine="openpyxl") as writer:
            comparison_df.to_excel(writer, sheet_name="Comparison", index=False)
            stats_df.to_excel(writer, sheet_name="Stats", index=False)
