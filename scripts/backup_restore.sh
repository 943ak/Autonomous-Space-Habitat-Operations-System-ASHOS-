#!/usr/bin/env bash
set -euo pipefail

CMD=${1:-backup}
BACKUP_FILE=${2:-telemetry_backup.tar.gz}

case "$CMD" in
  backup)
    echo "[backup] archiving telemetry logs"
    tar -czf "$BACKUP_FILE" logs_ai.txt logs_java.txt 2>/dev/null || true
    ;;
  restore)
    echo "[restore] extracting telemetry logs"
    tar -xzf "$BACKUP_FILE"
    ;;
  *)
    echo "usage: $0 [backup|restore] [file]" >&2
    exit 1
    ;;
esac
