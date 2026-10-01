export interface RelayEvent<T = unknown> {
  seq: number;
  at: number;
  kind: string;
  studioId?: string;
  payload: T;
}

export class EventJournal {
  readonly capacity: number;
  #seq = 0;
  #events: RelayEvent[] = [];

  constructor(capacity = 2048) {
    if (!Number.isInteger(capacity) || capacity < 32) {
      throw new Error("capacity must be an integer >= 32");
    }
    this.capacity = capacity;
  }

  get lastSeq(): number {
    return this.#seq;
  }

  push<T>(kind: string, payload: T, studioId?: string): RelayEvent<T> {
    const event: RelayEvent<T> = {
      seq: ++this.#seq,
      at: Date.now(),
      kind,
      payload,
      ...(studioId ? { studioId } : {}),
    };
    this.#events.push(event);
    const overflow = this.#events.length - this.capacity;
    if (overflow > 0) this.#events.splice(0, overflow);
    return event;
  }

  since(seq: number, kinds?: ReadonlySet<string>, limit = 256): {
    events: RelayEvent[];
    lastSeq: number;
    dropped: number;
  } {
    const oldest = this.#events[0]?.seq ?? this.#seq + 1;
    const dropped = seq < oldest - 1 ? oldest - 1 - seq : 0;
    const events: RelayEvent[] = [];
    for (const event of this.#events) {
      if (event.seq <= seq) continue;
      if (kinds && !kinds.has(event.kind)) continue;
      events.push(event);
      if (events.length >= limit) break;
    }
    return { events, lastSeq: this.#seq, dropped };
  }
}
