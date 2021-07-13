<script>
    import {createEventDispatcher} from 'svelte';
    export let items=[], active;
    const dispatch = createEventDispatcher();
</script>

<div class="tabs">
    <ul>
        {#each items as item}
        <li on:click={()=>dispatch('tabChanged',item)}>
            <div class:active={item===active}>{item}</div>
        </li>
        {/each}
    </ul>
</div>

<style>
.active{
    color: lightcoral;
    border-bottom: 2px solid;
    padding-bottom: 8px;
}
.tabs{
    margin-bottom: 40px;
    background-color: rgb(244, 254, 226);
}
ul{
    display: flex;
    justify-content: center;
    padding: 0;
    list-style-type: none;
}
li{
    margin:0 16px;
    color: gray;
    background-color: rgb(244, 254, 226);
}
</style>
