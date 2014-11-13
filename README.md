optics
======

Some quantum/nonlinear optics calculators and a QKD Monte Carlo simulation.  For technical details, see:

http://books.google.com/books?hl=en&lr=&id=RogcPECSNewC

fwmbw.c - a calculator for the four-wave-mixing bandwidth of an AlGaAs waveguide, including a calculation of the Sellemeier coefficients for AlGaAs.

fractional_clock.c - a Monte Carlo simulation of BB84 QKD.  Called fractional_clock because, unlike an earlier version, this one operates in real-time; the simulation clock tick can be a fraction of the Monte Carlo QKD clock cycle.
