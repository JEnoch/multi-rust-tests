FROM ubuntu:noble
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y \
    build-essential cmake rustup pkg-config ca-certificates \
    && rm -rf /var/lib/apt/lists/*
RUN mkdir -p /root/.cargo/bin && \
    ln -sf /usr/bin/rustup /root/.cargo/bin/rustup && \
    rustup toolchain install 1.75.0 --no-self-update && \
    rustup toolchain install 1.88.0 --no-self-update
WORKDIR /workspace
