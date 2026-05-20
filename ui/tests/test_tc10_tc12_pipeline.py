from pathlib import Path

import pandas as pd

from conftest import FakeCppRunner, make_settings
from pipeline import Pipeline


def test_tc10_pipeline_single_creates_result_xlsx(tmp_path: Path):
    settings = make_settings(tmp_path, mode="parallel", metric="both")

    pipeline = Pipeline()
    pipeline.runner = FakeCppRunner()

    logs = []
    pipeline.run(settings, logs.append)

    assert settings.output_xlsx.exists()

    result = pd.read_excel(settings.output_xlsx)
    assert "Country" in result.columns
    assert "wBI_1" in result.columns
    assert "wBI_2" in result.columns


def test_tc11_pipeline_compare_creates_comparison_and_stats(tmp_path: Path):
    settings = make_settings(tmp_path, mode="compare", metric="both")

    pipeline = Pipeline()
    pipeline.runner = FakeCppRunner()

    logs = []
    pipeline.run(settings, logs.append)

    assert settings.output_xlsx.exists()

    comparison = pd.read_excel(settings.output_xlsx, sheet_name="Comparison")
    stats = pd.read_excel(settings.output_xlsx, sheet_name="Stats")

    assert "parallel_Country" in comparison.columns
    assert "linear_Country" in comparison.columns
    assert "Parameter" in stats.columns
    assert "Value" in stats.columns
    assert "speedup" in set(stats["Parameter"])


def test_tc12_pipeline_removes_temporary_directory(tmp_path: Path, monkeypatch):
    import pipeline as pipeline_module

    settings = make_settings(tmp_path, mode="parallel", metric="both")
    fixed_tmp_dir = tmp_path / "fixed_wbi_temp"

    def fake_mkdtemp(prefix=""):
        fixed_tmp_dir.mkdir()
        return str(fixed_tmp_dir)

    monkeypatch.setattr(pipeline_module.tempfile, "mkdtemp", fake_mkdtemp)

    pipeline = Pipeline()
    pipeline.runner = FakeCppRunner()

    logs = []
    pipeline.run(settings, logs.append)

    assert settings.output_xlsx.exists()
    assert not fixed_tmp_dir.exists()
