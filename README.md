# Launchpad Pro performance-optimized Custom Firmware

This repository contains the source code of my custom firmware for the Launchpad Pro. The firmware retains all functionality (even Ableton Live integration) from the stock firmware in addition to the all-new Performance mode. The modification is easy to install, free to use and works with every currently existing project or DAW.

## Installation

The simplest and easiest way to install the firmware is via the [Launchpad Firmware Utility](https://fw.mat1jaczyyy.com) with the `Launchpad Pro (CFW)` option selected. While it isn't required that you close down other programs that use the Launchpad while installing, it is recommended to do so.

To install the firmware offline, simply send ("flash") the generated SysEx messages from the file to the Launchpad Pro while it is in Bootloader mode (hold Setup while powering on). There's a number of ways to flash:

* macOS only: [SysEx Librarian](https://www.snoize.com/SysExLibrarian/)
* Windows only: [MIDI-OX](http://www.midiox.com/moxdown.htm)

## Usage

All modes other than the new Performance mode behave much the same as they do on stock firmware. Included is a new boot animation, which can be skipped by pressing the Setup button while it is displaying. For Ableton Live integration (Live mode), set your Launchpad Pro up the same way you would set up a normal one, but use the MIDI Port instead of the Live Port (doesn't exist in open-source firmware). If you want to use any of the Standalone modes (including the new Performance mode), all you need to do is set the track inputs/outputs to the Standalone Port and the appropriate channel for the mode (listed shortly after). Note that on Windows, the Standalone Port displays itself as "Launchpad Open" or "x- Launchpad Open" and the MIDI Port displays itself as "Launchpad Open (Port 2)" or "MIDIIN2 (x- Launchpad Open)".

### Setup mode

You no longer have to hold the Setup button in order to change the settings on your Launchpad. A simple button press takes you in and out of the Setup mode with ease. If you're used to the old way of holding the button while changing settings, the Launchpad will recognise it and take you back when you release the button.

* Top left corner: Mode selection
    * Red = Performance mode (Standalone Port - Channel 16)
    * Green = Live mode (MIDI Port - User mode is locked to Channel 6)
    * Cyan = Note mode (Standalone Port - Channel 1)
    * Yellow = Drum mode (Standalone Port - Channel 2)
    * Pink = Fader mode (Standalone Port - Channel 3)
    * Orange = Programmer mode (Standalone Port - Channel 1 - this is required for FL Studio support)
    * Violet = Piano mode (Standalone Port - Channel 4)
* Left side: Idle animation toggle
* Bottom left corner: Input/Output Management
    * Cyan = Velocity sensitivity toggle
    * Orange = Direct standalone MIDI out toggle (3.5mm port)
    * Green = Aftertouch settings
        * In left-to-right order: Disabled, Channel Pressure enabled, Polyphonic Aftertouch enabled

Additional options will appear if the Performance mode is selected:
* Top-right corner: Layout configuration, in left-to-right order:
    * Pro = the Launchpad reacts to Top Lights normally
    * MK2 = the Launchpad reacts to Top Lights as if it was a Launchpad MK2
    * MK2 Rotated = similar to MK2 configuration, but additionally rotates the side lights by 180 degrees to display them on the bottom and left side
    * MK2 Mirrored = similar to MK2 configuration, but additionally mirrors the side lights along a diagonal descending axis to display them on the bottom and left side
    * Pink LED = Layout toggle
        * Bright = XY Layout
        * Dim = Drum Rack Layout
* Bottom-right corner: Palette selection
    * The top row selects the 3 custom palettes stored on the Launchpad's flash memory. If one of those palettes is selected, a rainbow edit button will be displayed on the left side. Alongside editing directly on the Launchpad, they can also be uploaded to the Launchpad in the Retina palette file format.
    * The bottom row selects 3 built-in preset palettes, in left-to-right order: Novation's original palette, mat1jaczyyy's custom palette, and the Launchpad S legacy palette.

### Performance mode

The new Performance mode is designed to eliminate the need for Top Lights plugins and Retina 2.1 on the host machine. Top Lights are made directly available via normal Note On messages (according to the Top Lights configuration), and the velocities received are displayed using the currently selected palette. This also allows for placing samples on the top row. 

As an additional touch, if you're using [Outbreak's latest Multi](https://github.com/mat1jaczyyy/outbreak/tree/master/Multi), entering the Performance mode will reset all multisamples in your Live set. This feature does not necessarily require a Multi Reset device to reside somewhere in the chain, but relies on any devices in between to properly pass CC messages through.

### Palette editor

The Palette editor shows a preview of the selected palette in left-to-right, top-to-bottom order. The top row displays the currently selected velocity in its 7-bit binary form. You can select a velocity either by flipping the bits to reach a desired number, or by directly selecting a velocity from the preview (note that you will only be able to access half the velocities in this way, in order to access the other half you must flip the most significant bit of the velocity number).

The left column displays the amount of red, the bottom row displays the amount of green, and the right column displays the amount of blue of the currently selected velocity in their 6-bit binary forms. To modify the color, flip the bits of the color channels to reach a desired number. For the vertical values (red and blue), the top-most bit is most significant and the bottom-most bit is least significant.

To save and exit, press the Setup button.

You can also use the palette uploader found at `max/CFW Palette Uploader.maxpat` to upload a Retina palette into one of the palette slots on the Launchpad. Simply load it up in Max (any version should work) and select your Launchpad's Standalone Port and the desired Palette Index. Then, click Upload and select your Retina palette file.

To download a palette from the Launchpad to the computer, load `max/CFW Palette Downloader.maxpat` in Max (probably requires 8, older versions were not tested) and select your Launchpad's Standalone Port. Then, press Up on your Launchpad to send the palette.

### Piano mode

The new Piano mode is similar to the Note mode, but employs a classical piano-like rows per octave layout. Note that it doesn't support Scale mode due to the vastly different layout. The navigation buttons change octaves and transpose the keys.

## Building

The Launchpad Pro uses SysEx messages embedded into `.syx` files in order to update its firmware. In case you are lazy to build the firmware yourself, you can find the latest firmware build in this repository at `build/cfw.syx`. I also regularly post those builds of the firmware in the `#dev` channel on [my Discord server](https://discord.gg/5p3Bwnv).

### Windows

First install make (preferably using [Chocolatey](https://chocolatey.org/install)):

```
choco install make
```

Next, install the [GCC ARM toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads). Make sure your PATH contains `arm-none-eabi-gcc\bin` (the installer can do this for you) and you can invoke `arm-none-eabi-gcc.exe` from the command line. Then, to directly compile the code:

```
make
```

### macOS

On macOS you can easily install the GCC ARM toolchain using the [Homebrew package manager](http://brew.sh). The EABI tools are maintained in an external repository which you need to put on tap first.

```
brew tap PX4/homebrew-px4
brew install gcc-arm-none-eabi
```

Then, to directly compile the code:

```
make
```

## Issues

Some Setup parameters found on the stock firmware are not implemented in order to simplify the firmware, including:
    
* Channel selection for all modes
* Pad Lighting setting for stock Standalone modes

For any other issues or new feature requests, submit an issue report.

If you don't like the firmware and want to roll back, you can use [Novation's Firmware Updater](https://customer.novationmusic.com/support/downloads?brand=Novation&product_by_range=527&download_type=all) or the [Launchpad Firmware Utility](https://fw.mat1jaczyyy.com) with the `Launchpad Pro` option selected to roll back to the stock firmware.
