import shutil
import tempfile
from pathlib import Path
from typing import Callable
from settings import RunSettings
from converter import ExcelCsvConverter
from cpp_runner import CppRunner

LogFunction = Callable[[str], None]


class Pipeline:
    def __init__(self):
        self.converter = ExcelCsvConverter()

        self.runner = CppRunner()

    def run(self, settings: RunSettings, log: LogFunction):
        log("Pipeline started")

        tmp_dir = Path(tempfile.mkdtemp(prefix="wbi_"))

        try:
            quotas_csv = tmp_dir / "quotas.csv"
            network_csv = tmp_dir / "network.csv"

            log("Converting quotas.xlsx to quotas.csv")
            self.converter.xlsx_to_csv(settings.quotas_xlsx, quotas_csv, "quota")

            log("Converting network.xlsx to network.csv")
            self.converter.xlsx_to_csv(settings.network_xlsx, network_csv, "network")

            if settings.mode == "compare":
                self.run_compare(settings, quotas_csv, network_csv, tmp_dir, log)

            else:
                self.run_single(settings, quotas_csv, network_csv, tmp_dir, log)

        finally:
            shutil.rmtree(tmp_dir, ignore_errors=True)

        log("Pipeline finished")

    def run_single(
        self,
        settings: RunSettings,
        quotas_csv: Path,
        network_csv: Path,
        tmp_dir: Path,
        log: LogFunction,
    ):
        result_csv = tmp_dir / "result.csv"
        self.runner.run(
            exe_path=settings.cpp,
            quotas_csv=quotas_csv,
            network_csv=network_csv,
            output_csv=result_csv,
            k=settings.k,
            mode=settings.mode,
            metric=settings.metric,
            log=log,
        )

        log("Converting result.csv to result.xlsx")
        self.converter.csv_to_xlsx(result_csv, settings.output_xlsx)

    def run_compare(
        self,
        settings: RunSettings,
        quotas_csv: Path,
        network_csv: Path,
        tmp_dir: Path,
        log: LogFunction,
    ):
        linear_csv = tmp_dir / "result_linear.csv"
        parallel_csv = tmp_dir / "result_parallel.csv"

        log("Runnig linear version")

        self.runner.run(
            exe_path=settings.cpp,
            quotas_csv=quotas_csv,
            network_csv=network_csv,
            output_csv=linear_csv,
            k=settings.k,
            mode=settings.mode,
            metric="linear",
            log=log,
        )

        log("Runnig parallel version")

        self.runner.run(
            exe_path=settings.cpp,
            quotas_csv=quotas_csv,
            network_csv=network_csv,
            output_csv=linear_csv,
            k=settings.k,
            mode=settings.mode,
            metric="parallel",
            log=log,
        )

        log("Creating comparison xlsx")
        # self.converter.csv_to_xlsx(linear_csv, settings.output_xlsx) надо исправить!
        self.converter.csv_to_xlsx(parallel_csv, settings.output_xlsx)
