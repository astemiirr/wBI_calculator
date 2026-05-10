import subprocess
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Callable

LogFunction = Callable[[str], None]


@dataclass
class CppRunResult:
    return_code: int
    seconds: float
    stdout: str
    stderr: str


class CppRunner:
    def run(
        self,
        exe_path: Path,
        quotas_csv: Path,
        network_csv: Path,
        output_csv: Path,
        k: int,
        mode: str,
        metric: str,
        log: LogFunction,
    ) -> CppRunResult:
        command = [
            str(exe_path),
            "--quotas",
            str(quotas_csv),
            "--network",
            str(network_csv),
            "--ouput",
            str(output_csv),
            "--k",
            str(k),
            "--mode",
            mode,
            "--metric",
            metric,
        ]

        log("Запуск C++: " + " ".join(command))

        start_time = time.perf_counter()

        completed_process = subprocess.run(
            command,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        )

        seconds = time.perf_counter() - start_time

        if completed_process.stdout:
            log("stdout:")
            log(completed_process.stdout)

        if completed_process.stderr:
            log("stderr:")
            log(completed_process.stderr)

        result = CppRunResult(
            return_code=completed_process.returncode,
            seconds=seconds,
            stdout=completed_process.stdout,
            stderr=completed_process.stderr,
        )

        if result.return_code != 0:
            raise RuntimeError(f"C++ finished with error code {result.return_code}")

        if not output_csv.exists():
            raise FileExistsError(f"C++ did not create result file {output_csv}")

        log(f"C++ finished in {seconds:.3f} seconds")

        return result
