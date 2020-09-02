Control Board (Printed Circuit Board)
=====================================

Due to the coupled nature of the controls, electrical, and mechanical components and their design the RespiraWorks
ventilator contains a custom fabricated printed circuit board (PCB) to connect the microcontroller, interface processor,
and sensor interfaces. The use of a custom PCB greatly reduces the chance for electrical assembly and fabrication errors
and is far more amenable to a quality fabrication process than hand-soldered components. We do not intend for this
component to be hand-assembled, and engineering turnkey fabrication instructions have been generated as part of the
submission; no assembly instructions are provided here.

![](images/image19.png)

The RespiraWorks Ventilator Mainboard PCB is a custom component that integrates the Cycle Controller, UI Computer,
sensors, actuator drives, power supplies, filtering, and protection components. The PCB is designed by RespiraWorks and
is open-source, with a dedicated repository available at the [pcb directory](../../pcb).

The complete turn-key manufacturing files for Rev 1.0 have been included as part of this submission package. This
package includes PCB artwork files, NC drill data,  fabrication and assembly drawings, Bill of Materials, and
machine-assembly centroid (pick-and-place) files.  Any standard PCB fabrication and assembly vendor will be able to
manufacture these boards using the file package available at the referenced
[link](../../pcb/rev1_export/20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-PKG-TURNKEY.zip), or submitted as a zip
as part of this application (01-08B RespiraWorks PCB (Manufacturing).zip). The design was created in Altium, and the
full design package for Rev 1.0 is submitted as a part of this application (01-08B RespiraWorks PCB (Manufacturing).zip)
be found at the referenced [link](../../pcb/rev1_export/20200424v2-RespiraWorks-Ventilator-Rev1.0-RC2-PKG-DESIGN.zip).

A detailed guide to setup of the PCB is included as part of the assembly instructions
(03-02 Respiraworks Assembly Instructions.pdf) and is available at the [pcb directory](../../pcb).
