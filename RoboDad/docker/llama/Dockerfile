FROM ollama/ollama:latest

# curl is needed for the health check and the entrypoint readiness probe.
RUN apt-get update && apt-get install -y --no-install-recommends curl \
    && rm -rf /var/lib/apt/lists/*

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

EXPOSE 11434

# Override the base image entrypoint so we can pull the model on first boot.
ENTRYPOINT ["/entrypoint.sh"]