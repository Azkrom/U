# Local Truth Policy V8

1. Exactly one Windows agent owns the local native Roblox Studio MCP transport.
2. Exactly one watchdog instance may supervise that agent.
3. The watchdog never kills a healthy agent just because a cloud request failed.
4. The agent never restarts Roblox Studio.
5. The agent reconnects outbound to the Worker; no inbound router port is required.
6. The relay considers local transport healthy only when both agent and native MCP are healthy.
7. A tool/harness failure is not proof that the local agent is offline.
8. After reconnect, refresh sessions once and native tools only when the native generation changed.
9. Never cache a write target across Studio reconnects. Resolve a fresh `studio_id`.
10. Secrets are never printed, returned by diagnostics, or committed.
