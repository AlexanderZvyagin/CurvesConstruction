++++++++++++++++
Curve
++++++++++++++++

.. doxygenclass:: Curve

.. cpp:class:: Curve

    The curve is defined as one dimensional interpolation of :math:`n`
    points :math:`(t_i,r_i);\ \ \ i=1\dots n`.

    - :math:`t` is time, the time *now* is :math:`t=0`;
    - :math:`r` is short rate.

    The following interpolation types are recommended to use:

    - PiecewiseConstant
    - Linear
    - CubicSpline
    - Steffen

.. cpp:function:: BuildPiecewiseConstant (void)

    Build method for curve construction applicable
    when *PiecewiseConstant* interpolation is used.

    The algorithm is very simple.

    - remove previously constructed interpolation (if exists)
    - start a loop on the curve instruments (in maturity increasing order)
    - for every instrument call a function AddToCurve().

      AddToCurve() function adds a new point :math:`(t,r)` to the curve.

      - :math:`t` is the instrument maturity
      - :math:`r` is the rate with which the instrument will be priced correctly.

      Because of the peicewise interpolation behaviour,
      adding a new point will not modify evaluation of instruments
      added previously.

    Algorithm features/limitations.

    - If two (or more) instruments have the same maturity,
      only one of the can be used. For example FRA(start=0,length=1Y) and
      FRA(start=6M,length=6M) will have the same maturity 1 year, and only
      the instrument added last will be used by the algorithm.
    - The time grid (interpolation points on time X-axis)
      is determined by the instrument maturities.

    See :ref:`Instrument`

.. cpp:function:: operator float (float t) const

    Returns the short rate :math:`r(t)`.

    :param float t: time point
    :return: short rate at time point :math:`t`

.. cpp:function:: float GetDiscountFactor (float t) const

    Calculates the discount factor at the given time point.

    :math:`P(t) = \exp\left( -\int_0^t r(u) \textrm{d}u \right)`

    :param float t: time point
    :return: Discount factor at time point :math:`t`

.. cpp:function:: float GetDiscountFactor (float t, float T) const

    Calculates the forward discount factor between the given time points.

    :math:`P(t,T) = \exp\left( -\int_t^T r(u) \textrm{d}u \right)`

    :param float t: time point
    :param float T: time point
    :return: Discount factor :math:`P(t,T)`

.. cpp:function:: float GetForwardRate (float t, float T) const

    Calculates the forward rate between time points :math:`(t,T)`

    :math:`f(t,T) = \frac{1}{T-t} \int_t^T r(u) \textrm{d}u`

    :param float t: time point
    :param float T: time point
    :return: Forward rate :math:`f(t,T)`
