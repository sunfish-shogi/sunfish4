#!/usr/bin/env bash
set -euo pipefail

repo_root=$(cd "$(dirname "$0")/.." && pwd)

docker run --rm \
  --user "$(id -u):$(id -g)" \
  -e HOME=/tmp \
  -v "$repo_root:/src" \
  -w /src \
  emscripten/emsdk:6.0.6 \
  bash tools/build-wasm-in-docker.sh
