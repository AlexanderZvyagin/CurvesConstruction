<script>
    import {debug,info,warn,error} from './Logs.svelte'
    import {curve_instruments,quotes} from './store.js';

    let fra_start=0, fra_length=0;

    function fra (start,length) {
        return {type:'FRA',start:fra_start,length:fra_length,use:true};
    }

    function fra_compare (a,b) {
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

    $:{
        // this variable is unused,
        // but it is neededd to trigger a reaction on quotes update
        const quotes_length = $quotes.length;
        curve_instruments.update(cur=>cur.map(fra=>fra_set_quote(fra)));
    }
</script>

<table>
    <tr>
        <th></th>
        <th>Use</th>
        <th>Instrument</th>
        <th>Quote</th>
    </tr>
    <div class="container">
    {#each $curve_instruments as v}
        <tr>
            <td>
                <button on:click|preventDefault={()=>fra_del(v)}>del</button>
            </td>
            <td>
                <input type=checkbox bind:checked={v.use}/>
            </td>
            <td>
                FRA {v.start}x{v.length}
            </td>
            <td>
                <input type=number value={v.quote} on:change={x=>{v.quote=x.target.value;}} min="any" max="any" step="any"/>
                <!-- <input type=number bind:value={v.quote} min="any" max="any" step="any"/> -->
            </td>
        </tr>
    {/each}
    </div>
</table>

<form class='fra-add' on:submit|preventDefault={fra_add}>
    <button>Add FRA:</button>
    <label for='start'>Start</label>
    <input id='start' type=number bind:value={fra_start}>
    <label for='length'>Length</label>
    <input id='length' type=number bind:value={fra_length}>
</form>

<style>
.fra-add label,input{
    display: inline;
}
.container{
    height: 300px;
    background-color: lightblue;
    overflow: scroll;
}
td input {
    background-color: lightblue;
}
</style>
