import { writable } from 'svelte/store'

function booleanStore(initial) {
  const isOpen = writable(initial)
  const { set, update } = isOpen
  return {
    isOpen,
    open: () => set(true),
    close: () => set(false),
    toggle: () => update((n) => !n),
  }
}

export {booleanStore}
