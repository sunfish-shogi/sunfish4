# Sunfish4 ShogiHome wasm implementation

## Decision

Sunfish4's ShogiHome build uses Emscripten pthreads. The engine keeps its native
recursive search and runs it on a dedicated pthread, leaving the module's main
Worker available to receive `stop`, `quit`, and other USI commands.

This replaces the earlier single-threaded polling design. A recursive search
cannot be safely time-sliced by unwinding it at a deadline unless its complete
continuation state is retained. The pthread build does not need a resumable
search implementation.

## Build contract

- Build with `emscripten/emsdk:6.0.6` through `make wasm`.
- Compile and link with `-pthread`.
- Preallocate four pthread workers with `-sPTHREAD_POOL_SIZE=4`.
- Set `requiresCrossOriginIsolation` to `true` in `engine.json`.
- Produce an ES module using `MODULARIZE`, `EXPORT_ES6`, and the
  `worker,node` environments.
- Export `usi_command`; no polling export or public `poll()` method is used.
- Keep `eval.bin` and `book.bin` external and load them through manifest
  `dataFiles` entries at `/eval.bin` and `/book.bin`.

## Runtime model

`postMessage(command)` calls `usi_command(command)` on the module's main
Worker. Commands that do not start a search complete synchronously.

For `go`, the adapter:

1. parses the limits and configures `Searcher`;
2. handles an opening-book hit on the main Worker;
3. starts one dedicated search pthread;
4. lets the existing `Searcher::idsearch()` create its normal helper pthreads;
5. emits `info` and the final `bestmove` from the search pthread.

The configured USI `Threads` value counts the search thread plus its helper
search trees and is limited to 1 through 4. The Emscripten pool has four workers,
so `pthread_create` never needs to wait for the browser event loop to create a
new Worker.

`stop` sets the existing atomic search interruption flag; the search pthread
then emits the final `bestmove`. The main Worker does not synchronously join a
thread that may be proxying stdout back to it. `quit` and `terminate()` first
suppress later output and interrupt the search, then detach the outer search
pthread so its resources are reclaimed when it exits without blocking the main
Worker. The adapter lives until the enclosing Worker is terminated.

Final `bestmove` ownership is represented by an atomic state transition. A
cancel command changes `Ready` or `Pending` to `Suppressed`; the search thread
must change the same state to `Emitted` before writing. Only one transition can
win, so cancellation cannot race between a standalone flag check and output.

## Native compatibility

The native search implementation and `src/usi` behavior remain unchanged. The
wasm adapter has its own input validation and lifecycle state. There is no
`SUNFISH_SINGLE_THREAD` branch and no incremental-search API in `Searcher`.

## Validation

The wasm adapter must cover:

- `usi`, options, and `isready`;
- normal timed search and fixed-depth search;
- `go infinite` followed by `stop`;
- `quit` and `terminate()` output suppression;
- invalid position histories without aborting;
- `score mate` formatting;
- `UseBook` and `Threads` options;
- manifest/USI option agreement;
- absence of a public `poll()` method;
- a shared-memory wasm build with a preallocated pthread pool.
