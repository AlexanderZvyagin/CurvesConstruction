// https://www.openfigi.com

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

const code = c => c.charCodeAt(0);

// returns number a new pos OR undefined if there is nothing to extract
function decode_months_part (s,i) {
    let l=0;

    // skip the leading 0, but only one time
    if( i<s.length && s[i]==='0')
        i++;

    while(true){
        const n = parseInt(s[i]);
        if(isNaN(n)) break;
        l = l*10 + 12 * n;
        i++;
    }
    // if(i<s.length){
    //     const n = parseInt(s[i]);
    //     if(n){
    //         l += 12 * n;
    //         i++;
    //     }
    // }

    if( i<s.length ){
        const c = code(s[i]);
        if( code(s[i])>=code('A') && code(s[i])<=code('K') ) {
            l += c - code('A') + 1;
            i++;
        }
    }
    return [l,i];
}

function decode_months(s) {
    const [n,i] = decode_months_part(s,0);
    if(i!==s.length) throw `decode_months() faild on ${s}`;
    return n;
}

// return two numbers in months
function decode_FRA_periods (name) {
    // 'EUFR0J1J' # FRA 10M x 22M '''
    // if( name=="EUR006M" )
    //     return [0,6];

    const fra0_prefix = 'EUR0';

    if( name.startsWith(fra0_prefix) ){
        const unit = name[name.length-1]; // 'M', 'W'
        const name2 = name.substring(fra0_prefix.length,name.length-1);
        const n = parseInt(name2);
        if(unit==='M')
            return [0,n];
        else
            return;
    }

    const fra_prefix = 'EUFR';
    if( name.startsWith(fra_prefix) ){

        const s = name.substring(fra_prefix.length);

        try {
            const [n1,i1] = decode_months_part(name,fra_prefix.length);
            const [n2,i2] = decode_months_part(name,i1);
            if(i2!==name.length)
                throw `check it: ${name}  (${n1} ${i1})   (${n2} ${i2})`;
            return [n1,n2];
        } catch (e) {
            error(e);
        }
    }
}

// For a period in months given as an integer number
// returns a string:  format_months(27) -> '2Y3M'
function format_months (months) {
    let
        p = Number(months),
        y = Math.floor(p/12),
        m = p % 12,
        r = '';
    if(y) r  = `${y}Y`;
    if(m) r += `${m}M`;
    return r || '0M';
}

function decode (name) {
    try {
        let
            r,
            last_char = name[name.length-1],
            last_char_TZ = 'TZ'.indexOf(last_char)!==-1;

        r = RegExp('^EUSP.*$').exec(name);
        if(r){
            return {
                name,
                type:'Swaption Volatility',
                descr:'Swaption Volatility',
            }
        }

        r = RegExp('^EUVE.*$').exec(name);
        if(r){
            return {
                name,
                type:'Swaption Volatility',
                descr:'Swaption Volatility',
            }
        }

        r = RegExp('^EUSWIM.*$').exec(name);
        if(r){
            return {
                name,
                type:'IMM Swap',
                descr:'IMM Swap',
            }
        }

        r = RegExp('^EUDR(.*)$').exec(name);
        if(r && name.length<7 && !last_char_TZ){
            if(r){
                const
                    length = decode_months(r[1]);
                return {
                    name,
                    type:'Deposit',
                    length,
                    descr:`EUR Deposit ${format_months(length)}`,
                }
            }
        }


        r = RegExp('^EUBSV(\\d+)$').exec(name);
        if(r){
            const
                leg1   = 3,
                leg2   = 6,
                length = Number(r[1]);
            return {
                name,
                type:'Basis Swap',
                leg1,
                leg2,
                length,
                descr:`EUR Basis Swap ${format_months(length)} (${format_months(leg1)} vs ${format_months(leg2)})`,
            }
        }

        r = RegExp('^EUSA(\\d{1,2})$').exec(name);
        if(r){
            const
                leg1   = 12,
                leg2   = 6,
                length = decode_months(r[1]);
            return {
                name,
                type:'Swap',
                leg1,
                leg2,
                length,
                descr:`EUR Swap ${format_months(length)} (${format_months(leg1)} vs ${format_months(leg2)})`,
            }
        }

        r = RegExp('^EUSW(\\d*[A-Z]*)V(\\d*[A-Z]*)$').exec(name);
        if(r){
            const
                leg1   = 12,
                leg2   = decode_months(r[2]),
                length = decode_months(r[1]);
            return {
                name,
                type:'Swap',
                leg1,
                leg2,
                length,
                descr:`EUR Swap ${format_months(length)} (${format_months(leg1)} vs ${format_months(leg2)})`,
            }
        }

        // r = RegExp('^EUSWE(\\d+)([A-Z])?$').exec(name);
        r = RegExp('^EUSWE(\\d*[A-Z]*)$').exec(name);
        if(r){
            if(last_char_TZ) return;
            let length = decode_months(r[1]);
            // const c = r[2];
            // if(c){
            //     if(last_char_TZ) return;
            //     months += c.charCodeAt(0)-'A'.charCodeAt(0)+1;
            // }
            return {
                name,
                type:'Swap',
                leg1:'EONIA',
                length,
                descr:`EUR Swap (EONIA) ${format_months(length)}`
            }
        }
    } catch (error) {
        warn(`decode: ${error}`);
    }
}

function instrument_info(name) {

    let decoded = decode(name);
    if(decoded) return decoded;

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
                    descr:`FRA ${format_months(r[0])}x${format_months(r[1])}`
                };
            }
        case 'SWAP':
        // info = swap_info(name)||'';
            return {name,type:'SWAP','descr':'SWAP'};
        default:
            return {name,type,descr:''};
    }
}

export {instrument_info};
