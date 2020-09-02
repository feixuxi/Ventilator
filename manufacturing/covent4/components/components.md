Production Methods for Custom Components

One of the foundational principles of the ventilator design was to avoid, where possible, custom components. However,
for several parts, it was unavoidable to design a new component if price, availability, and functional performance
requirements could not be achieved with already-available components. This document describes the few components that
have been custom designed for this application. The manufacturing process for the prototypes is discussed, as well as
plans and features for production at larger volumes, with an approximate scale-point of 1,000 units. For smaller orders
of less than 100 units production, some of the prototype production methods may prove more cost- and time-effective.

* * *

Pinch Valve
===========

Design Basis
------------

Two custom pinch valves are used to modulate flow through the ventilator: a valve on the inspiratory path works in
conjunction with the blower to enable quick responses to required flow changes; an identical valve on the expiratory
path closes periodically to minimize oxygen wastage due to excess bias flow during high pressure periods of the
breathing cycle.

Flexible tubing-based pinch valves were selected based on the requirements for quick response time, and the ability to
keep the gas pathway clean (a feature unique to pinch valves, as most other forms of actuated valves require passing the
gas through the valve workings).


Pinch valve CAD model and an earlier prototype of the assembled (3d printed) version. Neither shows the section of
flexible rubber tubing, which forms part of the ventilator pneumatic circuit.

The soft rubber tubing selected is manufactured and rated for peristaltic pumps, which have similar loads consisting of
cyclical stresses on the tubing walls. This tubing is ⅝ " OD x ⅜ " ID, somewhat smaller than the main ventilator circuit
tubing standard in order to make it easier to completely seal the tube when the valve is closed. Roller bearings on the
rotor eliminate frictional wear on the outer surface of the tubing. While this tubing is rugged enough for weeks of use
in this application, it will likely need to be replaced periodically. Current testing is underway using a
[life leader](../../quality-assurance/testing) to demonstrate the lifetime of the tubing and to quantify degradation in
performance over time.

Earlier prototype of pinch valve compressing soft rubber tubing to seal the gas pathway

Prototype Fabrication
---------------------

The valves are an assembly of two custom plastic parts with commonly available off-the-shelf hardware. The design of the
custom plastic parts requires minor modification to be amenable to injection molding in ABS plastic, which is the
intended at-scale production method. The parts for the prototype valve assembly are shown below:

Pinch valve components: two custom plastic parts (currently SLA printed in ABS-like resin), stepper motor and driver,
5x15mm roller bearings, hardware, and plumbing fittings

Further design documentation, including assembly instructions, can be found at the RespiraWorks:
[open source documentation page](https://github.com/RespiraWorks/SystemDesign/tree/master/research-development/project-pinch-valve)
for this part.

Fabrication at Scale
--------------------

For the pinch valve fabrication at scale, the key difference would be to use the enclosure mounting plate as part of the
assembly, removing the housing (item 10) in the BOM above. The tube supports would be replaced with routing clamps and
the pinch plate would be a single injection molded ABS piece affixed to the mounting plate.

The rotor is slightly more complicated, and a few different iterations have been proposed. Likely the cheapest and
fastest method to produce would be to procure aluminum 60601 bar stock with the correct outer dimensions. Blanks for
each rotor would be cut and milled to final size. A group of 10-15 rotors would be placed in a vice with the central
channel milled out. The set screw hole would be through-drilled and tapped. A third operation would be required to drill
and tap the axel and bearing shaft holes.

The boss on the rotors to isolate the inner race of the bearing would be replaced with a small shim washer.

Venturi
=======

Design Basis
------------

Two venturi flow sensors are used to measure flow into and out of the patient’s lungs by measuring a change in pressure
as the flow accelerates through the throat. Relative to other pressure sensor types, a venturi flow meter produces the
largest signal pressure at the smallest pressure loss. This allows significantly cheaper and more widely available
pressure sensors to be used, because a signal at peak flow can be 5 to 15 times larger than the flow resistance. For a
resistance type flow sensor, the ratio is 1:1. The change in pressure is measured between the two ports, and this is
correlated to the flow rate through the orifice.

Cross-section view through venturi flow sensor. Blue arrow indicates flow direction.

Prototype Fabrication
---------------------

Currently the prototype parts are 3D printed using an SLA printer in ABS-like resin. Ultimately this design requires
some modification to make it amenable to injection molding, which is the intended method for at-scale production.
Venturis like these are commonly injection molded, so there is precedent for successful injection molding of parts with
the required internal profile. However, the team did not identify any off-the-shelf parts that met the orifice sizing
requirements dictated by the ventilatory flow rates required, leading to a custom design for this part. This part
incorporates molded-in hose barbs, which are compatible with either ¾ " ID or 19mm ID flexible tubing depending on
supply chain availability.

The pressure sensor selected (MPXV5004DP) used with this venturi has 2.5 mm or 3/32" barbs (note: this is not
sufficiently close in size to use more common 3mm or 1/8" tubing and fittings). The venturi currently uses printed
threads to accept an adapter (black, below) from #10-32 to 3/32" barb. This interface may change when the part is
re-designed for injection molding.


Prototype venturi sensors, 3D printed in ABS-like resin,
with pressure sensor hose barb adapters installed.

Further design documentation, including flow rate correlation and characterization of sensor accuracy, can be found at
the RespiraWorks
[open source documentation page](https://github.com/RespiraWorks/SystemDesign/tree/master/research-development/project-venturi)
for this part.

Fabrication at Scale
--------------------

The basis for the design came from previous experience using adapted fertilizer injection venturis as flow measurement
devices. The original part is shown below.

![](images/image14.png)

Injection molded ABS fertilizer venturi

There is no fundamental reason why the exact same design cannot be used for this application, especially if these are
being produced at scale. However, the dimensions of this part are not quite right to optimize for the flow regime of
patient inspiration. The commonly available ½-NPT fertilizer venturi has an entrance diameter of 12.7mm and a throat
diameter of 4.1 mm. After experimentation with the prototype venturi, the final dimensions were selected as an entrance
diameter of 15.05 mm and a throat diameter of 5.5 mm. These are significantly different from a flow measurement
standpoint, but not particularly different from a manufacturing standpoint.

The fabrication for the above process is to produce two mirror half-molds with partial thread blanks for the three
threads and alignment pins in the molds. Glue is applied to the mold halves, with the pins used to precisely align the
components. After the glue sets, additional flashing and the alignment pins are machined off, and the threads are
cleaned up with a cutting die. By using common thread fittings (½ NPT on other end, and ¼ NPT on the pressure tap)
readily available adapters can be attached. For the upstream pressure tap, a ½ NPT tee is used, with the flow entering
the tee from upstream, and a barb adapter used to connect to the 3/32 tubing. For the throat tap, a ¼ NPT female to
3/32 barb adapter is required.
