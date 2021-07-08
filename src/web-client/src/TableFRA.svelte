<script>
    // import {onMount,onDestroy} from 'svelte';
    import SvelteTable from 'svelte-table';
    import {curve_instruments} from './store.js';
    // import CheckBox from "./CheckBox.svelte";
    // import ShowFRA from './ShowFRA.svelte';
	// export let data;

    // const unsubscriber = curve_instruments.subscribe(v=>{
    //     console.log('curve_instruments updated!');
    //     data = v;
    // });

    // onMount(()=>{
    //     console.log('onMount: TableFRA');
    // });
    //
    // onDestroy(()=>{
    //     console.log('onDestroy: TableFRA');
    //     unsubscriber();
    // });

    // let fra_instruments = [];// name, start, length
    //
    // let fras = [], fras_length=0;

    // function add_fra(fra) {
    //     const found=fras.find(v=>v.start==fra.start && v.length==fra.length);
    //     if(!found)
    //         fras.push(fra);
    // }

    // $:{
    //     console.log(data.length);
    //     for(let i in data){
    //         // console.log(data[i]);
    //         // let v = instrument_info(data[i][0]);
    //         // if(v.type==='FRA'){
    //         //     // console.log(v);
    //         //     add_fra(v);
    //         //     console.log(fras.length);
    //         // }
    //     }
    //     fras_length = fras.length;
    // }
    // console.log(data);
    // console.log('aaaaaaaaaaaaaaaa',data.length);
    // let data2 = [];
    // $:{
    //     data2 = data.map( (item,index)=>{return {...item,id:index};} );
    // }
    // // console.log(data2);

    // const onCheck = row => console.log(`${row}`);

    const h1 = e => console.log(e);

    function click_cell ({key,row}) {
        if(key==='use'){
            const index = $curve_instruments.findIndex(v=>v.id==row.id);
            curve_instruments.update(current=>{
                const item = current[index];
                item.use = !item.use;
                return current;
                // return [
                //     ...current.splice(0,index),
                //     item,
                //     ...current.splice(index+1,current.length)
                // ];
            });
        }
    }

    const columns = [{
    //     key: "use",
    //     title: "Use",
    //     sortable: true,
    //     renderValue: v => `<input type=checkbox on:change={{h1}}/>`,
    // },{
        key: "use",
        title: "Use",
        value: v => v.use,
        sortable: true,
        renderValue: v => {
            if(v.use)
                return `<strong>USE</strong>`;
            else
                return `skip`;
        },
    },{
        key: "start",
        title: "Start",
        value: v => v.start,
        // filterValue: v => v.start,
        sortable: true,
        headerClass: "text-left",
        // renderComponent: <input type="checkbox"/>
        // renderValue: v => {
        //     return `<span class="start">${v.start*1}</span>`;
        // },
        class: "fra-start",
    },{
        key: "length",
        title: "Length",
        value: v => v.length,
        // filterValue: v => v.length,
        sortable: true,
        headerClass: "text-left"
    },{
        key: "maturity",
        title: "Maturity",
        value: v => v.start+v.length,
        // filterValue: v => v.start+v.length,
        sortable: true,
        headerClass: "text-left"
    },{
        key: "ticker",
        title: "ticker",
        value: v => v.name,
        // filterValue: v => v.name,
        sortable: true,
        headerClass: "text-left"
    }];

</script>

<h4>Table-FRA of {$curve_instruments.length} items</h4>
<!-- <div>
{#each $curve_instruments as instr}
{/each}
</div> -->
<div>
    <p>The units are months. maturity = start+length.</p>
    <!--
    on:clickCell={event=>console.log('clickCell',event.detail)}
    on:clickRow={event=>console.log('clickRow',event.detail)}
    -->
    <SvelteTable
        columns={columns}
        rows={$curve_instruments}
        on:clickCell={event=>click_cell(event.detail)}
    />
</div>

<style>
/* .fra-start{
    color: green;
} */
.text-left {
    text-align: right;
  }
</style>
