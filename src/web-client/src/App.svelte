<script>
    import { onMount } from 'svelte';
    import TimeGrid from './TimeGrid.svelte';
    import ShowCurves from './ShowCurves.svelte';
    import CurveBuildInput from './CurveBuildInput.svelte';
    import {quotes, date} from './store.js';
    import {instrument_info} from './instruments.js';
    import CurvesBuildInfo from './CurvesBuildInfo.svelte'
    import Tabs from './components/Tabs.svelte'
    import QuotesTable from './QuotesTable.svelte'
    import Logs, {debug,info,warn,error} from './Logs.svelte'

    let
        nav_items = ['Curves Construction','Quotes','Logs'],
        nav_active = nav_items[0];
    let
        get_quotes_trigger = false,
        ws,
        request_id        = 0,
        quotes_request_id = 0;

    function connect() {
        let
            // url               = 'ws://192.168.1.82:8100',
            url               = 'ws://naz.hopto.org:8100',
            protocol          = 'curves';

        ws = new WebSocket(url,protocol);

        ws.onopen = () => {
            debug('WebSocket is opened!');
            get_quotes_trigger = !!$date;
        }

        ws.onclose = function(e) {
            warn('Socket is closed. Reconnect will be attempted in 1 second.', e.reason);
            setTimeout(function() {
                connect();
            }, 1000);
        };

        ws.onerror = function(err) {
            error('Socket encountered error: ', err.message, 'Closing socket');
            ws.close();
        };

        ws.onmessage = (msg) => {
        	const data = JSON.parse(msg.data);

        	switch(data.type){
        		case 'echo_OK':{
        			debug('echo received');
        			break;
        		}
        		case 'get_quotes_OK':{
        			if(quotes_request_id===data.payload.request_id){
        				quotes.update(current=>{
        					const quotes = data.payload.quotes || [];
        					return quotes.map(v=>{
        						let instrument = instrument_info(v[0]);
        						// instrument.name  = v[0];
        						instrument.quote = v[1];
        						return instrument;
        					})
        				});
        			}else{
        				warn(`get_quotes_OK: id mismatch ${quotes_request_id} ${data.payload.request_id}`)
        			}
        			break;
        		}
        		case 'build_curve_OK':{
        			curves.update(current=>{
                                        const item = current.find(v=>v.request.request_id===data.payload.request_id);
                                        if(!item) throw `Build curves: not found request id=${data.payload.request_id}`;
                                        item.reply = data.payload;
                                        return current;
        			});
        			break;
        		}
        		default:
        			error('ws.onmessage: bad response:',data);
        	}
        }
    }

    connect();

    const send = data => {
        if(ws.readyState === WebSocket.OPEN){
            request_id += 1;
            debug('sending:',data);
            ws.send(JSON.stringify(data));
        }else{
            throw Error(`WebSocket is not ready:  readyState=${ws.readyState}`);
        }
    };

    function get_quotes (e) {
        let the_date = e.detail;
        // debug(`get_quotes(${Object.keys(e)})`);
        debug(`get_quotes(${the_date})`);
        quotes.update(current=>[]);

        if(ws.readyState === WebSocket.OPEN){
            request_id += 1;
            let data = {
                type:'get_quotes',
                payload:{
                    request_id,
                    date:the_date,
                }
            };
            ws.send(JSON.stringify(data));
            quotes_request_id = request_id;
        }else{
            warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
        }
    }

    function build_curve (e) {
        if(ws.readyState === WebSocket.OPEN){
            request_id += 1;
            let data = {
                type:'build_curve',
                payload:{
                    ...e.detail,
                    request_id,
                    points:1000,
                }
            };
            curves.update(cur=>[...cur,{request:data.payload}]);
            ws.send(JSON.stringify(data));
        }else{
            warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
        }
    }

    $:{
        debug('seeting the date to',$date);
        if(get_quotes_trigger)
            get_quotes({detail:$date});
    }
</script>

<main>
    <div class='tabs'>
        <Tabs items={nav_items} active={nav_active} on:tabChanged={e=>{nav_active=e.detail;}}/>
    </div>

    {#if nav_active==='Curves Construction' }
        <!-- <button on:click={handle_echo}>echo</button> -->
        <div class='CurveBuildInput'>
            <CurveBuildInput on:buildCurve={build_curve} on:newDate={get_quotes}/>
        </div>
        <div class='ShowCurves'>
            <ShowCurves />
        </div>
        <div class='CurvesBuildInfo'>
            <CurvesBuildInfo />
        </div>
    {:else if nav_active==='Quotes'}
        <QuotesTable data={$quotes}/>
    {:else if nav_active==='Logs'}
        <Logs/>
    {/if}

</main>

<footer>
    Build: 3
</footer>

<style>
.CurveBuildInput{
    border: solid 1px;
}
footer{
    color: #aaa;
    border-top: 1px solid #ddd;
    display: inline-block;
    padding: 20px;
    font-size: 0.7em;
}
</style>
