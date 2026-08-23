#include "wasm/WasmUsiClient.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define SUNFISH_KEEPALIVE EMSCRIPTEN_KEEPALIVE
#else
#define SUNFISH_KEEPALIVE
#endif

namespace {
// The enclosing Worker owns the module lifetime. Keeping the adapter alive
// until Worker termination lets an interrupted, detached pthread finish safely.
sunfish::WasmUsiClient* client = new sunfish::WasmUsiClient();
}

extern "C" {

SUNFISH_KEEPALIVE void usi_command(const char* line) {
  client->command(line == nullptr ? "" : line);
}

}
