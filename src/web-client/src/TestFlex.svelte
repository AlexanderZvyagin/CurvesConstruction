<script>
    import {createEventDispatcher} from 'svelte';
    import FlexTabs from './components/FlexTabs.svelte';
    const dispatch = createEventDispatcher();
    export let items=[], active;
</script>

<h1 class='title'>even columns</h1>
<div class='even-columns'>
    <div class='col'></div>
    <div class='col'></div>
    <div class='col'></div>
    <div class='col'></div>
</div>

<h1 class='title'>grid-ish</h1>
<div class='grid-ish'>
    <div class='col'></div>
    <div class='col'></div>
    <div class='col'></div>
    <div class='col'></div>
</div>

<h1 class='title'>FlexTabs</h1>
<FlexTabs {items} {active} on:click/>

<style>
.even-columns{
    display: flex;
}
.even-columns > * {
    flex-basis: 100%;
}
.grid-ish{
    display: flex;
    flex-wrap: wrap;
    gap: 0.5ch;
}
.grid-ish > * {
    flex: 1 1 20ch;
}
.col{
    border: 1px solid;
    border-radius: 10px;
    min-width: 100px;
    min-height: 100px;
    background: yellow;
}
</style>
