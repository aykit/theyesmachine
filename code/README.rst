Build
-----

- Download `GNU Tools for ARM Embedded Processors`_, extract it and add it to your `PATH`.
- Download `Atmel Software Framework`_ (`asf-standalone-archive-3.16.0.88.zip`), extract it to `lib` and rename it to `asf`.
- Download `Cortex Microcontroller Software Interface Standard`_ (`CMSIS-SP-00300-r4p0-00rel0.zip`), extract it to `lib` and rename it to `cmsis`.
- Install `scons`_.
- Run `scons`.

.. _GNU Tools for ARM Embedded Processors: https://launchpad.net/gcc-arm-embedded
.. _Atmel Software Framework: http://www.atmel.at/tools/avrsoftwareframework.aspx?tab=overview
.. _Cortex Microcontroller Software Interface Standard: http://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php
.. _BOSSA: http://www.shumatech.com/web/products/bossa
.. _scons: http://www.scons.org/


Remove unused code
^^^^^^^^^^^^^^^^^^

To simplify browsing the code.

Linux::

	find lib/asf -regextype posix-extended -type d -regex '.*/(at)?(sam[4dg]|sam3[nux]|x?mega).*' -exec rm -r {} \;
	find lib/asf -regextype posix-extended -type d -regex '.*/(avr|at(32)?uc|sam0|unit_tests).*' -exec rm -r {} \;

BSD::

	find -E lib/asf -type d -regex '.*/(at)?(sam[4dg]|sam3[nux]|x?mega).*' -exec rm -r {} \;
	find -E lib/asf -type d -regex '.*/(avr|at(32)?uc|sam0|unit_tests).*' -exec rm -r {} \;


Flash
-----

USB
^^^

Install `BOSSA`_.

::

	scons usbflash


JTAG
^^^^

::

	scons flash


Debugging
---------

- Install `OpenOCD`_ 
- Install `GDB`_ 
.. _OpenOCD: http://openocd.sourceforge.net
.. _GDB: https://www.sourceware.org/gdb/


OpenOCD
^^^^^^^

Start OpenOCD::

	openocd -f board/olimex-sam3.cfg -s openocd/


gdb
^^^

http://openocd.sourceforge.net/doc/html/GDB-and-OpenOCD.html

Start::

	arm-none-eabi-gdb program.elf

	target remote localhost:3333
	set remote hardware-breakpoint-limit 6
	set remote hardware-watchpoint-limit 4

Flash::

	load

Reset::

	monitor reset

Reset and halt::

	monitor reset halt


Eclipse
^^^^^^^

- Install `Eclipse`_ 
.. _Eclipse: http://eclipse.org

Checkout `Pixhawk`_ for how to use Eclipse with GDB

.. _Pixhawk: http://http://pixhawk.org/dev/jtag/gdb

::

	Help → Install New Software → C/C++ GDB Hardware Debugging
	File → New → Project → C/C++/Makefile Project with Existing Code
	Run → Debug Configurations → GDB OpenOCD Debugging
