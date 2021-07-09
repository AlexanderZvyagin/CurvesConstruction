<script>
    export let data = {x:[],y:[]};
    import Scatter from "svelte-chartjs/src/Scatter.svelte"
    let plot_data;

    $:{
        console.log(data);
        if(data.x && data.y){
            plot_data = {
                datasets: [{
                }]
            };
            plot_data.datasets[0].data = data.x.map((x,i)=>{
                return {x,y:data.y[i]};
            });
        }
        console.log('plot_data:');
        console.log(plot_data);
    }

    // $:{
    //     data = {
    //         // labels: labels,
    //         datasets: [{
    //             // label: 'My First Dataset',
    //           data: vt.map(v=>{return {x:v,y:0,r:20};}),
    //           // fill: false,
    //           // borderColor: 'rgb(75, 192, 192)',
    //           // tension: 0.1
    //         }]
    //     };
    // }
</script>


<h1>Show Curve!</h1>
<div class='canvas'>
<Scatter data={plot_data}/>
</div>
<!-- <Line {data} /> -->

<style>
.canvas{
    /* width: 600px; */
    /* height: 600px; */
    /* border: solid; */
}
</style>
