# Relay Protocol V8

All frames are JSON. The Windows agent opens the only outbound connection.

## Agent hello

```json
{
  "type": "hello",
  "protocol": 8,
  "installation_id": "...",
  "agent_version": "0.2.0",
  "machine_alias": "...",
  "native": {
    "online": true,
    "tool_generation": 3,
    "tool_count": 28
  },
  "studios": [{"id":"...","name":"..."}]
}
```

## Request

```json
{
  "type": "request",
  "id": "uuid",
  "studio_id": "uuid-or-null",
  "method": "tools/call",
  "params": {},
  "deadline_ms": 15000,
  "idempotency_key": "stable-for-retries"
}
```

## Response

```json
{
  "type": "response",
  "id": "same-request-id",
  "ok": true,
  "result": {},
  "duration_ms": 12
}
```

## Event

```json
{
  "type": "event",
  "seq": 1042,
  "kind": "native.tools_changed",
  "studio_id": null,
  "payload": {}
}
```

## Heartbeat

A heartbeat replaces multiple status polls.

```json
{
  "type": "heartbeat",
  "seq": 1043,
  "native_online": true,
  "tool_generation": 3,
  "studios": [{"id":"...","name":"..."}],
  "inflight": 0
}
```

## Ordering

- Mutations are FIFO per `studio_id`.
- Read-only requests may run concurrently up to the configured read window.
- Requests without `studio_id` are limited to connection/global operations.
- A fresh Studio list is required before write operations after reconnect.

## Retry

- Reads: retry once on transient transport loss.
- Mutations: retry only with an idempotency key.
- Native MCP semantic errors are never retried automatically.
