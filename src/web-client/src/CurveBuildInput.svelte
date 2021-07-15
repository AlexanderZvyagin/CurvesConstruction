<script>
    import {createEventDispatcher, onMount} from 'svelte';
    import {debug,info,warn,error} from './Logs.svelte'
    import {quotes,curve_instruments,interpolation_type,date} from './store.js';
    import CurveInputData from './CurveInputData.svelte';
    import Modal from './components/Modal.svelte'
    import QuotesTable from './QuotesTable.svelte'

    let dispatch = createEventDispatcher();

    onMount(() => {
        // ... Run something after component has mounted
        dispatch('newDate',$date);
    });

    function set_quote(instr,quote) {
        if(instr.type==='FRA'){
            const q = $quotes.find(v=>v.type==='FRA'&&v.start===instr.start&&v.length===instr.length);
            instr.quote = q ? q.quote : undefined;
        }else
        if(instr.type==='Swap'){
            const q = $quotes.find(v=>v.type==='Swap'&&v.length===instr.length&&v.leg1===instr.leg1&&v.leg2===instr.leg2);
            instr.quote = q ? q.quote : undefined;
        }
        else
            warn(`set_quote: unknown instrument ${instr}`);
        return instr;
    }


    let
        M=1, Y=12*M,
        curve_name='',
        data_template = '',
        data_templates = {
            '3 FRAs starting at 0':[
                {type:'FRA',use:true,start: 0,length: 1},
                {type:'FRA',use:true,start: 0,length: 6},
                {type:'FRA',use:true,start: 0,length:12},
            ],
            'many FRAs':[
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
            ],
            '2 FRAs, 2 Swaps':[
                {type:'FRA',use:true,start: 0,length: 1},
                {type:'FRA',use:true,start: 0,length: 6},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 1*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 2*Y},
            ],
            'many':[
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
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 1*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 2*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 3*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 4*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 5*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 6*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 7*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 8*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length: 9*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:10*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:13*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:14*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:15*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:20*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:30*Y},
                {type:'Swap',use:true,leg1:1*Y,leg2:6*M,length:60*Y},
        ]};

    const instruments_to_use = v => v.use && isNaN(Number(v.quote))===false;
    const quote_norm = v => ({...v,quote:v.quote*1e-2});

    function handle_submit () {
        dispatch('buildCurve',{
            interpolation_type:$interpolation_type,
            date:$date,
            instruments:$curve_instruments.filter(instruments_to_use).map(quote_norm),
            curve_name
        });
    }

    function newDate (e) {
        const d = e.target.value;
        date.update(cur=>d);
        dispatch('newDate',d);
    }

    function set_data_template (name) {
        if(!name)
            name = Object.keys(data_templates) [0];
        data_template = name;
        debug('set_data_template',name);
        curve_instruments.update(()=>data_templates[name].map(set_quote));
    }

    set_data_template()

</script>

<header>
    <h1>Options & Data</h1>
</header>

<main>
    <form on:submit|preventDefault={handle_submit}>
        <div class="date">
            <label for="date">Date</label>
            <input id="date" type="date" name="date" min="2011-01-01" max="2019-08-01" value={$date} on:change={newDate}/>
            <div class="quotes">
                <Modal>
                    <div slot="trigger" let:open>
                        <!-- <button on:click|preventDefault={open}>Show</button> -->
                        <span class="show-quotes" on:click|preventDefault={open}>There are {$quotes.length} quotes available.</span>
                    </div>
                    <div slot="header"></div>
                    <div slot="content">
                        <QuotesTable data={$quotes}/>
                    </div>
                    <div slot="footer" let:store={{close}}>
                        <button on:click={close}>Close</button>
                    </div>
                </Modal>
            </div>
        </div>
        <div class='interpolation'>
            <label for='interpolation'>Interpolation</label>
            <select id='interpolation' value={$interpolation_type} on:change={e=>interpolation_type.update(cur=>e.target.value)}>
                <option value="PiecewiseConstant">PiecewiseConstant</option>
                <option value="Linear">Linear</option>
                <option value="CubicSpline">CubicSpline</option>
                <option value="Steffen">Steffen</option>
            </select>
        </div>
        <div class='data-template'>
            <label for='data-template'>Data Template</label>
            <select id='data-template' value={data_template} on:change={e=>set_data_template(e.target.value)}>
                {#each Object.keys(data_templates) as name}
                    <option value="{name}">{name}</option>
                {/each}
            </select>
        </div>
        <CurveInputData/>
        <label for="curve_name">Curve Name</label>
        <input id="curve_name" type=text bind:value={curve_name}/>
    	<button>Build The Curve</button>
    </form>
</main>

<style>
header h1{
    width: 100%;
    text-align: center;
}
input{
    max-width: 100px;
}
.date{
    display: inline-block;
    border: dotted 1px gray;
    border-radius: 11px;
}
/* .interpolatioin{
    display: inline-block;
    border: dotted 1px gray;
    border-radius: 11px;
} */
.quotes{
    display: inline-block;
}
.show-quotes{
    text-decoration: underline;
}
.interpolation{
    display: inline-block;
}
.data-template{
    display: inline-block;
}
</style>
