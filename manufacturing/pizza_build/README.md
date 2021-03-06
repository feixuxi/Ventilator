# Pizza Build

## DISCLAIMER

**This is an incomplete, as yet untested design that is not yet intended to be used on patients.**

This is a ventilator prototype for testing and development.
The build consists of a functional ventilator pneumatic assembly with controller and user interface. This build is adequate for integration and systems testing.

It lacks heating, humidification and battery backup.

It is also not enclosed like the final product and is meant to be operated in an open layout on your table, like a pizza.

It can also come in various configurations, depending on your testing needs:

| Solenoid valve setup  |  Pinch valve setup   | Blower bypass setup     |
|:---------------------:|:--------------------:|:-----------------------:|
![](images/photo-solenoid.jpg) |![](images/photo-pinch.jpg)  |![](images/photo-advanced.jpg)  |

## Essentials

The first thing you will need is a [Brain build](../brain), which contains the computing components.

We are using 3/4" ID tube as the default standard (and adapters where needed, like to the blower and the pinch valves).
This easily can have 19mm ID tubing substituted in metric manufacturing settings (3/4" = 19.05mm).
All rigid plumbing fittings should be NPT/imperial standard (3/4 NPS preferable, to keep ID consistent throughout the flowpath).

## Parts List (BOM)

**Note: If you are a member of the RespiraWorks team, review the
[part purchasing guidelines](../../manufacturing/README.md#part-purchasing-guidelines)
BEFORE purchasing any parts.**

### Purchasing Source Key

* **Z** = Amazon
* **C** = McMaster-Carr

### 3/4" ID Tubing Components

| Item | Quantity | Manufacturer  | Part #              | Price (USD)     |  Sources       | Notes |
| ---- |---------:| ------------- | ------------------- | ------------:|----------------| ----- |
| a1   |      5ft |      OCSParts |            117062-5 | 22.99 / 5ft  | [Z][a1amzn]    | soft 3/4"ID 1"OD tubing |
| a2   |      5ft | McMaster-Carr | 5233K71             | 14.20 / 10ft | [C][a2mcmc]    | hard 3/4"ID 1"OD tubing, **alternative to a1**, not as fun to work with, not for medical use |
| a3   |        6 | McMaster-Carr | 9579K73             | 12.24 / 20   | [C][a3mcmc]    | Herbie clip K (22.2-24.9mm) for clamping 1"OD tubing, works better for soft tubing |
| a4   |        6 | McMaster-Carr | 9579K74             | 12.33 / 20   | [C][a4mcmc]    | Herbie clip L (24.5-27.6mm) for clamping 1"OD tubing, **alternate to a3** maybe works better for hard tubing? |
| a5   |        1 | McMaster-Carr | 5463K648            | 10.20 / 10   | [C][a5mcmc]    | 3/4"ID <-> 3/8"ID adapter, to interface with pinch valve tubing |


[a1amzn]: https://www.amazon.com/gp/product/B01LZ5ZK53
[a2mcmc]: https://www.mcmaster.com/5233K71
[a3mcmc]: https://www.mcmaster.com/9579K73
[a4mcmc]: https://www.mcmaster.com/9579K74
[a5mcmc]: https://www.mcmaster.com/5463K648

### Metric Version

**#TODO: document how to build with metric components**


### Tools

Assembling the pneumatic system will likely need the following tools.
T1 might be enough and T4 might be unnecessary.
Also, scissors or a sharp knife might be ok instead of T3.

| Item | Manufacturer  | Part number        | Price (USD) | Sources         | Notes |
| ---- |--------------:| ------------------ | -------- | ---------------:|------ |
| c1   |        TEKTON |              37122 |    13.00 | [Z][c1amzn]     | 8 inch slip-joint pliers, for clamping tubes onto fittings |
| c2   |   Duda Energy |     LPpvc075-005ft |    19.99 | [Z][c2amzn]     | Adjustable wrench, at least 1" wide opening, for screwing in NTP fittings |
| c3   |        DEWILL |          (unknown) |    15.88 | [Z][c3amzn]     | Pipe and PVC cutter, for cutting tubing |
| c4   |         IRWIN |            2078216 |     8.90 | [Z][c4amzn]     | Long nose pliers, for removing herbie clips |
| c5   |          PTFE | vanguardSealantsPTFE-01 |     6.49 | [Z][c5amzn]     | It's advisable to teflon tape all NTP threaded components. Not only will they be more airtight, but also they will be easier to tighten and release. |

[c1amzn]: https://www.amazon.com/TEKTON-2-Inch-Joint-Pliers-37122/dp/B00KLY1FAY
[c2amzn]: https://www.amazon.com/GETUPOWER-10-Inch-Adjustable-Opening-Vanadium/dp/B07RGV2VK1
[c3amzn]: https://www.amazon.com/gp/product/B07Y997XKC
[c4amzn]: https://www.amazon.com/Tools-VISE-GRIP-Pliers-6-Inch-2078216/dp/B000A0OW2M
[c5amzn]: https://www.amazon.com/gp/product/B01L2F428C


## Sub-assemblies

The following are essential sub-assemblies that you'll need no matter what additional/optional "toppings" you chose to have.

* 1x [Blower assembly](../blower) - the main driver of pressure in the pneumatic circuit
* 2-3x [Venturi flow sensors](../venturi) and corresponding tubing, to provide the controller feedback
* 2x [Pinch valves](../pinch_valve) - a proportional pinch valve to control the airflow constriction with precision
* optional [Air filter assemblies](../filter_holder) - for the safety of patient and health workers. Constricts air flow
  to some extent and thus will affect the ventilator performance

## Assembly Instructions

| Pneumatics schematic  |  Shitty diagram     |
|:---------------------:|:--------------------:|
![](images/pizza-pneumatics.png) |![](images/pizza-diagram.jpg)  |

[Pneumatics diagram source](images/pizza-pneumatics.eps)

### Create a base

A base for the table top ventilator can be created out of any sturdy material, such as plywood, preferably with some
4cm high supports:

|         |          |
|:-------:|:--------:|
| ![](images/base_marked.jpg) | ![](images/base_legs.jpg) |

In the picture on the right we have marked down positions for the components to be attached.

### Attach Sub-Assemblies

1. Bolt down the pinch valves to the base using some M5 bolts, washers and lock nuts.

|         |          |
|:-------:|:--------:|
| ![](images/pinch_valves_front.jpg) | ![](images/pinch_valves_back.jpg) |

2.  Take the tube that belongs with pinch valve and attach it to the blower sub-assembly. lide the tube through pinch
valve and bolt down the blower from the back, using some M2.5 thread-forming screws:

|         |          |
|:-------:|:--------:|
| ![](images/blower_unattached.jpg) | ![](images/blower_attached.jpg) |

3.  Assemble a tee for the oxygen branch (in development), with a transition from ⅜ "ID tubing to ¾ "ID tubing, about 8
inches of ¾ "tubing and the inflow venturi. Here we have highlighted the flow direction arrow on the venturi:
4.  Connect the oxygen tee and venturi to the first pinch valve to complete the inhale limb of the pneumatic assembly:

|         |          |
|:-------:|:--------:|
| ![](images/oxy_tee_unattached.jpg) | ![](images/oxy_tee_attached.jpg) |

5.  Assemble the exhale limb of the pneumatic assembly, which includes the outflow venturi, 15cm of ¾ "ID tubing, a
tubing adapter and the ⅜ "ID tubing for the second pinch valve. Then, slide it into the pinch valve and zip tie the
adapter to secure it to the base board.

|         |          |
|:-------:|:--------:|
| ![](images/exhale_unattached.jpg) | ![](images/exhale_attached.jpg) |

### Assemble electrical components

These computing and digital user interface components enable full deploy and execution of all developed software.

1.  Secure the blower driver board to the base and attach spacer standoffs for supporting the main circuit board:
    ![](images/blower_driver.jpg)

2.  Install the PCB, running the blower’s power and control cables in the space between the base board and the PCB:

|         |          |
|:-------:|:--------:|
| ![](images/pcb1.jpg) | ![](images/pcb2.jpg) |

3.  Move the JP5 jumper in the upper-middle-center of the Nucleo board to the E5V position. This tells the board to
    expect external power from the PCB. This will avoid programming problems. If you wish to remove the Nucleo board and
    work with it on its own without the PCB, move the jumper back to the U5V position to power it from USB.

4.  Prepare the stepper boards, and label the pigtails accordingly. Also prepare a couple of 16AWG wires (~4cm each) for
    powering both boards.

![](images/stepper_boards.jpg)

5.  Run the pigtails from the back of the board, connecting them to the pinch valves as follows:

|         |          |
|:-------:|:--------:|
| ![](images/stepper_connection1.jpg) | ![](images/stepper_connection2.jpg) |

6.  Stack the stepper boards onto the Nucleo and wire up as follows:

![](images/stepper_connection3.jpg)

Additional details for how to attach stepper drivers to PCB can be found in the
[pcb section](../../pcb/rev1_export/stepper_driver_setup.md) of the repository.

### Raspberry Pi

Plug Raspberry Pi 4 into the Raspberry Pi socket. Note that if you have the standoffs fitted, do not over-tighten them
as this can damage the Raspberry Pi.

**#TODO:** Needs picture

### Connect sensor tubing

Connect tubing to the exhale venturi and inhale venturi, check the arrow on the venturis for direction to ensure the
ports are correctly oriented.  Connect the tubing from the inhale venturi to the board mounted pressure sensor using
2.5mm tubing. Note that of the two ports on the differential pressure sensors on the circuit board - the upper port
must connect to the upstream orifice of the venturi, while the lower port must connect to the downstream orifice.

1.  Fist, connect tubing to the inhale venturi. The tubes should be about 20 and 18 cm long.
2.  Now, connect a tube about 25 cm long from the downstream of the exhale venturi to the lower port of the exhale
    sensor.
3.  Run a tube (13cm) from the upstream orifice of the exhale venturi to a tee connector, and another tube (10cm) from
    tee connector to the upper port of the patient pressure sensor.
4.  The last tube (15cm) runs from the tee connector to the upper port of the exhale sensor.

|         |          |
|:-------:|:--------:|
| ![](images/sensor_tubing1.jpg) | ![](images/sensor_tubing2.jpg) |
| ![](images/sensor_tubing3.jpg) | ![](images/sensor_tubing4.jpg) |

### Connect test lung

Lastly, you may connect the test lung to the venturis, clamp the patient tubing and secure the venturis to the board
with some zipties:

![](images/test_lung.jpg)

### Connect touchscreen

Connect an HDMI cable (included with Touchscreen package) between the Pi and the Touchscreen. Similarly, connect a USB
micro cable (included with Touchscreen package) between the Pi and the TOUCH+5V connector on the Touchscreen.

### Connect power cable

The PCB must be powered in order for the Nucleo to program correctly. A power cable which plugs into the upper left
corner of the PCB is provided with the board and allows it to be powered from any 12V, 1.2A or greater power adapter
with a 5.5x2.1mm center-positive barrel jack.

With this, the table-top ventilator prototype is complete:

![](images/assembled.jpg)
