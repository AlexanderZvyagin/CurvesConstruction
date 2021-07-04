#pragma once

#include "Instrument.hpp"

class ZeroCouponBond : public Instrument {
public:
    float GetMaturity (void) const override {return maturity;}
    float maturity;
    float value;
    ZeroCouponBond (float t,float value = NAN)
        : maturity(t), value(value) {}
    ZeroCouponBond * Clone (void) const override {
        return new ZeroCouponBond(*this);
    }
    std::string About (void) const override {
        char buf[128];
        snprintf(
            buf, sizeof(buf),
            "ZeroCouponBond t=%g value=%g",
            maturity, value
        );
        return std::string(buf);
    }
    float Eval (const YieldCurve &curve) const override;
    float Value (void) const override {return value;}
    virtual void AddToCurve (YieldCurve &curve) const;
};
