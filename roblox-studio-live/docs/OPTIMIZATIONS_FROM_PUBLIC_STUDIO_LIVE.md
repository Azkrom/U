# Optimizations adopted from public Studio Live research

Reference reviewed: https://github.com/gurmyd/roblox-studio-live

We are not replacing Azkrom's cloud relay design. We are porting ideas that fit a remote ChatGPT -> Worker -> Windows agent topology.

## High-impact

### Persistent push transport
Keep exactly one outbound WebSocket per installation. Multiplex requests with request IDs and push replies/events on the same socket. Never poll the agent every 500 ms.

### Event journal + cursor
Every pushed event gets a monotonic sequence. Keep a bounded ring journal in the Durable Object so ChatGPT can request `events(since)` when it missed a push or reconnects.

### Whole-program Luau
Prefer `execute_luau` with a complete operation over dozens of `set_property` / `inspect_instance` micro-turns. This removes LLM/network turns, which dominate latency.

### Keep playtests alive
Where Roblox permits it, patch script source or run controller code in the active server/client DataModel instead of stop -> edit -> restart. Use explicit session/studio routing.

### FIFO writes
Serialize mutating calls per Studio session, but allow safe read-only calls to execute concurrently. This avoids write interleaving without making reads artificially slow.

### Native tool-list cache
The native MCP advertises `tools.listChanged=true`. Cache `tools/list` in the Windows agent and relay, and refresh only on list-changed notification, process restart, or an explicit refresh request.

### Background jobs
If the native tool returns a job handle, relay it immediately. Do not occupy a Cloudflare request waiting for a long AI/model/asset job. Let callers use `wait_job_finished`.

## Medium-impact

### Coalesced heartbeat
Heartbeat frames should carry liveness, current Studio IDs and native capability generation in one compact message. Do not separately poll status, sessions and tools.

### Adaptive reconnect
Reconnect immediately once, then exponential backoff with jitter and a cap. Reset after a stable connection. A watchdog should not spawn a second agent while the first is reconnecting.

### Idempotency keys
For relay retries, mutating requests carry a stable idempotency key. The agent keeps a short LRU of completed request IDs so a retried Worker request cannot perform the same mutation twice.

### Snapshot caching
Project summaries/intelligence can use a very short per-Studio cache keyed by Studio revision/event sequence. Invalidate on mutation or native change event.

### Bounded concurrency
Use a small read concurrency window (for example 4-8) instead of unbounded Promise.all. Writes stay FIFO.

## Optional / platform-specific

### Fast screenshot path
The public project reports large gains from Win32 `PrintWindow` instead of in-engine capture. V8 leaves this behind a capability flag so the native `screen_capture` remains the fallback.

### Resident helper runtime
A small resident Luau helper can provide bulk operations, assertions and run-until predicates. This is an optimization layer over Roblox's native MCP, not a replacement for it.

## Ideas deliberately not copied

- Local-only stdio as the primary architecture: Azkrom needs a remote ChatGPT endpoint.
- Anthropic-specific vision coupling.
- Removing authentication from the local bridge.
- Assuming a single Studio window.
