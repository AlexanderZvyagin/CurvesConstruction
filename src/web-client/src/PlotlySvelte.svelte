<script>
  	import Plotly from 'plotly.js/dist/plotly.js';
  	import { onMount } from 'svelte';
  	console.log(Plotly);

	export let data;
    export let layout;
    export let config;

    let div_for_plot = undefined;
    let Plot = undefined;

    onMount(() => {
      div_for_plot = document.getElementById('plot_svelte');
      Plot = new Plotly.newPlot(div_for_plot, data, layout, config);
    });

    $: if (Plot) Plotly.react(div_for_plot, data, layout, config);

</script>

<div id="plot_svelte" />


<style>
</style>
