<script>
	import Quotes from './Quotes.svelte';
	import TimeGrid from './TimeGrid.svelte';
	import ShowCurve from './ShowCurve.svelte';
	import ShowCurves from './ShowCurve.svelte';
	import CurveBuildInput from './CurveBuildInput.svelte';
	import {curve_instruments, quotes, curves} from './store.js';
	import {instrument_info} from './instruments.js';

	let
		url               = 'ws://localhost:8100',
		protocol          = 'curves',
		ws_ready          = false,
		ws                = new WebSocket(url,protocol),
		date,
		request_id        = 0,
		quotes_request_id = 0,
		build_curve_request_id = 0,
		plot              = {};
		// quotes_list       = [];

	ws.onopen = () => {
		ws_ready = true;
		if(date)
			get_quotes({detail:date});
	}
	ws.onclose = () => {
		ws_ready = false;
	}
	ws.onerror = () => {
		console.error('onerror');
	}
	ws.onmessage = (msg) => {
		const data = JSON.parse(msg.data);
		console.log(`onmessage size=${msg.data.length}  ${data.type}`);

		switch(data.type){
			case 'echo_OK':{
				console.log('echo received');
				break;
			}
			case 'get_quotes_OK':{
				if(quotes_request_id===data.payload.request_id){
					quotes.update(current=>{
						const quotes = data.payload.quotes || [];
						return quotes.map(v=>{
							let instrument = instrument_info(v[0]);
							instrument.name  = v[0];
							instrument.quote = v[1];
							return instrument;
						})
					});
					// console.log('DONE');
					// console.log($quotes);
				}else{
					console.warn(`get_quotes_OK: id mismatch ${quotes_request_id} ${data.payload.request_id}`)
				}
				break;
			}
			case 'build_curve_OK':{
				curves.update(current=>{
					console.log(`curves update!  current.length=${current.length}`);
					return [...current,data.payload];
				});
				// if(build_curve_request_id===data.payload.request_id){
				// 	plot = {
				// 		x:data.payload.plot.x,
				// 		y:data.payload.plot.y
				// 	};
				// }else{
				// 	console.warn(`build_curve_OK: id mismatch ${build_curve_request_id} ${data.payload.request_id}`)
				// }
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

	function get_quotes (e) {
		// console.log(`get_quotes: ready=${ws.readyState === WebSocket.OPEN} ${e.detail}`);
		date = e.detail;
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
			// console.log('get_quotes, sending:',data);
			ws.send(JSON.stringify(data));
			quotes_request_id = request_id;
		}else{
			console.warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	}

	const handle_echo = () => {
		send({
			type:'echo',
			payload:{
				request_id,
			}
		});
	};

	function build_curve (e) {
		// console.log('yyy',e.detail);
		if(ws.readyState === WebSocket.OPEN){

			// let instruments = [];

			// for(let i in $curve_instruments){
			// 	const v = $curve_instruments[i];
			// 	if(!v.use) continue;
			// 	const q = $quotes.find(x=>x[0]===v.name);
			// 	// console.log(`${v.name} quote: ${q}`);
			// 	if(q)
			// 		instruments.push({...v,quote:q[1]});
			// }

			// console.log(instruments);

			request_id += 1;
			let data = {
				type:'build_curve',
				payload:{
					...e.detail,
					request_id,
					// date,
					points:1000,
					// instruments
				}
			};
			ws.send(JSON.stringify(data));
			build_curve_request_id = request_id;
		}else{
			console.warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	}

	// $:{
	// 	get_quotes(date);
	// }

</script>

<header>
	<h1>Curves Construction</h1>
</header>

<!-- <nav>
	<ul>
		<li>Tab A</li>
		<li>Tab B</li>
	</ul>
</nav> -->

<main>
	<!-- <button on:click={handle_echo}>echo</button> -->
	<div class='CurveBuildInput'>
		<CurveBuildInput on:buildCurve={build_curve} on:newDate={get_quotes}/>
	</div>
	<!-- <input type="date" name="date" min="2011-01-01" max="2019-08-01" bind:value={date}> -->
	<!-- <button on:click={build_curve}>Build Curve</button> -->
	<!-- <div class='ShowCurves'>
		<ShowCurves />
	</div> -->
	<!-- <Quotes data={$quotes}/> -->
</main>

<style>
.CurveBuildInput{
	border: solid 1px;
}
header h1{
	/* position: relative; */
	/* left: 100px; */
	/* border: solid red 1px; */
	display: inline-block;
	/* position
	text-align: center; */
	/* background-color: #f3f3f3; */
	text-align: center;
	/* position: fixed; */
	width: 100%;
}
nav{
	background: green;
	padding: 20px;
}
nav ul{
	white-space: nowrap;
}
/* nav li{
	color: yellow;
	display: inline-block;
	width: 50%;
	text-align: center;
} */
</style>
