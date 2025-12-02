"""Placeholder for RL-style policy recommendations."""
from typing import Dict


def recommend_power_split(telemetry: Dict[str, float]) -> Dict[str, int]:
    load = telemetry.get("load_w", 3000)
    solar = telemetry.get("solar_input_w", 5000)
    soc = telemetry.get("battery_soc", 8000)

    reserve = 0
    if soc < 3000:
        reserve = 1000
    elif soc > 9000:
        reserve = -500

    charge_limit = max(0, solar - load + reserve)
    discharge_limit = max(0, load - solar)

    return {
        "charge_limit_w": int(charge_limit),
        "discharge_limit_w": int(discharge_limit),
        "preferred_source": 2 if solar > load else 0,
    }
