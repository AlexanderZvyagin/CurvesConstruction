<script>
    import {createEventDispatcher} from 'svelte';
    const dispatch = createEventDispatcher();
    export let items=[], active;
</script>

<div class='menu'>
    {#each items as item}
        <div
            class='menu-item'
            class:selected={item===active}
            on:click={()=>dispatch('click',item)}
            on:hover={()=>console.logs('hover',item)}
        >
            {item}
        </div>
    {/each}
</div>

<style>
/* https://www.w3schools.com/w3css/w3css_color_themes.asp */
.w3-theme {
color:#fff !important;background-color:#3f51b5 !important;}
.w3-theme-light {
color:#000 !important;background-color:#e8eaf6 !important;}
.w3-theme-dark {
color:#fff !important;background-color:#1a237e !important;}
.w3-theme-l5 {
color:#000 !important;background-color:#e8eaf6 !important;}
.w3-theme-l4 {
color:#000 !important;background-color:#c5cae9 !important;}
.w3-theme-l3 {
color:#000 !important;background-color:#9fa8da !important;}
.w3-theme-l2 {
color:#fff !important;background-color:#7986cb !important;}
.w3-theme-l1 {
color:#fff !important;background-color:#5c6bc0 !important;}
.w3-theme-d1 {
color:#fff !important;background-color:#3949ab !important;}
.w3-theme-d2 {
color:#fff !important;background-color:#303f9f !important;}
.w3-theme-d3 {
color:#fff !important;background-color:#283593 !important;}
.w3-theme-d4 {
color:#fff !important;background-color:#1a237e !important;}
.w3-theme-action {
color:#fff !important;background-color:#311b92 !important;}
.w3-text-theme {
color:#1a237e !important;}

.menu{
    display: flex;
    flex-wrap: wrap;
    justify-content: space-between;
    align-items: left;
    padding: 1ch 10%;
    color:#000 !important;background-color:#e8eaf6 !important;
    /* height: 3ch; */
}
.menu-item {
    margin: 0 1ch;
    padding: 0.33ch;
    color:#000 !important;background-color:#e8eaf6 !important;
    border: 0.25ch solid #e8eaf6;
    cursor: pointer;
}
.menu-item:hover {
    padding: 0.33ch;
    border: 0.25ch solid #303f9f;
    border-radius: 1ch;
}
.selected {
    border: 0.25ch solid #9fa8da;
    border-radius: 1ch;
    color:#fff !important;background-color:#9fa8da !important;
}
</style>
