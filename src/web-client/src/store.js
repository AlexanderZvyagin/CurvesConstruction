import {writable} from 'svelte/store'

let
    curve_instruments = writable([
        {type:'FRA',use:true,start: 0,length: 1},
        {type:'FRA',use:true,start: 0,length: 2},
        {type:'FRA',use:true,start: 0,length: 3},
        {type:'FRA',use:true,start: 0,length: 6},
        {type:'FRA',use:true,start: 0,length: 9},
        {type:'FRA',use:true,start: 0,length:12},
        {type:'FRA',use:true,start: 1,length:12},
        {type:'FRA',use:true,start: 3,length: 6},
        {type:'FRA',use:true,start:12,length:12},
    ]),
    quotes = writable([]),
    curves = writable([]);

export {curve_instruments, quotes, curves};
