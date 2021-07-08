#pragma once

#include <cmath>
#include <iostream>

class YieldCurve;

class Instrument {
public:
    virtual float GetMaturity (void) const {return NAN;}
    virtual ~Instrument (void) {}
    Instrument (void) {}
    virtual Instrument * Clone (void) const = 0;
    virtual std::string About (void) const {return "";}
    virtual float Eval (const YieldCurve &curve) const {return NAN;}
    virtual float Value (void) const {return NAN;}
    virtual void AddToCurve (YieldCurve &curve) const = 0;
};

inline
std::ostream & operator << (std::ostream &os,const Instrument &x) {
    os << x.About() << "\n";
    return os;
}
