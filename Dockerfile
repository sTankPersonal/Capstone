# ── Stage 1: Build ────────────────────────────────────────────────────────────
# Ubuntu 24.04 (Noble) ships GLIBC 2.39 and GCC 14 in its main repos,
# ensuring the builder and runtime share the exact same ABI.
FROM ubuntu:24.04 AS builder

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    gcc-14 g++-14 \
    cmake \
    ninja-build \
    libpq-dev \
    libcurl4-openssl-dev \
    libssl-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# .dockerignore keeps Windows build artefacts and IDE folders out of context.
COPY . .

RUN cmake -S . -B /build \
        -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_COMPILER=g++-14 \
        -DCMAKE_C_COMPILER=gcc-14 \
        -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=/build/bin \
        -DBUILD_TESTING=OFF \
    && cmake --build /build --parallel "$(nproc)"

# ── Stage 2: Runtime ──────────────────────────────────────────────────────────
# Same Ubuntu 24.04 base guarantees GLIBC 2.39 and GLIBCXX 3.4.32
# are present — matching exactly what the builder linked against.
FROM ubuntu:24.04 AS runtime

ARG DEBIAN_FRONTEND=noninteractive

# libcurl4t64 / libssl3t64: Ubuntu 24.04 renamed these packages for the
# 64-bit time_t ABI transition; libcurl4t64 pulls in libssl3t64 as a dep.
RUN apt-get update && apt-get install -y --no-install-recommends \
    libpq5 \
    libcurl4t64 \
    libstdc++6 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /build/bin/RoboDad ./RoboDad

EXPOSE 18080

CMD ["./RoboDad"]
