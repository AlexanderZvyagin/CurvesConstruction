#pragma once

#include "Instrument.hpp"

class ForwardRateAgreement : public Instrument {
public:
    float GetMaturity (void) const override {
        return start+length;
    }

    float start, length;
    float rate;

    ForwardRateAgreement (
        float start,
        float length,
        float rate = NAN
    )
        : start(start)
        , length(length)
        , rate(rate)
    {}
    ForwardRateAgreement * Clone (void) const override {
        return new ForwardRateAgreement(*this);
    }
    std::string About (void) const override {
        char buf[256];
        snprintf(
            buf, sizeof(buf),
            "ForwardRateAgreement start=%g length=%g rate=%g",
            start,
            length,
            rate
        );
        return std::string(buf);
    }
    float Eval (const Curve &curve) const override;
    float Value (void) const override {return rate;}
    virtual void AddToCurve (Curve &curve) const;
};
