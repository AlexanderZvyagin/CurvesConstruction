<script>
    import {curves} from './store.js';
    import Scatter from "svelte-chartjs/src/Scatter.svelte"
    let data = {datasets: []};

    $:{
        data.datasets = $curves.filter(c=>c.reply).map(c=>({
            label:c.request.curve_name,
            data:c.reply.plot.x.map((x,i)=>({x,y:c.reply.plot.y[i]})),
        }));
    }
</script>

<!-- <header>
    <h1>Show {$curves.length} Curves!!</h1>
</header> -->

<main>
    <button on:click={()=>curves.update(()=>[])}>Remove all</button>
    <div class='canvas'>
        <Scatter {data}/>
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
