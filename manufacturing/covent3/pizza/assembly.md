#Assembly Instructions

This document contains instructions on how to assemble the component parts as well as the entirety of the ventilator
prototype. Note that these are the instructions to assemble the v0.2 prototype, which is still subject to significant
revision. These assembly instructions are adapted from the prototype build and are not necessarily instructive for the
full ventilator. Please consult with info@respira.works before purchasing to ensure the status of the design.

Overview
--------

This is a ventilator prototype for testing and development. The build consists of a functional ventilator pneumatic
assembly with controller and user interface. This build is adequate for integration and systems testing. It lacks
heating, humidification, battery backup and air filtering. It is not enclosed like the final product and is meant to be
operated in an open layout.

The prototype assembly is shown below, note that the oxygen inlet line is not included in this prototype.
![](images/image3.png)


A. Assemble sensors, drivers, and pneumatic components
------------------------------------------------------

First, you will need to assemble the sub-components of the ventilator.

*   Blower: The main driver of pressure in the pneumatic circuit.
*   2 Venturi flow sensors: To provide the controller feedback.
*   Proportional pinch valve: A proportional pinch valve can control the airflow constriction with much better
precision. Multiple such pinch valves are used in the prototype, the part is buildable from 3d-printed plastic
components and easily obtainable generic parts. The ventilator requires two pinch valves.

Use the pictures and diagram above to help with assembly.

Note: Mind which tubes go into the bottom and top ports of the sensors on the PCB.

B. Create base for tabletop ventilator prototype
------------------------------------------------

A base for the table top ventilator can be created out of any sturdy material, such as plywood, preferably with some
4cm high supports:

![](images/image47.jpg)

![](images/image35.jpg)

In the picture on the right we have marked down positions for the components to be attached.

C. Attach Sub-Assemblies
------------------------

1.  Bolt down the pinch valves to the base using some M5 bolts, washers and lock nuts.

![](images/image6.jpg)

![](images/image12.jpg)

2.  Take the tube that belongs with pinch valve and attach it to the blower sub-assembly. lide the tube through pinch
valve and bolt down the blower from the back, using some M2.5 thread-forming screws:


![](images/image28.jpg)
-----------------------

![](images/image4.jpg)

3.  Assemble a tee for the oxygen branch (in development), with a transition from ⅜ "ID tubing to ¾ "ID tubing, about 8
inches of ¾ "tubing and the inflow venturi. Here we have highlighted the flow direction arrow on the venturi:
    ![](images/image18.jpg)

4.  Connect the oxygen tee and venturi to the first pinch valve to complete the inhale limb of the pneumatic assembly:
    ![](images/image17.jpg)
5.  Assemble the exhale limb of the pneumatic assembly, which includes the outflow venturi, 15cm of ¾ "ID tubing, a
tubing adapter and the ⅜ "ID tubing for the second pinch valve. Then, slide it into the pinch valve and zip tie the
adapter to secure it to the base board.


![](images/image40.jpg)

![](images/image37.jpg)

D. Assemble electrical components
---------------------------------

These computing and digital user interface components enable full deploy and execution of all developed software.

6.  Secure the blower driver board to the base and attach spacer standoffs for supporting the main circuit board:
    ![](images/image31.jpg)

7.  Install the PCB, running the blower’s power and control cables in the space between the base board and the PCB:


![](images/image9.jpg)

![](images/image27.jpg)

8.  Move the JP5 jumper in the upper-middle-center of the Nucleo board to the E5V position. This tells the board to
    expect external power from the PCB. This will avoid programming problems. If you wish to remove the Nucleo board and
    work with it on its own without the PCB, move the jumper back to the U5V position to power it from USB.

9.  Prepare the stepper boards, and label the pigtails accordingly. Also prepare a couple of 16AWG wires (~4cm each) for
    powering both boards.
    ![](images/image21.jpg)
10.  Run the pigtails from the back of the board, connecting them to the pinch valves as follows:


![](images/image22.jpg)

![](images/image36.jpg)

11.  Stack the stepper boards onto the Nucleo and wire up as follows:


![](images/image16.jpg)

Additional schematic and model files can be found at the RespiraWorks:
[open source documentation page](https://github.com/respiraworks/pcbreathe) for this part.

### Raspberry Pi

Plug Raspberry Pi 4 into the Raspberry Pi socket. Note that if you have the standoffs fitted, do not over-tighten them
as this can damage the Raspberry Pi.


E. Connect sensor tubing
------------------------

Connect tubing to the exhale venturi and inhale venturi, check the arrow on the venturis for direction to ensure the
ports are correctly oriented.  Connect the tubing from the inhale venturi to the board mounted pressure sensor using
2.5mm tubing. Note that of the two ports on the differential pressure sensors on the circuit board - the upper port
must connect to the upstream orifice of the venturi, while the lower port must connect to the downstream orifice.

1.  Fist, connect tubing to the inhale venturi. The tubes should be about 20 and 18 cm long.
    ![](images/image19.jpg)

2.  Now, connect a tube about 25 cm long from the downstream of the exhale venturi to the lower port of the exhale
    sensor.
    ![](images/image42.jpg)
3.  Run a tube (13cm) from the upstream orifice of the exhale venturi to a tee connector, and another tube (10cm) from
    tee connector to the upper port of the patient pressure sensor.
    ![](images/image5.jpg)
4.  The last tube (15cm) runs from the tee connector to the upper port of the exhale sensor.
    ![](images/image10.jpg)
5.  Lastly, you may connect the test lung to the venturis, clamp the patient tubing and secure the venturis to the board
    with some zipties:

    ![](images/image7.jpg)


F. Connect touchscreen
----------------------

Connect an HDMI cable (included with Touchscreen package) between the Pi and the Touchscreen. Similarly, connect a USB
micro cable (included with Touchscreen package) between the Pi and the TOUCH+5V connector on the Touchscreen.

G. Connect power cable
----------------------

The PCB must be powered in order for the Nucleo to program correctly. A power cable which plugs into the upper left
corner of the PCB is provided with the board and allows it to be powered from any 12V, 1.2A or greater power adapter
with a 5.5x2.1mm center-positive barrel jack.

With this, the table-top ventilator prototype is complete:

![](images/image3.png)
