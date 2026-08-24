#!/usr/bin/env bash
set -euo pipefail

build_dir=out/build/wasm
package_dir=out/wasm/sunfish4

rm -rf "$build_dir" "$package_dir"
emcmake cmake -S src/wasm -B "$build_dir" -DCMAKE_BUILD_TYPE=Release
cmake --build "$build_dir" --parallel
chmod a-x "$build_dir/sunfish4.wasm"

mkdir -p "$package_dir"
cp "$build_dir/sunfish4.js" "$build_dir/sunfish4.wasm" "$package_dir/"
cp src/wasm/engine.json "$package_dir/"

echo "Wasm engine package: $package_dir"
