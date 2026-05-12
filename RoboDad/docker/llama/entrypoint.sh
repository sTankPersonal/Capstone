#!/usr/bin/env bash
set -euo pipefail

MODEL="${OLLAMA_MODEL:-llama3.2:3b}"

# Start the Ollama server in the background.
ollama serve &
SERVER_PID=$!

# Wait until the REST API is accepting connections.
echo "[ollama] Waiting for server to start..."
until curl -sf http://localhost:11434/api/tags > /dev/null 2>&1; do
    sleep 2
done
echo "[ollama] Server is up."

# Pull the model only if it is not already cached in the volume.
if ! ollama list 2>/dev/null | grep -q "^${MODEL}"; then
    echo "[ollama] Pulling model: ${MODEL}"
    ollama pull "${MODEL}"
    echo "[ollama] Model ready: ${MODEL}"
else
    echo "[ollama] Model already cached: ${MODEL}"
fi

# Hand off to the server process; exit when it exits.
wait "${SERVER_PID}"