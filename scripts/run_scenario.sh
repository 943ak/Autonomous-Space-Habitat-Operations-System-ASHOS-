#!/usr/bin/env bash
set -euo pipefail

SCENARIO=${1:-config/scenarios/solar_storm.yaml}

echo "[scenario] starting stack"
./scripts/start_all.sh

sleep 2

echo "[scenario] injecting fault from $SCENARIO"
./scripts/inject_fault.sh "$SCENARIO"

sleep 2

echo "[scenario] running AI analysis"
python3 - <<'PYCODE'
from python_ai.analysis_tools import run_trials
from python_ai.anomaly_detection import anomaly_score


def mock_sim(scenario):
    scenario["anomaly"] = anomaly_score(
        {
            "o2_percent": scenario.get("o2_percent", 2100),
            "pressure_pa": scenario.get("pressure_pa", 101325),
            "temperature_c": scenario.get("temperature_c", 2200),
        }
    )
    return scenario

print(run_trials(mock_sim, 3))
PYCODE

echo "[scenario] stopping stack"
./scripts/stop_all.sh
