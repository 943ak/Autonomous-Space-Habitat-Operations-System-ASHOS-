#!/usr/bin/env bash
set -euo pipefail

echo "[start_all] launching C++ engine" 
(cd cpp_engine && g++ -std=c++17 -pthread ../c_core/ls_ctrl.c ../c_core/pwr_ctrl.c ../c_core/thermal_ctrl.c ../c_core/watchdog.c engine_main.cpp -o engine && ./engine &) 
ENGINE_PID=$!

echo "[start_all] launching Python AI"
(cd python_ai && nohup python3 ai_server.py > ../logs_ai.txt 2>&1 &)
AI_PID=$!

echo "[start_all] launching Java backend"
if command -v mvn >/dev/null 2>&1; then
  (cd java_backend && nohup mvn spring-boot:run > ../logs_java.txt 2>&1 &)
  JAVA_PID=$!
else
  echo "[start_all] Maven missing; skipping Java backend launch"
  JAVA_PID=0
fi

echo "$ENGINE_PID $AI_PID $JAVA_PID" > .pids
