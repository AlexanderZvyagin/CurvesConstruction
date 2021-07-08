<script>
	import SvelteTable from "svelte-table";
	export let data;

	// // define column configs
	// const columns = [
	//   {
	//     key: "id",
	//     title: "ID",
	//     value: v => v.id,
	//     sortable: true,
	//     filterOptions: rows => {
	//       // generate groupings of 0-10, 10-20 etc...
	//       let nums = {};
	//       rows.forEach(row => {
	//         let num = Math.floor(row.id / 10);
	//         if (nums[num] === undefined)
	//           nums[num] = { name: `${num * 10} to ${(num + 1) * 10}`, value: num };
	//       });
	//       // fix order
	//       nums = Object.entries(nums)
	//         .sort()
	//         .reduce((o, [k, v]) => ((o[k] = v), o), {});
	//       return Object.values(nums);
	//     },
	//     filterValue: v => Math.floor(v.id / 10),
	//     headerClass: "text-left"
	//   },

	const columns = [{
		key: 'id',
		title: 'ID',
		value: v => v.id,
		sortable: true
	},{
		key: 'start',
		title: 'Start',
		value: v => v.start,
		sortable: true
	},{
		key: 'length',
		title: 'Length',
		value: v => v.length,
		sortable: true
	}];

	let data2 = [];
	$: {
		// data2 = data.map( (item,index)=>{{...item,id:index}} );
		data2 = data;
	}

	// bind:filterSelections={selection}

</script>

<div>
	<SvelteTable
  		columns={columns}
  		rows={data2}
	/>
	<div>
		<!-- <input class='use' type='checkbox' bind:value={data.use}/> -->
	    <div class="start">{data.start}</div>
		<div class="length">{data.length}</div>
		<div class="maturity">{data.start+data.length}</div>
	</div>
</div>

<style>
.start{
	color: green;
	width: 150px;
	display: inline-block;
}
.length{
	color: blue;
	width: 80px;
	display: inline-block;
}
.maturity{
	color: green;
	width: 80px;
	display: inline-block;
}
</style>
