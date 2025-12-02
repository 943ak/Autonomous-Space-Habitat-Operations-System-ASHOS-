"""Monte-Carlo style sampling for fault resilience."""
import random
from typing import Callable, Dict, List


def run_trials(sim_fn: Callable[[Dict[str, float]], Dict[str, float]], trials: int = 10) -> List[Dict[str, float]]:
    results = []
    for _ in range(trials):
        scenario = {
            "pressure_pa": random.randint(98_000, 104_000),
            "o2_percent": random.randint(1950, 2300),
            "temperature_c": random.randint(1800, 2600),
            "solar_input_w": random.randint(2000, 9000),
            "load_w": random.randint(1500, 6000),
        }
        results.append(sim_fn(scenario))
    return results
