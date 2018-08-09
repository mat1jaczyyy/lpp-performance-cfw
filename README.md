# Launchpad Pro performance-optimized Custom Firmware

This repository contains the source code of my custom firmware for the Launchpad Pro. The firmware retains almost all functionality (even Ableton Live integration) from the stock firmware in addition to the all-new Performance mode. The modification is easy to install, free to use and works with every currently existing project or DAW.

## Building

The Launchpad Pro uses SysEx messages embedded into `.syx` files in order to update its firmware. I regularly post these builds of the firmware in the `#dev` channel on [my Discord server](https://discord.gg/5p3Bwnv), in case you are lazy to build the firmware yourself.

To build the firmware, set up the development environment according to dvhdr's [Launchpad Pro open-source firmware repository](https://github.com/dvhdr/launchpad-pro). After you are ready to build, replace the example source code (`app.c` file) with the source code hosted in this repository and build it using `$ make`. You will find the build at `build/launchpad_pro.syx`, ready for uploading.

## Installation

To install the firmware, simply send ("flash") the generated SysEx messages from the file to the Launchpad Pro while it is in Bootloader mode (hold Setup while powering on). While it isn't required that you close down other programs that use the Launchpad while flashing, it is recommended to do so. There's a number of ways to flash:

* macOS only: [SysEx Librarian](https://www.snoize.com/SysExLibrarian/)
* Windows only: [MIDI-OX](http://www.midiox.com/moxdown.htm)
* Universal method: `CFW Flash Tool.maxpat`
    * To flash the firmware using my tool, simply load it up in Max (any version should work). Boot the Launchpad into Bootloader mode, click the Upload button and select the firmware SysEx file you want to upload.

## Usage

All modes other than the new Performance mode behave much the same as they do on stock firmware. Included is a new boot animation, which can be skipped by pressing the Setup button while it is displaying. For Ableton Live integration (Live mode), set your Launchpad Pro up the same way you would set up a normal one, but use the MIDI Port instead of the Live Port (doesn't exist in open-source firmware). If you want to use any of the Standalone modes (including the new Performance mode), all you need to do is set the track inputs/outputs to the Standalone Port and the appropriate channel for the mode (listed shortly after). Note that on Windows, the Standalone Port displays itself as "Launchpad Open" and the MIDI Port displays itself as "Launchpad Open (Port 2)".

### Setup mode

You no longer have to hold the Setup button in order to change the settings on your Launchpad. A simple button press takes you in and out of the Setup mode with ease. If you're used to the old way of holding the button while changing settings, the Launchpad will recognise it and take you back when you release the button.

* Top left corner: Mode selection
    * Red = Performance mode (Standalone Port - Channel 16)
    * Green = Live mode (MIDI Port - User mode is locked to Channel 6)
    * Cyan = Note mode (Standalone Port - Channel 1)
    * Yellow = Drum mode (Standalone Port - Channel 2)
    * Pink = Fader mode (Standalone Port - Channel 3)
    * Orange = Programmer mode (Standalone Port - Channel 1 - this is required for FL Studio support) 
* Bottom left corner: Velocity sensitivity toggle
    * Bright = Enabled
    * Dim = Disabled

Additional options will appear if the Performance mode is selected:
* Top-right corner: Top Lights configuration, in left-to-right order:
    * Pro = the Launchpad reacts to Top Lights normally
    * MK2 = the Launchpad reacts to Top Lights as if it was a Launchpad MK2
    * MK2 Rotated = similar to MK2 configuration, but additionally rotates the side lights by 180 degrees to display them on the bottom and left side
    * MK2 Mirrored = similar to MK2 configuration, but additionally mirrors the side lights along a diagonal descending axis to display them on the bottom and left side
* Bottom-right corner: Palette selection
    * The top row selects the 3 custom palettes stored on the Launchpad's flash memory. If one of those palettes is selected, a rainbow edit button will be displayed on the left side. Alongside editing directly on the Launchpad, they can also be uploaded to the Launchpad in the Retina palette file format.
    * The bottom row selects 3 built-in preset palettes, in left-to-right order: Novation's original palette, mat1jaczyyy's custom palette, and the Launchpad S legacy palette.

### Performance mode

The new Performance mode is designed to eliminate the need for Top Lights plugins and Retina 2.1 on the host machine. Top Lights are made directly available via normal Note On messages (according to the Top Lights configuration), and the velocities received are displayed using the currently selected palette. This also allows for placing samples on the top row.

### Palette editor

The Palette editor shows a preview of the selected palette in left-to-right, top-to-bottom order. The top row displays the currently selected velocity in its 7-bit binary form. You can select a velocity either by flipping the bits to reach a desired number, or by directly selecting a velocity from the preview (note that you will only be able to access half the velocities in this way, in order to access the other half you must flip the most significant bit of the velocity number).

The left column displays the amount of red, the bottom row displays the amount of green, and the right column displays the amount of blue of the currently selected velocity in their 6-bit binary forms. To modify the color, flip the bits of the color channels to reach a desired number. For the vertical values (red and blue), the top-most bit is most significant and the bottom-most bit is least significant.

To save and exit, press the Setup button.

You can also use the palette uploader `CFW Palette Uploader.maxpat` to upload a Retina palette into one of the palette slots on the Launchpad. Simply load it up in Max (any version should work), select your Launchpad's Standalone Port and the desired Palette Index. Then, click Upload and select your Retina palette file.

## Issues

The firmware currently lacks a very small amount of features from the stock firmware, which could be implemented if there is a demand. If you want to add the following features to the firmware yourself, feel free to fork the repository and submit a pull request when you're done.
* Due to lack of external MIDI synthesizers, I am unable to test the MIDI DIN/DOUT ports and thus won't support them (for now).
* Aftertouch
* Some Setup parameters found on the stock firmware are not implemented in order to simplify the firmware, including:
    * Channel selection for all modes
    * Pad Lighting setting for stock Standalone modes
    * Settings for unimplemented features (aftertouch, MIDI DIN/DOUT ports)

Another known issue is that Novation's USB Driver does not support units with customized firmware. For this reason, apps which utilize the driver's multi-client features (such as Retina) will not function with this firmware. To work around that particular limitation, use the built-in palette and Top Lights features of the Performance mode. This issue does not exist on macOS due to native support for multi-client MIDI output which doesn't require a special driver.

For any other issues or new feature requests, submit an issue report.

If you don't like the firmware and want to roll back, you can use [Novation's Firmware Updater](https://global.novationmusic.com/support/product-downloads?product=Launchpad+Pro) to roll back to the stock firmware.
