<script>
    import Quote from './Quote.svelte';
    import {curve_instruments} from './store.js';
    import TableFRA from './TableFRA.svelte';
    import TimeGrid from './TimeGrid.svelte';
    import {instrument_info} from './instruments.js';
    // import {fras_store} from './store.js'
	export let data;
    let fra_instruments = [];// name, start, length

    let fras = [];

    // function add_fra(fra) {
    //     const found=fras.find(v=>v.start==fra.start && v.length==fra.length);
    //     if(!found)
    //         fras.push(fra);
    // }

    $:{
        // console.log(data.length);
        for(let i in data){
            // console.log(data[i]);
            let v = instrument_info(data[i][0]);
            // console.log(v);
            if(v.type==='FRA'){
                // console.log(v);
                const found=$curve_instruments.find(x=>x.type===v.type && x.start===v.start && x.length===v.length);
                if(!found){
                    // fras = [...fras,v];
                    v.id = $curve_instruments.length;
                    v.use = false;
                    curve_instruments.update(current=>[...current,v]);
                }
            }
        }
        // console.log('fras:',fras.length);
    }
</script>

<div>
    <TimeGrid/>
    <TableFRA/>
    <h4>Quotes Table</h4>
    <p>{data.length} quotes</p>
    <div class="quotes-list">
        {#each data as quote}
            <Quote data={quote}/>
        {/each}
    </div>
</div>

<style>
.quotes-list{
    /* background: gray; */
    display: grid;
    grid-template-columns: 1fr 1fr;
    /* grid-gap: 20px; */
}
</style>
