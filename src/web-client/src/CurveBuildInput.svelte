<script>
    import {createEventDispatcher, onMount} from 'svelte';
    import {quotes,curve_instruments} from './store.js';
    import CurveInputData from './CurveInputData.svelte';
    import Modal from './components/Modal.svelte'
    import QuotesTable from './QuotesTable.svelte'

    let dispatch = createEventDispatcher();

    let
        interpolation_type = 'PieceWise',
        date = "2018-02-02";
    onMount(() => {
        // ... Run something after component has mounted
        dispatch('newDate',date);
    });

    let curve_name='';

    function handle_submit () {
        dispatch('buildCurve',{
            interpolation_type,
            date,
            instruments:$curve_instruments.filter(v=>v.use&&isNaN(Number(v.quote))===false),
            curve_name
        });
    }

    $:{
        dispatch('newDate',date);
    }
</script>

<header>
    <h1>Options & Data</h1>
</header>

<main>
    <form on:submit|preventDefault={handle_submit}>
        <div class="date">
            <label for="date">Date</label>
            <input id="date" type="date" name="date" min="2011-01-01" max="2019-08-01" bind:value={date}/>
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
            <select id='interpolation' bind:value={interpolation_type}>
                <option value="PieceWise">PieceWise</option>
                <option value="Linear">Linear</option>
                <option value="CubicSpline">CubicSpline</option>
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
</style>
