export interface CachedResult<T> {
  expiresAt: number;
  value: T;
}

export class RequestDedupe<T> {
  readonly ttlMs: number;
  readonly maxEntries: number;
  #items = new Map<string, CachedResult<T>>();

  constructor(ttlMs = 60_000, maxEntries = 512) {
    this.ttlMs = ttlMs;
    this.maxEntries = maxEntries;
  }

  get(key: string): T | undefined {
    const item = this.#items.get(key);
    if (!item) return undefined;
    if (item.expiresAt <= Date.now()) {
      this.#items.delete(key);
      return undefined;
    }
    // Refresh insertion order for simple LRU behavior.
    this.#items.delete(key);
    this.#items.set(key, item);
    return item.value;
  }

  set(key: string, value: T): void {
    this.#items.delete(key);
    this.#items.set(key, { value, expiresAt: Date.now() + this.ttlMs });
    while (this.#items.size > this.maxEntries) {
      const oldest = this.#items.keys().next().value as string | undefined;
      if (!oldest) break;
      this.#items.delete(oldest);
    }
  }

  prune(now = Date.now()): void {
    for (const [key, item] of this.#items) {
      if (item.expiresAt <= now) this.#items.delete(key);
    }
  }
}
