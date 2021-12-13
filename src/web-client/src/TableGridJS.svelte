<script>
    import { Grid } from "gridjs"

    function g(node, data) {
        let v = new Grid({
            columns: ["Name", "Email"],
            data,
            search: {
                enabled: true,
                placeholder: 'Search...'
            }
        })

        v.render(node);
    }

    let
        name = "world",
        data = [
            ["John", "john@example.com"],
            ["Mark", "mark@gmail.com"],
            ["Eoin", "eoin@gmail.com"],
            ["Sarah", "sarahcdd@gmail.com"],
            ["Afshin", "afshin@mail.com"],
        ];

</script>


<svelte:head>
    <link
        href="https://unpkg.com/gridjs/dist/theme/mermaid.min.css"
        rel="stylesheet"
    />
</svelte:head>

<section>
<h1>Table with GridJS</h1>

<div use:g={data} />

</section>
