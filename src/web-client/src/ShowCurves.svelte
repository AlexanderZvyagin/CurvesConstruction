<script>
    import {curves} from './store.js';
    // import Scatter from "svelte-chartjs/src/Scatter.svelte"
    // import Line    from "svelte-chartjs/src/Line.svelte"
    import ChartJS from "svelte-chartjs/src/Base.svelte"

    let
        data = {
            datasets: [],
        },
        options = {
            elements:{
                point:{
                    pointStyle:'cross'
                }
            },
            title:{
                display: true,
                text:'Instantaneous rate'
            }
        };

    // function get_color

    $:{
        data.datasets = $curves.filter(c=>c.reply).map(c=>({
            label:c.request.curve_name,
            data:c.reply.plot.x.map((x,i)=>({x,y:c.reply.plot.y[i]})),
            // backgroundColor:'rgb(220,220,220,0.5)',
            borderColor: "#3e95cd",
            markerType: "triangle",
            // borderWidth:1,
            // fill: true,
        }));
        console.log(data);
    }
</script>

<!-- <header>
    <h1>Show {$curves.length} Curves!!</h1>
</header> -->

<main>
    <button on:click={()=>curves.update(()=>[])}>Remove all</button>
    <div class='canvas'>
        <ChartJS type="scatter" {data}/>
    </div>
</main>

<style>
.canvas{
    width: 100%;
    /* max-width: 1200; */
    /* height: 600px; */
    border: solid;
}
</style>
