from dataclasses import dataclass

from pathlib import Path


@dataclass
class RunSettings:
    cpp: Path
    quotas_xlsx: Path
    network_xlsx: Path
    output_xlsx: Path
    k: int

    # режим вычисление linear, parallel, compare
    mode: str

    # мера центральности: wbi1, wbi2, both
    metric: str
