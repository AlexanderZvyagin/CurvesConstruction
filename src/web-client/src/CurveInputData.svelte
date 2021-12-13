<script>
    import {debug,info,warn,error} from './Logs.svelte'
    import {curve_instruments,quotes} from './store.js'
    import {format_months} from './instruments.js'

    let fra_start=0, fra_length=0;

    function fra (start,length) {
        return {type:'FRA',start:fra_start,length:fra_length,use:true};
    }

    function fra_compare (a,b) {
        if(a.type!=='FRA' || b.type!=='FRA' ) return 1;
        if(a.start<b.start) return -1;
        if(a.start>b.start) return  1;
        if(a.length<b.length) return -1;
        if(a.length>b.length) return  1;
        return 0;
    }

    function fra_set_quote (fra) {
        const q = $quotes.find(v=>v.type==='FRA'&&v.start===fra.start&&v.length===fra.length);
        fra.quote = q ? q.quote : undefined;
        return fra;
    }

    function fra_add () {
        if(fra_start<0 || fra_length<0 || (fra_start+fra_length)<=0 ){
            warn(`bad start=${fra_start} and/or length=${fra_length} arguments`);
            return;
        }
        const
            a = fra(fra_start,fra_length),
            b = $curve_instruments.find(b=>fra_compare(a,b)==0);
        if(b)
            return;
        fra_set_quote(a);
        curve_instruments.update(current=>{
            let v = [...current,a];
            v.sort(fra_compare);
            return v;
        });
    }

    function fra_del (a) {
        let index = $curve_instruments.findIndex(b=>fra_compare(a,b)==0);
        if(index>=0)
            curve_instruments.update(v=>{
                return [...v.slice(0,index),...v.slice(index+1,v.length)];
            });
        else{
            warn('fra_del: not found:',v);
        }
    }

    let swap_length=1, swap_leg1=1, swap_leg2=1;

    function swap (length,leg1,leg2) {
        return {type:'Swap',length:swap_length,leg1:swap_leg1,leg2:swap_leg2,use:true};
    }

    function swap_compare (a,b) {
        if(a.type!=='Swap' || b.type!=='Swap' ) return 1;
        if(a.length<b.length) return -1;
        if(a.length>b.length) return  1;
        if(a.leg1<b.leg1) return -1;
        if(a.leg1>b.leg1) return  1;
        if(a.leg2<b.leg2) return -1;
        if(a.leg2>b.leg2) return  1;
        return 0;
    }

    function swap_set_quote (swap) {
        const q = $quotes.find(v=>v.type==='Swap'&&v.length===swap.length&&v.leg1===swap.leg1&&v.leg2===swap.leg2);
        swap.quote = q ? q.quote : undefined;
        return swap;
    }

    function swap_add () {
        if(swap_length<=0 || swap_leg1<=0 || swap_leg2<=0 ){
            warn(`bad swap ignore`);
            return;
        }
        const
            a = swap(swap_length,swap_leg1,swap_leg2),
            b = $curve_instruments.find(b=>swap_compare(a,b)==0);
        if(b)
            return;
        swap_set_quote(a);
        curve_instruments.update(current=>{
            let v = [...current,a];
            v.sort(swap_compare);
            return v;
        });
    }

    function swap_del (a) {
        let index = $curve_instruments.findIndex(b=>swap_compare(a,b)==0);
        if(index>=0)
            curve_instruments.update(v=>{
                return [...v.slice(0,index),...v.slice(index+1,v.length)];
            });
        else{
            warn('swap_del: not found:',v);
        }
    }

    $:{
        // this variable is unused,
        // but it is neededd to trigger a reaction on quotes update
        const quotes_length = $quotes.length;
        curve_instruments.update(cur=>cur.map(v=>{
            if(v.type==='FRA')
                fra_set_quote(v);
            else if(v.type==='Swap')
                swap_set_quote(v);
            return v;
        }));
    }

    function export_data1 () {
//        console.log('export');
//        let fname = 'data.json';
        
        var a = document.createElement('a');
        a.innerHTML = "Click here";
        a.href     = 'data:application/json;charset=utf-8,' + encodeURIComponent(JSON.stringify($curve_instruments));
//        a.target   = '_blank';
//        a.download = fname;
        document.body.appendChild(a);
        
    }


    function export_data () {
        console.log('saving');
        var myBlob = new Blob(["CONTENT"], {type: 'text/plain'});
        var anchor = document.createElement("a");
        anchor.download = "demo.txt";
        anchor.url = window.URL.createObjectURL(myBlob);
        anchor.click(); 
//        document.body.appendChild(anchor);
    }

</script>

<table>
    <div class="grid-container">
        {#each $curve_instruments as v}
            <div>
                {#if v.type==='FRA'}
                    <button on:click|preventDefault={()=>fra_del(v)}>del</button>
                {:else if v.type==='Swap'}
                    <button on:click|preventDefault={()=>swap_del(v)}>del</button>
                {/if}
            </div>
            <div>
                <input type=checkbox bind:checked={v.use}/>
            </div>
            <div>
            {#if v.type==='FRA'}
                FRA {format_months(v.start)}x{format_months(v.length)}
            {:else if v.type==='Swap'}
                Swap {format_months(v.length)} ({format_months(v.leg1)} vs {format_months(v.leg2)})
            {:else}
                {v.type}
            {/if}
            </div>
            <div>
                <input class="quote" type=number value={v.quote} on:change={x=>{v.quote=x.target.value;}} min="any" max="any" step="any"/>
            </div>
        {/each}
    </div>
</table>
<button on:click={()=>console.log($curve_instruments)}>Export data</button>
<textarea rows="4" cols="50">
{JSON.stringify($curve_instruments)}
</textarea>

<form class='fra-add' on:submit|preventDefault={fra_add}>
    <button>Add FRA:</button>
    <label for='start'>Start</label>
    <input class="period" id='start' type=number bind:value={fra_start} step=1 min=0>
    <label for='length'>Length</label>
    <input class="period" id='length' type=number bind:value={fra_length} step=1 min=0>
</form>

<form class='swap-add' on:submit|preventDefault={swap_add}>
    <button>Add Swap:</button>
    <label for='length'>Length</label>
    <input class="period" id='length' type=number bind:value={swap_length} step=1 min=1>
    <label for='leg1'>Leg1</label>
    <input class="period" id='leg1' type=number bind:value={swap_leg1} step=1 min=1>
    <label for='leg2'>Leg2</label>
    <input class="period" id='leg2' type=number bind:value={swap_leg2} step=1 min=1>
</form>

<style>
.fra-add label,input{
    display: inline;
}
.swap-add label,input{
    display: inline;
}
.grid-container{
    display: grid;
    grid-template-columns: 1fr 1fr 4fr 2fr;
    max-height: 300px;
    background-color: lightblue;
    overflow: scroll;
}
input.quote {
    max-width: 100px;
}
input.period {
    max-width: 60px;
}
</style>
