#!/usr/bin/env bash
set -euo pipefail

failures=0

check_process() {
  local name=$1
  local pattern=$2
  if pgrep -f "$pattern" >/dev/null; then
    echo "[health] $name alive"
  else
    echo "[health] $name missing" >&2
    failures=$((failures+1))
  fi
}

check_process "C++ engine" "cpp_engine/engine"
check_process "Python AI" "python_ai/ai_server.py"
check_process "Java backend" "ashos-mission-control"

if [[ $failures -gt 0 ]]; then
  echo "[health] restarting missing services"
  ./scripts/start_all.sh
fi
