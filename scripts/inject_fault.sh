#!/usr/bin/env bash
set -euo pipefail

FILE=${1:?"scenario file required"}
if [[ ! -f "$FILE" ]]; then
  echo "scenario file missing: $FILE" >&2
  exit 1
fi

echo "[fault] loading scenario $FILE"
cat "$FILE"
echo

echo "[fault] notifying AI service"
if command -v curl >/dev/null 2>&1; then
  curl -s -X POST http://localhost:5001 -H 'Content-Type: application/json' -d @"$FILE" || true
fi
