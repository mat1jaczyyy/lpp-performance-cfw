# Launchpad Pro performance-optimized Custom Firmware

This repository contains the source code of my custom firmware for the Launchpad Pro. The firmware retains all* functionality from the stock firmware in addition to the all-new Performance mode. The modification is easy to install, free to use and works with every currently existing project or DAW.

* Yes, even Ableton Live functionality. Due to lack of external MIDI synthesizers, I am unable to test the MIDI DIN/DOUT ports and thus won't support them (for now).

## Building

The Launchpad Pro uses SysEx messages embedded into `.syx` files in order to update its firmware. I regularly post these builds of the firmware in the `#dev` channel on [my Discord server](https://discord.gg/5p3Bwnv), in case you are lazy to build the firmware yourself.

To build the firmware, set up the development environment according to dvhdr's [Launchpad Pro open-source firmware repository](https://github.com/dvhdr/launchpad-pro). After you are ready to build, replace the example source code (`app.c` file) with the source code hosted in this repository and build it using `$ make`. You will find the build at `build/launchpad_pro.syx`, ready for uploading.

## Installation

To install the firmware, simply send ("flash") the generated SysEx messages from the file to the Launchpad Pro while it is in bootloader mode (hold Setup while powering on). While it isn't required that you close down other programs that use the Launchpad while flashing, it is recommended to do so. There's a number of ways to flash:

* macOS only: [SysEx Librarian](https://www.snoize.com/SysExLibrarian/)
* Windows only: [MIDI-OX](http://www.midiox.com/moxdown.htm)
* Universal method: `CFW Flash Tool.maxpat`
    * To flash the firmware using my tool, simply load it up in Max (any version should work) while the Launchpad is in bootloader mode. 

## Usage
