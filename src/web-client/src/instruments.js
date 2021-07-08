const instrument_type = name => {
    let type;
    if(name.startsWith('EUFR') || name.startsWith('EUR0'))
        type = 'FRA';
    else if(name.startsWith('EUS') || name.startsWith('EUDR1T'))
        type = 'SWAP';
    return type;
};

function swap_info (name) {
    // FIXME: no code for the moment
    return;
}

// const instrument_maturity = {
// 	EUDR1T:'1D',
//     EUSWE1Z:'1W',
//     EUSWE2Z:'2W',
//     EUSWE3Z:'3W',
//     EUSWEA:'1M',
//     EUSWEB:'2M',
//     EUSWEC:'3M',
//     EUSWED:'4M',
//     EUSWEE:'5M',
//     EUSWEF:'6M',
//     EUSWEG:'7M',
//     EUSWEH:'8M',
//     EUSWEI:'9M',
//     EUSWEJ:'10M',
//     EUSWEK:'13M',
//     EUSWE1:'1Y',
//     EUSWE1A:'13M',
//     EUSWE1B:'14M',
//     EUSWE1C:'15M',
//     EUSWE1D:'16M',
//     EUSWE1E:'17M',
//     EUSWE1F:'18M',
//     EUSWE1G:'19M',
//     EUSWE1H:'20M',
//     EUSWE1I:'23M',
//     EUSWE1J:'22M',
//     EUSWE1K:'23M',
//     EUSWE2:'2Y',
//     EUSWE3:'3Y',
//     EUSWE4:'4Y',
//     EUSWE5:'5Y',
//     EUSWE6:'6Y',
//     EUSWE7:'7Y',
//     EUSWE8:'8Y',
//     EUSWE9:'9Y',
//     EUSWE10:'10Y',
//     EUSWE11:'11Y',
//     EUSWE12:'12Y',
//     EUSWE15:'15Y',
//     EUSWE20:'20Y',
//     EUSWE25:'25Y',
//     EUSWE30:'30Y',
//     EUSWE40:'40Y',
//     EUSWE50:'50Y',
//     EUR006M:'6M',
//     EUFR00F:'6M',
//     EUFR0AG:'7M',
//     EUFR0BH:'8M',
//     EUFR0CI:'9M',
//     EUFR0DJ:'10M',
//     EUFR0EK:'11M',
//     EUFR0F1:'1Y',
//     EUFR0G1A:'13M',
//     EUFR0H1B:'14M',
//     EUFR0I1C:'15M',
//     EUFR0J1D:'16M',
//     EUFR0K1E:'17M',
//     EUFR011F:'18M',
//
//     // A:1 B:2 C:3 D:4 E:5 F:6 G:7 H:8 I:9 J:10 K:11
//     EUFR0EH:'5M',       // FRA 5M x 8M
//     EUFR0AD:'1M',
//     EUFR0BE:'2M',
//     EUFR0CF:'3M',
//     EUFR0DG:'4M',
//     EUFR0FI:'6M',
//     EUFR0GJ:'7M',
//     EUFR0HK:'8M',
//     EUFR0I1:'9M',       // FRA 9M x 1Y
//     EUFR0J1J:'10M',     // FRA 10M x 22M
//     EUFR0H1H:'8M',
//     EUFR0G1G:'7M',
//     EUFR0I1I:'9M',
//     EUFR0C1C:'3M',
//     EUFR0A1A:'1M',
//     EUFR0E1E:'5M',
//     EUFR0K1B:'11M',
//     EUFR011C:'1Y',      // FRA 1Y x 15M
//     EUFR0J1A:'10M',
//     EUFR0F1F:'6M',
//     EUFR012:'1Y',
//     EUFR0B1B:'2M',
//     EUFR0D1D:'4M',
//
//     EUSA1C:'3M',
//     EUSA1F:'6M',
//     EUSA1I:'9M',
//     EUSA1:'1Y',
//     EUSA2:'2Y',
//     EUSA3:'3Y',
//     EUSA4:'4Y',
//     EUSA5:'5Y',
//     EUSA6:'6Y',
//     EUSA7:'7Y',
//     EUSA8:'8Y',
//     EUSA9:'9Y',
//     EUSA10:'10Y',
//     EUSA11:'11Y',
//     EUSA12:'12Y',
//     EUSA13:'13Y',
//     EUSA14:'14Y',
//     EUSA15:'15Y',
//     EUSA16:'16Y',
//     EUSA17:'17Y',
//     EUSA18:'18Y',
//     EUSA19:'19Y',
//     EUSA20:'20Y',
//     EUSA21:'21Y',
//     EUSA22:'22Y',
//     EUSA23:'23Y',
//     EUSA24:'24Y',
//     EUSA25:'25Y',
//     EUSA26:'26Y',
//     EUSA27:'27Y',
//     EUSA28:'28Y',
//     EUSA29:'29Y',
//     EUSA30:'30Y',
//     EUSA35:'35Y',
//     EUSA40:'40Y',
//     EUSA50:'50Y',
//     EUSA60:'60Y',
// }

// return two numbers in months
function decode_FRA_periods (name) {
    const code = c => c.charCodeAt(0);
    // 'EUFR0J1J' # FRA 10M x 22M '''
    // if( name=="EUR006M" )
    //     return [0,6];

    const fra0_prefix = 'EUR0';

    if( name.startsWith(fra0_prefix) ){
        const unit = name[name.length-1]; // 'M', 'W'
        // if(name[name.length-1]!=='M')
        // 	throw `Bad name ${name}`;
        const name2 = name.substring(fra0_prefix.length,name.length-1);
        const n = parseInt(name2);
        // console.log(`${name} ${name2} ${n} ${unit}`);
        if(unit==='M')
            return [0,n];
        else
            return;
    }
    // console.log(name);


    const fra_prefix = 'EUFR';
    if( name.startsWith(fra_prefix) ){

        const s = name.substring(fra_prefix.length);
        // console.log(`${name} => ${s}`);

        // returns number a new pos OR undefined if there is nothing to extract
        const decode_months = (s,i) => {
            // console.log(`decoding ${s} from ${i}`)
            let l=0;

            // skip the leading 0, but only one character
            if( i<s.length && s[i]==='0')
                i++;

            if(i<s.length){
                const n = parseInt(s[i]);
                if(n){
                    // take a single number as a year
                    l += 12 * n;
                    i++;
                }
            }
            if( i<s.length ){
                const c = code(s[i]);
                if( code(s[i])>=code('A') && code(s[i])<=code('K') ) {
                    // console.log('months:',c - code('A') + 1);
                    l += c - code('A') + 1;
                    i++;
                }
            }
            return [l,i];
        }

        try {
            const [n1,i1] = decode_months(name,fra_prefix.length);
            const [n2,i2] = decode_months(name,i1);
            // console.log(`${name} => ${n1} ${n2}`)
            if(i2!==name.length)
                throw `check it: ${name}  (${n1} ${i1})   (${n2} ${i2})`;
            return [n1,n2];
        } catch (e) {
            console.error(e);
        }
    }
}

function instrument_info(name) {
    let
        type = instrument_type(name);
    switch(type){
        case 'FRA':{
            const r = decode_FRA_periods(name);
            if(r)
                // info = `${r[0]}Mx${r[1]}M`;
                return {
                    name,
                    type:'FRA',
                    'start':r[0],
                    'length':r[1],
                    descr:`FRA ${r[0]}Mx${r[1]}M`
                };
            }
        case 'SWAP':
        // info = swap_info(name)||'';
            return {name,type:'SWAP','descr':'SWAP'};
        default:
            return {type,descr:''};
    }
}

export {instrument_info};
