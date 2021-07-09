<script>
    import {createEventDispatcher, onMount} from 'svelte';
    import CurveInputData from './CurveInputData.svelte';
    import {quotes,curve_instruments} from './store.js';

    let dispatch = createEventDispatcher();

    let
        interpolation_type = 'PieceWise',
        date = "2018-02-02";
    onMount(() => {
        // ... Run something after component has mounted
        // console.log('App component has mounted');
        dispatch('newDate',date);
    });


    function handle_submit () {
        // console.log(data);
        dispatch('buildCurve',{
            interpolation_type,
            date,
            instruments:$curve_instruments.filter(v=>v.use&&isNaN(Number(v.quote))==false),
        });
    }

    $:{
        // console.log(`dispatching with date=${date}`);
        dispatch('newDate',date);
    }
    // console.log('check...');
    // dispatch('newDate',date);
    // console.log('check...done');
    // // dispatch('newDate',date);
</script>

<header>
<h1>Options & Data</h1>
</header>

<main>
    <form on:submit|preventDefault={handle_submit}>
        <div class='date'>
            <label for='date'>Date</label>
            <input id='date' type="date" name="date" min="2011-01-01" max="2019-08-01" bind:value={date}/>
            There are {$quotes.length} quotes available.
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
.interpolatioin{
    display: inline-block;
    border: dotted 1px gray;
    border-radius: 11px;
}
</style>
