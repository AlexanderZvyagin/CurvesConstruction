<script>
    export let data = [];
</script>

{#if data.length===0}
    <p>There are no quotes.</p>
{:else}
    <header>{data.length} Quotes</header>
    <main>
        <div class="info">
            Check for the <span>Ticker/Name</span> definition <a href="https://www.openfigi.com/search#!?page=1">here</a>.
        </div>
        <div class="quotes-list">
            <div class='name header'>Ticker/Name</div>
            <div class='type header'>Type</div>
            <div class='quote header'>Quote</div>
            <div class='descr header'>Description</div>
            {#each data as quote}
                <div class='name item'> {quote.name||''} </div>
                <div class='type item'> {quote.type||''} </div>
                <div class='quote item'> {Number(quote.quote).toFixed(4)} </div>
                <div class='descr item'> {quote.descr||''} </div>
            {/each}
        </div>
    </main>
{/if}

<style>
.info{
    margin: 15px;
}
.info span{
    color: blue;
}
header{
    text-align: center;
    font-size: 2em;
    /* position: sticky;
    top: 0px; */
}
.header{
    /* position: sticky;
    top: 40px; */
    color: blue;
}
.item{
    /* position: sticky;
    top: 60px; */
    color: green;
}
.quotes-list{
    display: grid;
    grid-template-columns: 1fr 1fr 1fr 3fr;
    grid-gap: 10px;
    margin: 0 auto;
    max-height: 500px;
    overflow: scroll;
}
</style>
