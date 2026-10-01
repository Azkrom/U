# Roblox Studio Live — Azkrom

Cloud-hosted Roblox Studio MCP used by Azkrom.

This tree preserves the custom architecture already deployed at:

- MCP: `https://roblox-studio-live.toxicwinter120130.workers.dev/mcp`
- Transport: ChatGPT -> Cloudflare Worker / Durable Object relay -> one outbound Windows agent per installation -> Roblox Studio native MCP
- Local truth: the Windows agent is the only process allowed to own the native Studio MCP connection.
- Multi-Studio routing: every write is routed by a fresh `studio_id`; the relay never assumes that the first Studio window is the intended place.

## Current deployed behavior

The currently paired installation reports:

- agent version: 0.1.0
- authenticated + approved
- native Studio MCP online
- 28 native tools
- MCP protocol version 2025-11-25

The original V7/V7.1 source archive is not recoverable from the current ChatGPT file surface, so this repository is a faithful reconstruction of the deployed architecture and its public tool contract, prepared as the source-of-truth for V8.

## V8 goals

V8 keeps the cloud relay architecture and adds the useful performance ideas found in the MIT-licensed public `gurmyd/roblox-studio-live` project without replacing this implementation:

1. one persistent multiplexed WebSocket; no per-tool reconnects;
2. monotonic request/event sequence numbers and replayable event journal;
3. native `tools/list` cache invalidated only by `notifications/tools/list_changed`;
4. coalesced heartbeats and adaptive reconnect backoff;
5. whole-program Luau calls instead of chains of micro-calls;
6. bounded read batching and parallel read-only calls;
7. long-running operations return job handles instead of holding the MCP request;
8. optional Win32 screenshot sidecar for fast captures while Studio is occluded/minimized;
9. hot-patch helpers that keep playtests alive;
10. per-Studio FIFO write queues so concurrent agents cannot interleave destructive edits;
11. request idempotency and retry rules at the relay boundary;
12. payload/result size limits and short-lived snapshot caches.

## Repository layout

- `apps/mcp-worker/` — Cloudflare Worker + Durable Object relay.
- `apps/windows-agent/` — outbound Windows bridge that owns the native Roblox Studio MCP process.
- `docs/` — protocol, optimization notes and rollout plan.
- `windows/` — local-truth/self-healing policy.

## Safety rule

Do not auto-kill or continuously restart the Windows agent. One watcher per PC may start it only when it is absent. A healthy native MCP or agent must be left alone.
