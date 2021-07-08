import {writable} from 'svelte/store'

const
    curve_instruments = writable([]),
    quotes = writable([]);

export {curve_instruments, quotes};
