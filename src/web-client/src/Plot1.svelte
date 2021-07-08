<script>
    // import Line from "svelte-chartjs/src/Line.svelte"
    // import Bar from "svelte-chartjs/src/Bar.svelte"
    // import Scatter from "svelte-chartjs/src/Scatter.svelte"
    import Bubble from "svelte-chartjs/src/Bubble.svelte"
    import {curve_instruments} from './store.js';

    let vt = [], data;
    $:{
        vt = $curve_instruments.filter(v=>v.use).map(v=>v.start+v.length);
        vt.push(0);
        vt = [...new Set(vt)].sort();
        console.log(vt);
        data = {
            // labels: labels,
            datasets: [{
                // label: 'My First Dataset',
              data: vt.map(v=>{return {x:v,y:0,r:20};}),
              // fill: false,
              // borderColor: 'rgb(75, 192, 192)',
              // tension: 0.1
            }]
        };
    }
</script>


<h1>Plot1</h1>
<Bubble {data}/>
<!-- <Line {data} /> -->

<style>
</style>
