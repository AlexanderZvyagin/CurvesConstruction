<script>
	export let data;
	import {instrument_info} from './instruments.js';
</script>

<div>
	<div class="type">{instrument_info(data[0]).descr}</div>
    <div class="name">{data[0]}</div>
	<div class="value">{data[1]}</div>
</div>

<style>
.type{
	color: green;
	width: 150px;
	display: inline-block;
}
.name{
	color: blue;
	width: 80px;
	display: inline-block;
}
.value{
	color: green;
	width: 80px;
	display: inline-block;
}
</style>
