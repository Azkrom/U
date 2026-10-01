export interface ToolDescriptor {
  name: string;
  description?: string;
  inputSchema?: unknown;
  annotations?: unknown;
}

export class NativeToolCache {
  #generation = -1;
  #tools: ToolDescriptor[] = [];

  get generation(): number {
    return this.#generation;
  }

  get tools(): readonly ToolDescriptor[] {
    return this.#tools;
  }

  update(generation: number, tools: ToolDescriptor[]): boolean {
    if (generation < this.#generation) return false;
    const changed = generation !== this.#generation;
    this.#generation = generation;
    this.#tools = tools;
    return changed;
  }

  invalidate(): void {
    this.#generation = -1;
    this.#tools = [];
  }

  needsRefresh(nativeGeneration: number): boolean {
    return this.#generation !== nativeGeneration;
  }
}
