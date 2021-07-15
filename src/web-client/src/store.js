import {writable} from 'svelte/store'

const M=1, Y=12*M;

export let
    logs =
        writable([]),
    interpolation_type =
        writable('Steffen'),
    date =
        writable("2018-02-02"),
    curve_instruments =
        writable([]),
    quotes =
        writable([]),
    curves =
        writable([]);
