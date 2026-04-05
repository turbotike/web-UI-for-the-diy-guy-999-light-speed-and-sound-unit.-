#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"

PY=""
if [ -x ".venv/bin/python3" ]; then
  PY=".venv/bin/python3"
elif [ -x ".venv/bin/python" ]; then
  PY=".venv/bin/python"
elif command -v python3 >/dev/null 2>&1; then
  PY="python3"
elif command -v python >/dev/null 2>&1; then
  PY="python"
fi

if [ -z "$PY" ]; then
  echo "Python was not found. Install Python 3 or create .venv first."
  exit 1
fi

echo "Starting RC Engine Sound Configurator..."
exec "$PY" configure.py
