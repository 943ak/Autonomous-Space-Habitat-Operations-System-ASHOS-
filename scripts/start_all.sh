#!/usr/bin/env bash
set -euo pipefail

mkdir -p logs

# If a previous run left processes alive, stop them before relaunching.
kill_if_running() {
  local pid=$1
  local label=$2
  if [[ -n "${pid:-}" && ${pid:-0} -gt 0 ]] && kill -0 "$pid" 2>/dev/null; then
    echo "[start_all] stopping prior $label (pid=$pid)"
    kill "$pid" 2>/dev/null || true
  fi
}

if [[ -f .pids ]]; then
  read -r old_engine old_ai old_java < .pids || true
  kill_if_running "${old_engine:-0}" "C++ engine"
  kill_if_running "${old_ai:-0}" "Python AI"
  kill_if_running "${old_java:-0}" "Java backend"
fi

echo "[start_all] building C++ engine"
(cd cpp_engine && g++ -std=c++17 -pthread ../c_core/ls_ctrl.c ../c_core/pwr_ctrl.c ../c_core/thermal_ctrl.c ../c_core/watchdog.c engine_main.cpp -o engine)

echo "[start_all] launching C++ engine"
./cpp_engine/engine > logs/engine.log 2>&1 &
ENGINE_PID=$!

echo "[start_all] launching Python AI"
AI_PID=$(cd python_ai && nohup python3 ai_server.py > ../logs/ai_server.log 2>&1 & echo $!)

echo "[start_all] launching Java backend"
if command -v mvn >/dev/null 2>&1; then
  JAVA_PID=$(cd java_backend && nohup mvn spring-boot:run > ../logs/java_backend.log 2>&1 & echo $!)
else
  echo "[start_all] Maven missing; skipping Java backend launch"
  JAVA_PID=0
fi

echo "$ENGINE_PID $AI_PID $JAVA_PID" > .pids
