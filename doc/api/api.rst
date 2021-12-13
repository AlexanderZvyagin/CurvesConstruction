+++++++++++++++
API
+++++++++++++++

Instrument
++++++++++++++++

.. doxygenclass:: Instrument

The base class for all *instruments* which can be priced in the framework.
This is an abstract class (you cannot use it directly).

.. cpp:function:: Instrument(void)

    The default constructor, it does not do anything.

.. cpp:function:: ~Instrument(void)

    The destructor.

.. cpp:function:: float GetMaturity(void) const

    Get instrument maturity.

   :returns float: Time point (with the value of NAN,
                   as children should overload the virtual function)

.. cpp:function:: Instrument* Clone(void) const

    Create a copy of the instrument (clone the instrument).

    :returns Instrument pointer: Pointer to a clone.

.. cpp:function:: std::string About(void) const

    Gives a brief instrument info.
    It should be a short, single-line string (without end-of-line characters)

    :returns string: Instrument info.

.. cpp:function:: float Value(void) const

    Value of the instrument. The units are not defined. For example for
    FRAs it can be rate per year (which is close to zero),
    or rate in % per year, or rate in basis points for year.
    For ZeroCouponBond it can be just a bond value (which is close to 1).

    :returns float: Instrument value.

.. cpp:function:: float Eval(Curve) const

    Evaluate the instrument value with the given curve. Units of a computed
    value must be the same as for Value() function.
    If Eval(Curve)==Value() than the instrument is priced
    perfectly with a provided Curve.

    :returns float: Instrument value.

ZeroCouponBond
++++++++++++++++

.. doxygenclass:: ZeroCouponBond

ForwardRateAgreement
++++++++++++++++++++++++++++++++

.. doxygenclass:: ForwardRateAgreement

Swap
++++++++++++++++++++++++++++++++

.. doxygenclass:: Swap

LegFixed
++++++++++++++++++++++++++++++++

.. doxygenclass:: LegFixed

LegFloat
++++++++++++++++++++++++++++++++

.. doxygenclass:: LegFloat

Misc
++++++++++++++++

.. .. doxygenfile:: YieldCurve.cpp
    :project: Curves

.. .. autodoxygenfile:: files.html

.. .. autodoxygenindex::
