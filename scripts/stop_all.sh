#!/usr/bin/env bash
set -euo pipefail

kill_if_running() {
  local pid=$1
  local label=$2
  if [[ -n "${pid:-}" && ${pid:-0} -gt 0 ]] && kill -0 "$pid" 2>/dev/null; then
    echo "[stop_all] stopping $label (pid=$pid)"
    kill "$pid" 2>/dev/null || true
    wait "$pid" 2>/dev/null || true
  fi
}

if [[ -f .pids ]]; then
  read -r engine_pid ai_pid java_pid < .pids || true
  kill_if_running "${engine_pid:-0}" "C++ engine"
  kill_if_running "${ai_pid:-0}" "Python AI"
  kill_if_running "${java_pid:-0}" "Java backend"
  rm -f .pids
else
  echo "[stop_all] no pid file; nothing to stop"
fi
