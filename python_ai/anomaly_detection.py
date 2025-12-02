"""Tiny heuristic anomaly detector to emulate ML scoring."""
from typing import Dict


SAFE_RANGES = {
    "o2": (19.0, 23.5),
    "pressure": (95_000, 105_000),
    "temp": (18.0, 27.0),
}


def _normalize(value: float, bounds) -> float:
    low, high = bounds
    if high == low:
        return 0.0
    return max(0.0, (value - low) / (high - low))


def anomaly_score(telemetry: Dict[str, float]) -> float:
    """Compute a normalized anomaly score (0.0 nominal, 1.0 severe)."""
    terms = []
    if "o2_percent" in telemetry:
        terms.append(abs(_normalize(telemetry["o2_percent"] / 100.0, SAFE_RANGES["o2"]) - 0.5))
    if "pressure_pa" in telemetry:
        terms.append(abs(_normalize(telemetry["pressure_pa"], SAFE_RANGES["pressure"]) - 0.5))
    if "temperature_c" in telemetry:
        terms.append(abs(_normalize(telemetry["temperature_c"] / 100.0, SAFE_RANGES["temp"]) - 0.5))

    if not terms:
        return 0.0
    return min(1.0, sum(terms) / len(terms) * 2)
