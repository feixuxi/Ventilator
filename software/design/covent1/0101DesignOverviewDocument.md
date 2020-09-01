Software Design
===============

The RespiraWorks ventilator includes software running on two computers:
an STM32 microcontroller and a Raspberry Pi.

The microcontroller runs bare-metal and controls the sensors and
actuators (valves and blower).  Its main loop runs every
10ms on a hardware timer interrupt,
and the code running in the main loop contains no loops of variable
length, ensuring minimal jitter. The microcontroller’s software is
written in C++.

The Raspberry Pi runs Linux and is used for displaying the GUI. The
software we wrote is a Qt app, also written in C++. The app displays a
GUI on the ventilator’s touch screen, showing sensor data received from
the controller. Users set high-level parameters via the GUI (e.g. “PIP
20 cmH2O”), and then the GUI software commands the controller to meet
these parameters. The GUI software is also responsible for calculating
alarms and readings derived from raw sensor values (such as measured
PIP/PEEP).

We chose to use two separate computers for three reasons. First, two
computers reduces the amount of software that is immediately hazardous
to the patient. Since our UI code runs on a separate device with no
direct access to the ventilator’s actuators, a bug or crash in it is
less likely to cause immediate harm; the ventilator will just keep
running with the old params (and, in the future, sound an alarm).
Second, this design saves time and money because it allows us to use
large libraries like Linux and Qt, which make it easier to develop the
GUI, but would likely be unacceptable in a patient-critical module like
the cycle controller. Third, having two modules adds a level of
redundancy. If the microcontroller crashes, it will immediately receive
new commands from the Raspberry Pi, and the Pi can raise an alarm.
Similarly, if the Pi crashes, the ventilator continues running with the
last set of parameters, and the cycle controller can raise an alarm.
(Disclosure: Neither of these alarms is currently implemented, and we
have not performed extensive testing of this failure model.)

Like the rest of the RespiraWorks project, our software is developed in
the open on GitHub. We have an extensive continuous integration
infrastructure running unit tests on x86, and we are building
infrastructure to run automated tests on the full device. We are also
nearly ready to start testing our software against a Modelica simulation
of the device. We hope this will make it easier for developers who do
not have access to hardware to contribute to the project. We also
believe having a robust simulation environment will allow us to run
tests that would otherwise be prohibitively time-consuming, for instance
running each commit against many combinations of ventilator parameters
plus lung compliances.

Besides a detailed user manual and maintenance plan, the device also
comes with a robust self-test mode.  This ensures that produced
ventilators achieve necessary safety levels before they are used with
patients and will allow users to check the health of the device over its
lifetime. A cryptographic hash will be released with the approved
software, and allow local manufacturers to be confident in the software
they are loading to the device.


Communication Between Pi and Microcontroller
--------------------------------------------

The microcontroller and Raspberry Pi communicate over a serial bus using
a simple, predictable protocol. Every \~50ms, the microcontroller sends
its current state to the Raspberry Pi, and every \~50ms, the Raspberry
Pi sends its full state to the microcontroller.  There are no ACKs,
resends, detection of missed packets, etc.

This simplified communication protocol avoids failure modes present in
other protocols.  For instance, imagine a different, stateful protocol
where one command the GUI can send is “set PIP to X”.  Suppose the GUI
sends two packets, “set PIP to 15” and “set PIP to 20”, and imagine that
the first one gets corrupted or lost on the wire.  The controller must
not accept the “PIP 20” command until it receives and applies the “PIP
15” command, otherwise the final PIP will be 15!  This requires buffers,
resends, and complex logic that, in the limit, looks much like a full
implementation of TCP.

Our design sidesteps this whole class of problems. If a message gets
missed or corrupted, we can just wait for the next one.

The state is encoded for transport on the wire using protocol
buffers (specifically, nanopb) and
wrapped in a frame with a checksum for detecting packet corruption. As
compared to a fully custom binary format, this makes it far easier to
change the protocol and potentially interoperate with other systems.

Cycle Controller
----------------

Our cycle controller runs on an STM32 ARM chip and is written in C++17.

### Why C++?

C is a much more common programming language in the embedded space, so
our choice of C++ deserves some discussion.

Simply put, we chose C++ over C because we believe it is a more
productive programming language.  Here are some advantages we’ve found
to using C++.

-   In C we’d have to represent a pressure measurement as a number, e.g.
    a float.  This is
    [error-prone](https://www.google.com/url?q=https://mars.nasa.gov/msp98/news/mco990930.html&sa=D&ust=1598802820324000&usg=AOvVaw1JJGniE2Ej0cV9kzlLRVHm) in
    part because the compiler can’t help you keep your units consistent.
    In contrast, our C++ code represents a pressure measurement using an
    explicit
    [Pressure](https://www.google.com/url?q=https://github.com/RespiraWorks/VentilatorSoftware/blob/master/common/libs/units/units.h&sa=D&ust=1598802820325000&usg=AOvVaw1lzJ287EMVabEsr3ukSM0a) type.
    You can create a Pressure from a measurement in kPa or cmH2O and the
    software will do the unit conversions automatically. We also support
    operators between types.  For instance, dividing a Volume by a
    Duration yields an object of type VolumetricFlow, again with the
    units automatically correct. This is a zero-cost abstraction -- it
    runs just as fast as the equivalent C code -- and it eliminates
    whole classes of bugs.\
-   We use pieces of the C++17 standard library to add safety to our
    code.  For instance, we use C++’s std::variant class for
    discriminated unions instead of C’s union construct.  We also use
    std::optional throughout the code to represent a value that may not
    be present.  This is much safer than most C solutions (e.g. using a
    sentinel value like -1 or NaN to mean “not present”).
-   Some language features of C++ are inherently safer than the
    respective features of plain C. For example, C++ requires the
    programmer to be explicit about type conversion in cases where the
    conversion may be lossy (e.g. from a floating-point to an integer
    type, or from an integer type to a more narrow integer type). In
    contrast, C allows such conversions implicitly, failing to protect
    against a common source of errors in safety-critical software.

We have configured our environment to disallow dynamic memory
allocation, and we do not use C++ exceptions.

### Software Provenance

We wrote all of the cycle controller software from scratch, with two
exceptions:

-   We use
    [nanopb](https://www.google.com/url?q=https://github.com/nanopb/nanopb&sa=D&ust=1598802820326000&usg=AOvVaw32h1Wx44EbdQLQ1Rf3RZ_P) for
    protocol buffer encoding/decoding. Nanopb is an extensively tested
    library used in many projects, and protocol buffers are an industry
    standard for data exchange.
-   Our PID controller is based on an [Arduino
    library](https://www.google.com/url?q=https://github.com/br3ttb/Arduino-PID-Library/&sa=D&ust=1598802820327000&usg=AOvVaw1t1c78Ws5W1u3T7RY1ghsN),
    although our version has evolved beyond the point of recognition --
    every line of code has been rewritten and our version is several
    times smaller by line count.  It has fewer features, and we have
    added extensive unit tests.

Notably, all of our code for interacting with the STM32 hardware is
bespoke; we are not using the Arduino STM32 hardware abstraction layer
or any other third-party HAL. This ensures auditability over every line
of code in this critical part of the system.

### Software Design

The cycle controller follows a modular design, described below.

![](images/image12.png)

The main loop runs every 10ms and has the following responsibilities:

-   Reset the watchdog timer. If the watchdog timer is not reset within
    250ms, the microcontroller assumes it is stuck and restarts.
-   Instruct comms to send a packet to GUI if necessary, and check
    whether comms has received a packet from the GUI.
-   Instruct controller to recalculate desired actuator positions, based
    on the commands in the latest packet received from the GUI.

Comms communicates with the GUI app running on the Raspberry Pi. Recall
that the communication protocol is simply that the controller
periodically sends all its state to the GUI, and the GUI periodically
sends all of its state to the controller, overwriting any previous
state. The protocol is defined
[here](https://www.google.com/url?q=https://github.com/RespiraWorks/VentilatorSoftware/blob/master/common/generated_libs/network_protocol/network_protocol.proto&sa=D&ust=1598802820328000&usg=AOvVaw31f9RjZArgBu1iWTBflj7I).

On each iteration of the main loop, the controller component forwards
the parameters from the GUI (e.g. PEEP 5, PIP 20, …) to the breath
FSM (finite state machine, see below), which responds with a “desired
system state”, the physical properties we want the pneumatic system to
have at this moment. Since we have implemented pressure-controlled
ventilation modes, the main physical property to achieve is patient
pressure. The controller reads from the sensors, uses PID to calculate
the valve positions and fan power which it thinks will achieve the
desired state, and forwards these to the actuator.

The breath FSM (finite state machine) translates ventilator parameters
into a timeseries of desired states of the ventilator’s pneumatic
system. For example, the ventilator parameters “command pressure mode
with PEEP 5, PIP 20, RR 12 bpm, I:E 0.25” translates to the timeseries
“start pressure at 5 cmH2O, linearly ramping up to 20 cmH2O over 100ms;
sit at 20 cmH2O for 900ms; then drop to 5 cmH2O for 4s”.

The breath FSM is also responsible for inspiratory effort detection in
pressure assist mode. In this case, the time series would keep pressure
at PEEP until either a breath is detected or it has been too long
between breaths.

The sensors module reads raw sensor voltages from HAL (hardware
abstraction layer, see below) and translates them into physical
measurements. It is responsible for calibrating the system’s three DP
sensors and translating the readings into two flow measurements (one for
each venturi) and one patient pressure measurement.

The actuators module receives a set of commands from the controller for
every actuator in the system, e.g. “blower speed 90%, inspiratory pinch
valve 28% open, etc.”, and forwards them down to HAL to be acted upon.

The actuators module might seem like an unnecessary component -- why
can’t the controller simply call into HAL itself?  There are two
reasons.

-   Separating the choice of actuation state from actual actuation lets
    us unit test the controller.
-   When we run in simulation with Modelica for software-in-the-loop
    testing, we replace the real actuators component with a component
    that sends the commands to Modelica.

Finally, our HAL (hardware abstraction layer) is responsible for
communicating directly with the hardware, e.g. reading a voltage from
the STM32’s analog-to-digital converter. The HAL also sends commands to
the stepper drivers that control the ventilator’s pinch valves.

One last component which does not fit in the diagram or description
above is the debug module.  Among
other things, this powerful module lets us read and write values in the
controller without reflashing the device. We have used it to tune PID in
a live system, to operate the ventilator without the GUI attached (e.g.
for automated testing), and to capture and graph the ventilator’s
internal state as it runs.

### Algorithms of note

This section describes some significant algorithms implemented in the
cycle controller.

#### Valve control

The controller translates a desired patient pressure into
inspiratory/expiratory valve positions using closed-loop PID control.
The PID’s integral term is set based on the PEEP-to-PIP pressure delta
(i.e. some minor gain scheduling). The expiratory valve tracks the
inspiratory valve; as the inspiratory valve opens more, the expiratory
valve closes more.

#### Venturi pressure to flow conversion

We characterized the response of our venturis over a large range of
flows using a Fleisch pneumotachograph. We used a standard venturi
pressure-to-flow formula with a correction factor of 0.97 as the only
correction applied. The comparison between the two instruments matched
our empirical measurements very closely. The 0.97 correction factor is
in line with ISO recommendations for smooth surfaces with a Reynolds
number of \~104.

#### Volume zeroing

The net volume change over a breath is not always exactly 0, due to
leakage, sensor zero-point drift, and the fact that venturis have
relatively high error at low flows. A simple way to correct for this
would be to set volume at the beginning of every breath to 0, but this
introduces a discontinuity in the graph at each breath, which looks
wrong. And fundamentally it does not solve the problem; one can still
observe that volume measurements are “sloped”.

Our volume zeroing algorithm addresses this. At the start of each
breath, we predict what the volume would be at the next breath if the
flow error remained constant. We then apply a flow offset to drive the
next breath’s volume to 0.

This works well on test lungs, but more work is required to characterize
its behavior in more realistic situations, like coughing, airway
blockage, etc. We also need to understand better users’ expectations
about how flow leakage should show up in the ventilator’s graphs. We
expect we will need a more sophisticated algorithm.

#### Inspiratory effort detection

We use the following algorithm to detect inspiratory effort in pressure
support mode.  First, we wait for flow to become nonnegative during the
exhale phase. Then we start keeping two [exponentially-weighted moving
averages](https://www.google.com/url?q=https://en.wikipedia.org/wiki/Moving_average%23Exponential_moving_average&sa=D&ust=1598802820332000&usg=AOvVaw16YnTD2TU9-RmsnOgfXU10) of
flow. The “slow average” has a small alpha term and thus reacts slowly
to changes in flow. The “fast average” has a large alpha term and thus
reacts quickly. We can think of the slow average as characterizing
“normal flow” during the expiratory cycle (which we’ve observed on test
lungs does change, but slowly), while the fast average calculates
“current flow”.  When the fast average exceeds the slow average by a
certain threshold, that triggers a breath.

This works well on our test lungs, but much more testing is needed to
see how it performs in more realistic situations. Graphs and a demo
video are available in [02-1 Performance
Evaluation](https://www.google.com/url?q=https://docs.google.com/document/d/1g7qLD5qD4BKfR1mcGq7-QY6XE2C9oIIw5GJdUzU31Zg/edit?ts%3D5eefc588%23heading%3Dh.pt7ef8fp4ywf&sa=D&ust=1598802820333000&usg=AOvVaw3PSoAKrxDU1RlhGSshEkvd).

User Interface (UI) Controller
------------------------------

The UI controller runs on a Raspberry Pi with a touchscreen and is
written in C++17 using QT as the user interface framework. The reasoning
behind this technology choice is described above, in the “Software
Design” section. Below we discuss the GUI features and architecture in
more detail.

### Features

The most important features of the UI are displaying data and adjusting
ventilation parameters.

-   Data Display: The UI controller presents the user with the data
    coming from the ventilator, including both the actual sensor data
    and values that are derived from it (e.g. tidal volume and PIP/PEEP
    of the most recent breath). The data presented includes the standard
    respiratory cycle plot that most doctors and technicians would
    expect.
-   User Mode / Parameter Setting and Adjustment: The UI controller
    allows the user to set the desired ventilator mode and the desired
    control settings for that mode. The two modes currently supported
    are Pressure Control and Pressure Assist, as they are the modes
    supported by the Cycle Controller. The settings and ventilation mode
    can be changed at any time during operation, and adjusting a setting
    requires confirmation.
-   System state display: The UI controller displays a power source /
    battery status indicator and current time. Currently the battery
    status indicator uses a hard-coded value because the physical design
    lacks sensors to provide that data.

The UI implements an alarm subsystem based on ISO-60601-1-8.

-   Alarm State Detection: The UI controller is responsible for alarming
    when off-nominal events occur that require a clinician’s attention.
    The currently implemented alarms include over/under pressure (PIP is
    overshot or undershot by a certain amount compared to the commanded
    value) and disconnection of the patient circuit. The alarm
    architecture is robust and extensible, allowing us to easily add
    other alarms in the future.
-   Alarm Alerting and Information Display: The UI controller alerts the
    user to alarms through visual and audio cues:

-   Whenever an alarm condition is active, relevant sections in the UI
    are highlighted with color corresponding to the alarm priority. For
    example, over/under pressure alarms highlight the pressure graph.
-   If an alarm condition has been triggered, the UI emits an audio
    signal compliant with ISO 60601-1-8 according to the alarm priority
    until the alarm is acknowledged. If there are multiple
    unacknowledged alarms, the audio is based on the highest-priority
    one.
-   In ISO terminology, the audio signal is “latching”: it persists even
    if the alarm condition is no longer active, so that the operator is
    made aware of a prior hazardous condition even if the condition
    activates sporadically and disappears before the operator walks up
    to the device upon hearing the audio signal.

-   Alarm Lifecycle: Whenever there are any unacknowledged alarms, the
    UI displays a banner showing the highest-priority currently
    unacknowledged alarm and offering to pause this alarm’s audio signal
    for 2 minutes.
-   Silenced alarms: The UI has a widget displaying the number of
    currently active but silenced (acknowledged) alarms, the highest
    priority of the alarms, and a countdown until the highest-priority
    alarm is unpaused.

### Software architecture

The GUI follows a standard layered Model-View Architecture depicted in
the following diagram.

![](images/image10.png)

Architecture of the RespiraWorks GUI

The GUI talks to the Cycle Controller over the serial bus, sending
commanded values of ventilation mode and ventilation parameters, and
receiving current sensor readings as well as a “breath id” value that is
used to establish breath boundaries for computing per-breath signals.

The state of the GUI is encapsulated in a State Container object, which
contains current parameter values to be commanded, recent history of
sensor readings for displaying time series graphs, as well as objects
that manage calculation of per-breath signals, detection of alarm
conditions and management of alarm lifecycle.

The presentation layer is written in QML (the markup language used by
the QT framework), where properties of various visual and audio elements
are wired together with aspects of the State Container, including
bidirectional updates.

Breath signals and the alarm subsystem deserve further discussion.

#### Breath signals

As noted above, each set of sensor readings obtained from the Cycle
Controller includes a “breath id” value, which changes on breath
boundaries. Breath boundaries are mandated or detected by the Cycle
Controller. This value can be used to partition the stream of sensor
readings into intervals corresponding to a single breath, letting us
compute per-breath signals.

We currently compute the following per-breath signals:

-   Measured PIP and PEEP are respectively the maximum and minimum
    patient pressure observed during the most recent breath.
-   Measured RR (in modes where RR is not mandated, i.e. in Pressure
    Assist mode) is the inverse average time-per-breath over the several
    most recent breaths.

These signals are naturally updated once per breath and are used both
for displaying respective readings in the Presentation layer and for
alarms.

#### Alarm subsystem

The UI controller is responsible for calculating and signaling all
alarms, with the exception of a planned “UI controller not responding”
alarm. The decision to place alarms into the UI controller, as opposed
to the Cycle controller, was intended to minimize the amount of code in
the cycle controller, the more safety-critical component of the system.

The alarm subsystem is based on ISO-60601-1-8 “General requirements,
tests and guidance for alarm systems in medical electrical equipment and
medical electrical systems”. The key concepts are alarm conditions --
potentially hazardous conditions requiring operator awareness or action
-- which can have different priority, and audio and visual alarm
signals.

The subsystem consists of several independent alarms (one alarm for each
condition) and an alarm manager that aggregates their signals, for
example determining which audio signal should sound when several alarms
are active.

Each alarm is notified on each new sensor reading (together with updated
per-breath signals) in order to update the state of its condition. Each
alarm can be queried by the Presentation layer for the current state of
its visual and audio signals.

The visual signal is non-latching and is active whenever the alarm
condition is currently active. It is wired up at the Presentation layer
to the related visual elements, e.g., the over/under pressure alarms
affect the color of the pressure graph.

The audio signal is latching and remains active when the alarm condition
was active in the past but was not yet acknowledged. Audio signals of
active alarms are aggregated by the Alarm manager in order to emit only
the highest-priority one.

The presentation layer provides a control to acknowledge the current
highest-priority active alarm, which silences the alarm’s audio signal
for up to 2 minutes or until its condition becomes inactive. It also
provides a control notifying the user of the existence of active alarms
that have been silenced, with a countdown until the end of silencing.

### User experience principles

The user experience and the design of the user interface follows
mandatory ISO norms for medical equipment, specifically lung
ventilators. These norms include, but are not limited to:

-   BS EN ISO 80601-2-12:2020 Medical electrical equipment
-   BS EN 62366-1:2015 Medical devices
-   BS ISO 19223:2019 Lung ventilators and related equipment -
    Vocabulary and semantics

The user interface was designed using these ISO norms as guidelines,
focusing on patient safety, with the following set of user experience
principles in mind:

#### Use safety

The GUI and user experience must be designed with error prevention front
and center, because every mistake made by an operator in a stressful and
life-threatening situation can have severe consequences for the patient.

Example: any parameter that can be changed by the operator and that has
direct impact on the ventilator performance must be confirmed before
being executed.

#### System usability

The GUI’s design must present an effective, efficient, and easy-to-use
solution that lets the operator complete their tasks in a successful,
confident and satisfying manner.

Example: the main screen serves as the hub of the application, from
which every functionality can be easily accessed and performed in a
linear fashion, ultimately leading back to the hub. This design prevents
the operator from getting lost within the application.

#### Workload

For every task available, the GUI must provide a solution that keeps the
operator’s mental demand, physical demand, temporal demand, effort and
frustration as low as possible.

Example: alarms can be silenced for a brief period of time to let the
operator better focus on the patient’s need.

#### Visibility

Critical information displayed in the user interface (i.e. values,
waveforms, alarm signals) must be perceivable at 4m distance and
perfectly legible at 1m.

Example: the high contrast of screen elements vs background color, the
font size and selected alarm colors support this requirement.

#### Touch Target Sizing

Interactive affordances such as buttons must meet the minimal
recommended touch target size of 1 x 1 cm to guarantee a large enough
surface for the operator to press. Touch target dimensions should also
be adapted to meet the needs of various situations.

Example: primary functions used in critical situations should have a
larger touch target than a secondary function.

#### Fitt’s law

The user experience must follow [Fitt’s
law](https://www.google.com/url?q=https://www.interaction-design.org/literature/topics/fitts-law&sa=D&ust=1598802820342000&usg=AOvVaw2xPiA5Q91wdMij8jqyT8Le) in
determining the position and dimension of interactive elements on the
UI, to ensure operators can perform primary tasks quickly.

Example:

#### Hick’s law

The user experience must follow [Hick’s
law](https://www.google.com/url?q=https://www.interaction-design.org/literature/article/hick-s-law-making-the-choice-easier-for-users&sa=D&ust=1598802820343000&usg=AOvVaw1PIeVNgXkrF5mFfc2Q0N_p) in
structuring content, information architecture, and user flows, to ensure
operators always have a clear path forward when performing a task.

Example: touch targets to change or set parameters should be larger than
the touch target for the menu in the top left corner, as accessing the
menu is a secondary operation compared to changing values to directly
support the patient.

#### Accessibility

The design must take into account accessibility guidelines that let
operators with disabilities use the system without constraints or risks
to the patient’s health.

Example: color values for alarm priorities were selected and tested to
accommodate the most common types of color blindness (deuteranopia,
protanopia, achromatopsia, etc.). Additionally,
different alarm priorities emit different sounds communicating their
degree of urgency per ISO-60601-1-8.
