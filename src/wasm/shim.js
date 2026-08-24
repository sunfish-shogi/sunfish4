// ShogiHome wasm engine ABI adapter. Loaded through Emscripten --pre-js.
var sunfishMessageListeners = new Set();
var sunfishOriginalPrint = Module["print"];

Module["print"] = function(line) {
  var text = String(line);
  sunfishMessageListeners.forEach(function(listener) { listener(text); });
  if (sunfishOriginalPrint) sunfishOriginalPrint(text);
};

Module["postMessage"] = function(command) {
  Module.ccall("usi_command", null, ["string"], [String(command)]);
};
Module["addMessageListener"] = function(listener) {
  sunfishMessageListeners.add(listener);
};
Module["removeMessageListener"] = function(listener) {
  sunfishMessageListeners.delete(listener);
};
Module["terminate"] = function() {
  Module.ccall("usi_command", null, ["string"], ["quit"]);
  sunfishMessageListeners.clear();
};
