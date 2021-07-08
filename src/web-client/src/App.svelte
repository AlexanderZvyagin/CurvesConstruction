<script>
	import Quotes from './Quotes.svelte';
	import TimeGrid from './TimeGrid.svelte';
	import ShowCurve from './ShowCurve.svelte';
	import {curve_instruments, quotes} from './store.js';

	let
		url               = 'ws://localhost:8100',
		protocol          = 'curves',
		ws_ready          = false,
		ws                = new WebSocket(url,protocol),
		date              = "2018-02-02",
		request_id        = 0,
		quotes_request_id = 0,
		build_curve_request_id = 0,
		plot              = {};
		// quotes_list       = [];

	ws.onopen = () => {
		ws_ready = true;
		get_quotes(date);
	}
	ws.onclose = () => {
		ws_ready = false;
	}
	ws.onerror = () => {
		console.error('onerror');
	}
	ws.onmessage = (msg) => {
		const data = JSON.parse(msg.data);
		console.log(`onmessage size=${msg.data.size}  ${data}`);

		switch(data.type){
			case 'echo_OK':{
				console.log('echo received');
				break;
			}
			case 'get_quotes_OK':{
				if(quotes_request_id===data.payload.request_id){
					quotes.update(current=>data.payload.quotes || []);
				}else{
					console.warn(`get_quotes_OK: id mismatch ${quotes_request_id} ${data.payload.request_id}`)
				}
				break;
			}
			case 'build_curve_OK':{
				if(build_curve_request_id===data.payload.request_id){
					plot = {
						x:data.payload.plot.x,
						y:data.payload.plot.y
					};
				}else{
					console.warn(`build_curve_OK: id mismatch ${build_curve_request_id} ${data.payload.request_id}`)
				}
				break;
			}
			default:
				console.error('ws.onmessage: bad response:',data);
		}
	}

	const send = data => {
		// console.log('send_request');
		if(ws.readyState === WebSocket.OPEN){
			request_id += 1;
			ws.send(JSON.stringify(data));
		}else{
			throw Error(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	};

	const get_quotes = (date) => {
		// quotes_list = [];
		quotes.update(current=>[]);

		if(ws.readyState === WebSocket.OPEN){
			request_id += 1;
			let data = {
				type:'get_quotes',
				payload:{
					request_id,
					date,
				}
			};
			ws.send(JSON.stringify(data));
			quotes_request_id = request_id;
		}else{
			console.warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	};

	const handle_echo = () => {
		send({
			type:'echo',
			payload:{
				request_id,
			}
		});
	};

	function build_curve () {
		if(ws.readyState === WebSocket.OPEN){

			let instruments = [];

			for(let i in $curve_instruments){
				const v = $curve_instruments[i];
				if(!v.use) continue;
				const q = $quotes.find(x=>x[0]===v.name);
				console.log(`${v.name} quote: ${q}`);
				if(q)
					instruments.push({...v,quote:q[1]});
			}

			console.log(instruments);

			// const instruments_to_use = $curve_instruments.filter(v=>v.use);
			// 	if(!v.use) return false;
			// 	const v2 = $quotes.find(v=>v.name===);
			// });

			request_id += 1;
			let data = {
				type:'build_curve',
				payload:{
					request_id,
					date,
					points:1000,
					instruments
				}
			};
			ws.send(JSON.stringify(data));
			build_curve_request_id = request_id;
		}else{
			console.warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	}

	$:{
		get_quotes(date);
	}

</script>

<main>
	<!-- <button on:click={handle_echo}>echo</button> -->
	<input type="date" name="date" min="2011-01-01" max="2019-08-01" bind:value={date}>
	<button on:click={build_curve}>Build Curve</button>
	<ShowCurve data={plot}/>
	<Quotes data={$quotes}/>
</main>

<style>
</style>
