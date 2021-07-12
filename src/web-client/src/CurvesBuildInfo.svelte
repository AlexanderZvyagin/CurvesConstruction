<script>
    import {curves} from './store.js';
    $:{
        console.log('CurvesBuildInfo',$curves.length);
        console.log($curves[0]);
    }

    const num = v=> v.toPrecision(4);

</script>

<header>
    {$curves.length} curves.
</header>

<main>
    <div class="container">
    {#each $curves as curve}
        <div class="curve-info">
            <div>name:{curve.request.curve_name}</div>
            <div>interpolation:{curve.request.interpolation_type}</div>
            <div>#instruments:{curve.request.instruments.length}</div>
        </div>
        {#if curve.reply}
            <div class="container2">
                <div class="header">Type</div>
                <div class="header">Instrument</div>
                <div class="header">Quote</div>
                <div class="header">Computed</div>
                <div class="header">Diff</div>
            {#each curve.reply.result.instruments as v}
                <div>{v.type}</div>
                <div>{v.about}</div>
                <div>{num(v.quote)}</div>
                <div>{num(v.computed)}</div>
                <div>{num(v.quote-v.computed)}</div>
            {/each}
            </div>
        {/if}
    {/each}
    </div>
</main>

<style>

.container2 {
    display: grid;
    /* background: rgb(255, 185, 249); */
    grid-template-columns: 1fr 3fr 1fr 1fr 1fr;
    grid-gap: 10px;
}

.curve-info {
    /* border: 4px solid;
    border-color: red;
    border-radius: 10px; */
    margin: 0 auto;
    background: rgb(130, 255, 184);
    display: grid;
    grid-template-columns: repeat(3, 1fr);
}

.curve-info:hover {
    background: gray;
    /* border: 2px dotted; */
    /* padding: 30px; */
}

/* .container div:nth-child(odd){
    background: white;
}
.container div:nth-child(even){
    background: gray;
} */

.container {
    display: grid;
    border: 2px solid red;
    grid-template-columns: repeat(1, 1fr);
    grid-gap: 10px;
}

.header{
    font-weight: bold;
}

</style>
