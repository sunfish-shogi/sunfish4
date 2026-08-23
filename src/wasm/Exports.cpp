#include "wasm/WasmUsiClient.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define SUNFISH_KEEPALIVE EMSCRIPTEN_KEEPALIVE
#else
#define SUNFISH_KEEPALIVE
#endif

namespace {
sunfish::WasmUsiClient client;
}

extern "C" {

SUNFISH_KEEPALIVE void usi_command(const char* line) {
  client.command(line == nullptr ? "" : line);
}

}
