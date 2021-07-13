import {writable} from 'svelte/store'

export let
    logs =
        writable([]),
    interpolation_type =
        writable('Steffen'),
    date =
        writable("2018-02-02"),
    curve_instruments = writable([
        {type:'FRA',use:true,start: 0,length: 1},
        {type:'FRA',use:true,start: 0,length: 2},
        {type:'FRA',use:true,start: 0,length: 3},
        {type:'FRA',use:true,start: 0,length: 6},
        {type:'FRA',use:true,start: 0,length: 9},
        {type:'FRA',use:true,start: 0,length:12},
        {type:'FRA',use:true,start: 1,length:12},
        {type:'FRA',use:true,start: 1,length:13},
        {type:'FRA',use:true,start: 2,length: 5},
        {type:'FRA',use:true,start: 2,length:14},
        {type:'FRA',use:true,start: 3,length: 6},
        {type:'FRA',use:true,start: 3,length:15},
        {type:'FRA',use:true,start: 5,length:17},
        {type:'FRA',use:true,start: 6,length: 9},
        {type:'FRA',use:true,start: 6,length:18},
        {type:'FRA',use:true,start: 7,length:13},
        {type:'FRA',use:true,start: 9,length:21},
        {type:'FRA',use:true,start:11,length:14},
        {type:'FRA',use:true,start:11,length:17},
        {type:'FRA',use:true,start:12,length:12},
        {type:'FRA',use:true,start:12,length:18},
    ]),
    quotes =
        writable([]),
    curves =
        writable([]);
