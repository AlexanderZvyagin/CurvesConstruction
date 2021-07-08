<script>
  export let row, col, checked, onCheck;
  //
  // // export let onContactButtonClick;
  // export let col;
  // export let row;
  // let key = col.key.charAt(0).toUpperCase() + col.key.slice(1)
// on:click={() => onContactButtonClick(row)}>{key} {row.first_name} {row.last_name} </button>
</script>

<input type="checkbox" {checked} on:click/>
