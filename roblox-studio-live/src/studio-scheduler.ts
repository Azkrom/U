type Task<T> = () => Promise<T>;

class Semaphore {
  #available: number;
  #waiters: Array<() => void> = [];

  constructor(size: number) {
    this.#available = Math.max(1, size);
  }

  async acquire(): Promise<() => void> {
    if (this.#available > 0) {
      this.#available--;
      return () => this.release();
    }
    await new Promise<void>((resolve) => this.#waiters.push(resolve));
    return () => this.release();
  }

  private release(): void {
    const next = this.#waiters.shift();
    if (next) next();
    else this.#available++;
  }
}

export class StudioScheduler {
  #writeTails = new Map<string, Promise<unknown>>();
  #reads: Semaphore;

  constructor(readConcurrency = 6) {
    this.#reads = new Semaphore(readConcurrency);
  }

  async read<T>(task: Task<T>): Promise<T> {
    const release = await this.#reads.acquire();
    try {
      return await task();
    } finally {
      release();
    }
  }

  write<T>(studioId: string, task: Task<T>): Promise<T> {
    if (!studioId) throw new Error("studioId is required for writes");
    const previous = this.#writeTails.get(studioId) ?? Promise.resolve();
    const next = previous.catch(() => undefined).then(task);
    this.#writeTails.set(studioId, next);

    next.finally(() => {
      if (this.#writeTails.get(studioId) === next) {
        this.#writeTails.delete(studioId);
      }
    }).catch(() => undefined);

    return next;
  }
}
