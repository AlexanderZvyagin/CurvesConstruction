<script>
    import UPlot from "uplot"  // npm install uplot
    const g = (node,arg) => new UPlot(arg.opts, arg.data, node);
</script>

<div use:g={{
    data : [
        [1,2,3, 4, 5],
        [1,4,9,16,25]
    ],
    opts:{
        title: 'My Title',
        width: 1000,
        height: 600,
        series: [{label: "x"}, {label: "sin(x)", stroke: "red"}],
        scales:{x:{time:false}},
    },
}}/>
