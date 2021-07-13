import {writable} from 'svelte/store'

const M=1, Y=12*M;

export let
    logs =
        writable([]),
    interpolation_type =
        writable('Steffen'),
    date =
        writable("2018-02-02"),
    curve_instruments = writable([
        {type:'FRA',use:true,start: 0,length: 1},
        // {type:'FRA',use:true,start: 0,length: 2},
        // {type:'FRA',use:true,start: 0,length: 3},
        {type:'FRA',use:true,start: 0,length: 6},
        // {type:'FRA',use:true,start: 0,length: 9},
        // {type:'FRA',use:true,start: 0,length:12},
        // {type:'FRA',use:true,start: 1,length:12},
        // {type:'FRA',use:true,start: 1,length:13},
        // {type:'FRA',use:true,start: 2,length: 5},
        // {type:'FRA',use:true,start: 2,length:14},
        // {type:'FRA',use:true,start: 3,length: 6},
        // {type:'FRA',use:true,start: 3,length:15},
        // {type:'FRA',use:true,start: 5,length:17},
        // {type:'FRA',use:true,start: 6,length: 9},
        // {type:'FRA',use:true,start: 6,length:18},
        // {type:'FRA',use:true,start: 7,length:13},
        // {type:'FRA',use:true,start: 9,length:21},
        // {type:'FRA',use:true,start:11,length:14},
        // {type:'FRA',use:true,start:11,length:17},
        // {type:'FRA',use:true,start:12,length:12},
        // {type:'FRA',use:true,start:12,length:18},
        {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 1*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 2*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 3*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 4*Y},
        {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 5*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 6*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 7*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 8*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 9*Y},
        // {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:10*Y},
    ]),
    quotes =
        writable([]),
    curves =
        writable([]);
