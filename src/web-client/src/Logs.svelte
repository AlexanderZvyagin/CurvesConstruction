<script>
</script>

<script context="module">

    import {logs} from './store.js';

    function add (level,...args) {
        logs.update(cur=>{
            cur.push({
                time:new Date(),
                level,
                msg: args.join(' ')
            });
            return cur;
        });
    }

    export function error (...msg) {
        console.error(...msg);
        add('error',...msg);
    }
    export function debug (...msg) {
        console.log(...msg);
        add('debug',...msg);
    }
    export function info (...msg) {
        console.log(...msg);
        add('info',...msg);
    }
    export function warn (...msg) {
        console.warn(...msg);
        add('warn',...msg);
    }
    export const warning = warn;
</script>


<main>
    <div class="container">
    {#each $logs as log}
        <div>{`${log.time.getHours()}:${log.time.getMinutes()}:${log.time.getSeconds()}`}</div>
        <div>{log.level}</div>
        <div>{log.msg}</div>
    {/each}
    </div>
</main>

<style>
.container{
    display: grid;
    grid-template-columns: 1fr 1fr 5fr;
    grid-gap: 10px;
    margin: 0 auto;
    max-height: 500px;
    overflow: scroll;
}
</style>
