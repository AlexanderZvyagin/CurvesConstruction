<script context="module">
    // import {curve_instruments,quotes} from './store.js'
    let fra_start=0, fra_length=0;

    export function fra (start,length) {
        return {type:'FRA',start:fra_start,length:fra_length,use:true};
    }

    export function fra_compare (a,b) {
        if(a.type!=='FRA' || b.type!=='FRA' ) return 1;
        if(a.start<b.start) return -1;
        if(a.start>b.start) return  1;
        if(a.length<b.length) return -1;
        if(a.length>b.length) return  1;
        return 0;
    }

    export function fra_set_quote (fra,quotes) {
        const q = quotes.find(v=>v.type==='FRA'&&v.start===fra.start&&v.length===fra.length);
        fra.quote = q ? q.quote : undefined;
        return fra;
    }

    export function fra_add (list,fra_start,fra_length) {
        if(fra_start<0 || fra_length<0 || (fra_start+fra_length)<=0 ){
            warn(`bad start=${fra_start} and/or length=${fra_length} arguments`);
            return;
        }
        const
            a = fra(fra_start,fra_length),
            list.find(b=>fra_compare(a,b)==0);
        if(b)
            return list;
        fra_set_quote(a);
        list.push(a);
        list.sort(fra_compare);
        // curve_instruments.update(current=>{
        //     let v = [...current,a];
        //     v.sort(fra_compare);
        //     return v;
        // });
        return list;
    }

    export function fra_del (a) {
        let index = $curve_instruments.findIndex(b=>fra_compare(a,b)==0);
        if(index>=0)
            curve_instruments.update(v=>{
                return [...v.slice(0,index),...v.slice(index+1,v.length)];
            });
        else{
            warn('fra_del: not found:',v);
        }
    }

    let swap_length=1, swap_leg1=1, swap_leg2=1;

    export function swap (length,leg1,leg2) {
        return {type:'Swap',length:swap_length,leg1:swap_leg1,leg2:swap_leg2,use:true};
    }

    export function swap_compare (a,b) {
        if(a.type!=='Swap' || b.type!=='Swap' ) return 1;
        if(a.length<b.length) return -1;
        if(a.length>b.length) return  1;
        if(a.leg1<b.leg1) return -1;
        if(a.leg1>b.leg1) return  1;
        if(a.leg2<b.leg2) return -1;
        if(a.leg2>b.leg2) return  1;
        return 0;
    }

    export function swap_set_quote (swap,quotes) {
        const q = quotes.find(v=>v.type==='Swap'&&v.length===swap.length&&v.leg1===swap.leg1&&v.leg2===swap.leg2);
        swap.quote = q ? q.quote : undefined;
        return swap;
    }

    export function swap_add (list,swap_length,swap_leg1,swap_leg2) {
        if(swap_length<=0 || swap_leg1<=0 || swap_leg2<=0 ){
            warn(`bad swap ignore`);
            return;
        }
        const
            a = swap(swap_length,swap_leg1,swap_leg2),
            b = list.find(b=>swap_compare(a,b)==0);
        if(b)
            return list;
        swap_set_quote(a);
        // curve_instruments.update(current=>{
        //     let v = [...current,a];
        //     v.sort(swap_compare);
        //     return v;
        // });
        list.push(a);
        list.sort(swap_compare);
        return list;
    }

    export function swap_del (list,a) {
        let index = .findIndex(b=>swap_compare(a,b)==0);
        if(index>=0)
            curve_instruments.update(v=>{
                return [...v.slice(0,index),...v.slice(index+1,v.length)];
            });
        else{
            warn('swap_del: not found:',v);
        }
    }
</script>
