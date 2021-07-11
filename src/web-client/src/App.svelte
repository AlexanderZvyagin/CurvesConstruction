<script>
	import TimeGrid from './TimeGrid.svelte';
	import ShowCurves from './ShowCurves.svelte';
	import CurveBuildInput from './CurveBuildInput.svelte';
	import {curve_instruments, quotes, curves} from './store.js';
	import {instrument_info} from './instruments.js';

	let
		url               = 'ws://192.168.1.82:8100',
		protocol          = 'curves',
		ws_ready          = false,
		ws                = new WebSocket(url,protocol),
		date,
		request_id        = 0,
		quotes_request_id = 0,
		plot              = {};

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
		// console.log(`onmessage size=${msg.data.length}  ${data.type}`);

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
				}else{
					console.warn(`get_quotes_OK: id mismatch ${quotes_request_id} ${data.payload.request_id}`)
				}
				break;
			}
			case 'build_curve_OK':{
				curves.update(current=>{
                                    const item = current.find(v=>v.request.request_id===data.payload.request_id);
                                    if(!item) throw `Build curves: not found request id=${data.payload.request_id}`;
                                    console.log('curves update');
                                    item.reply = data.payload;
                                    return current;
				});
				break;
			}
			default:
				console.error('ws.onmessage: bad response:',data);
		}
	}

	const send = data => {
		if(ws.readyState === WebSocket.OPEN){
			request_id += 1;
			ws.send(JSON.stringify(data));
		}else{
			throw Error(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	};

	function get_quotes (e) {
		date = e.detail;
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
			console.warn(`WebSocket is not ready:  readyState=${ws.readyState}`);
		}
	}

</script>

<header>
	<h1>Curves Construction</h1>
</header>

<main>
	<!-- <button on:click={handle_echo}>echo</button> -->
	<div class='CurveBuildInput'>
		<CurveBuildInput on:buildCurve={build_curve} on:newDate={get_quotes}/>
	</div>
	<div class='ShowCurves'>
		<ShowCurves />
	</div>
	<!-- <div class='CurvesBuildInfo'>
		<CurvesBuildInfo />
	</div> -->
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
