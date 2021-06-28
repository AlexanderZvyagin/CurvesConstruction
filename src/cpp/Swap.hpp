#pragma once

#include "Instrument.hpp"

class LegFixed : public Instrument {
public:
    float t0;
    float dt;
    int n; // number of periods
    float rate;

    LegFixed (void) {}
    LegFixed (float t0,float dt,int n,float rate)
        : t0 (t0)
        , dt (dt)
        , n (n)
        , rate (rate)
    {}
    LegFixed * Clone (void) const override {
        return new LegFixed(*this);
    }
    std::string About (void) const override {
        char buf[256];
        snprintf(
            buf, sizeof(buf),
            "LegFixed t0=%g dt=%g n=%d",
            t0,dt,n
        );
        return buf;
    }

    float GetMaturity (void) const override {return dt*n;}
    float Eval (const YieldCurve &curve) const override;
    float Value (void) const override {return rate;}
    // void AddToCurve (YieldCurve &curve) const override {}
};

class LegFloat : public Instrument {
public:
    float t0;
    float dt;
    int n; // number of periods
    // std::shared_ptr<YieldCurve> curve;
    YieldCurve* curve {nullptr};

    LegFloat (void) {}
    LegFloat (float t0,float dt,int n,YieldCurve &curve)
        : t0 (t0)
        , dt (dt)
        , n (n)
        , curve (&curve)
    {}
    LegFloat * Clone (void) const override {
        return new LegFloat(*this);
    }
    std::string About (void) const override {
        char buf[256];
        snprintf(
            buf, sizeof(buf),
            "LegFloat t0=%g dt=%g n=%d",
            t0,dt,n
        );
        return buf;
    }

    float GetMaturity (void) const override {return dt*n;}
    float Eval (const YieldCurve &curve) const override;
    // void AddToCurve (YieldCurve &curve) const override {}
};

class Swap : public Instrument {
public:
    LegFixed lfix;
    LegFloat lflt;

    Swap (void) {}

    float GetMaturity (void) const override
        {return std::max(lfix.GetMaturity(),lflt.GetMaturity());}

    float Eval (const YieldCurve &curve) const override
        {return lfix.Eval(curve) - lflt.Eval(curve);}

    Swap * Clone (void) const override
        {return new Swap(*this);}

    std::string About (void) const override {
        char buf[256];
        snprintf(
            buf,sizeof(buf),
            "Swap: (%s) (%s)",
            lfix.About().c_str(),
            lflt.About().c_str()
        );
        return buf;
    }

    float value {0};

    float Value (void) const override
        {return value;}

    // void AddToCurve (YieldCurve &curve) const override;
};
