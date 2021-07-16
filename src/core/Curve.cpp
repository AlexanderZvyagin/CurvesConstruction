#include "Curve.hpp"
#include "Instrument.hpp"
#include "minimizer.hpp"

Curve & Curve::Add (const Instrument &x) {
    instruments[x.GetMaturity()].reset(x.Clone());
    return *this;
}

math::Result
Curve::BuildPiecewiseConstant (void) {
    *this = math::Interpolator1D();
    // for(auto [t,instr]: GetInstruments())
    //     instr->AddToCurve(*this);
    for(std::map <float,std::shared_ptr<Instrument>>::const_iterator it=instruments.cbegin(); it!=instruments.cend(); it++)
        it->second->AddToCurve(*this);
    return math::Result();
}

math::Result
Curve::Build (
    math::Interpolator1D::Type itype
){
    math::Options opts;
    opts.eps_abs = 1e-5;
    opts.eps_rel = 1e-5;
    opts.iters = 1000 + instruments.size()*1000;
    return Build(itype,opts);
}

math::Result
Curve::Build (
    math::Interpolator1D::Type itype,
    const math::Options &opts
) {
    if(itype==math::Interpolator1D::Type::PiecewiseConstant)
        return BuildPiecewiseConstant();

    // grid on time
    std::vector<double> vx {0}; // always start with t=0 point
    // for(auto [t,instr]: GetInstruments()) {
    for(std::map <float,std::shared_ptr<Instrument>>::const_iterator it=instruments.cbegin(); it!=instruments.cend(); it++){
        const float &t = it->first;
        if(t>0)
            vx.push_back(t);
        else
            throw std::invalid_argument("Curve::Build: instrument maturity must be >1");
    }

    std::vector<math::Parameter> pars (vx.size(),math::Parameter(0,1e-2));

    auto func = [&] (const std::vector<double> &_vy) -> double {
        std::vector<double> vy(_vy.begin(),_vy.end());
        *this = Interpolator1D (vx,vy,itype);
        double result {0};
        for(std::map <float,std::shared_ptr<Instrument>>::const_iterator it=instruments.cbegin(); it!=instruments.cend(); it++)
            result += std::pow( it->second->Value()-it->second->Eval(*this), 2 );
        return result;
    };
    math::Result result = math::minimize( func, pars, opts );

    std::vector<double> vy;
    // for( auto y: r.x)
    for(std::vector<math::Parameter>::const_iterator it=result.x.cbegin();
        it!=result.x.cend(); it++)
        vy.push_back(it->value);

    if(vx.size()!=vy.size())
        throw std::logic_error("Curve::Build: internal error");

    *this = math::Interpolator1D (vx,vy,itype);

    return result;
}

void Curve::Print(void) const {
    printf("Curve interpolation type: %s size=%d\n",Name().c_str(),GetSize());


    std::cout << static_cast<math::Interpolator1D>(*this) << "\n";

    // for(auto [t,instr]: GetInstruments()) {
    for(std::map <float,std::shared_ptr<Instrument>>::const_iterator it=instruments.cbegin();
        it!=instruments.cend(); it++
    ){
        const Instrument *instr = it->second.get();
        if(!instr) continue;
        printf("%s\n",instr->About().c_str());
        printf("    market quote ... %g\n",instr->Value());
        printf("    caluclated ..... %g\n",instr->Eval(*this));
        printf("    difference ..... %g\n",instr->Value()-instr->Eval(*this));
        printf("\n");
    }
}
