# Launchpad Pro performance-optimized Custom Firmware

This repository contains the source code of my custom firmware for the Launchpad Pro. The firmware retains all functionality (even Ableton Live integration) from the stock firmware in addition to the all-new Performance mode. It brings considerably faster LED updating speed, especially when using Apollo Studio. The modification is easy to install, free to use and works with every currently existing project or DAW.

## Installation

The simplest and easiest way to install the firmware is via the [Launchpad Utility](https://fw.mat1jaczyyy.com) with the `Launchpad Pro (CFW)` option selected. While it isn't required that you close down other programs that use the Launchpad while installing, it is recommended to do so.

To install the firmware offline, simply send ("flash") the generated SysEx messages from the file to the Launchpad Pro while it is in Bootloader mode (hold Setup while powering on). There's a number of ways to flash:

* macOS only: [SysEx Librarian](https://www.snoize.com/SysExLibrarian/)
* Windows only: [MIDI-OX](http://www.midiox.com/moxdown.htm)

## Usage

All modes other than the new Performance mode behave much the same as they do on stock firmware. Included is a new boot animation, which can be skipped by pressing the Setup button while it is displaying. For Ableton Live integration (Live mode), set your Launchpad Pro up the same way you would set up a normal one, but use the MIDI Port instead of the Live Port (doesn't exist in open-source firmware). If you want to use any of the Standalone modes (including the new Performance mode), all you need to do is set the track inputs/outputs to the Standalone Port and the appropriate channel for the mode (listed shortly after). Note that on Windows, the Standalone Port displays itself as "Launchpad Open" or "x- Launchpad Open" and the MIDI Port displays itself as "Launchpad Open (Port 2)" or "MIDIIN2 (x- Launchpad Open)".

### Setup mode

You no longer have to hold the Setup button in order to change the settings on your Launchpad. A simple button press takes you in and out of the Setup mode with ease. If you're used to the old way of holding the button while changing settings, the Launchpad will recognise it and take you back when you release the button.

* Top left corner: Mode selection
    * Red = Performance mode (Standalone Port - Channel 16, use 12 for flashing and 13 for pulsing)
    * Green = Live mode (MIDI Port)
    * Cyan = Note mode (Standalone Port)
    * Yellow = Drum mode (Standalone Port)
    * Pink = Fader mode (Standalone Port)
    * Orange = Programmer mode (Standalone Port - use Channel 1 for FL Studio support!)
    * Violet = Piano mode (Standalone Port)
    * White = Custom modes (Standalone Port)
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
    
Additional options will appear in the bottom-right corner if the Custom modes are selected, in left-to-right order:
* Internal LED feedback toggles whether pads on the 8x8 grid light up when pressed.
* External LED feedback toggles whether pads on the 8x8 grid light up when MIDI is received externally.
* Fader Velocity toggles whether faders will ignore velocity sensitivity, even if it's turned on globally.

In modes found on the stock firmware, the channel selection will appear in the bottom-right corner. Note mode additionally supports translation (yellow key next to root note selector in Scale setup), which will light the grid in the selected root key even when not using Scale mode.

### Performance mode

The new Performance mode is designed to eliminate the need for Top Lights plugins and Retina 2.1 on the host machine. Top Lights are made directly available via normal Note On messages (according to the Top Lights configuration), and the velocities received are displayed using the currently selected palette. This also allows for placing samples on the top row. 

As an additional touch, if you're using [Outbreak's latest Multi](https://github.com/mat1jaczyyy/outbreak/tree/master/Multi), entering the Performance mode will reset all multisamples in your Live set. This feature does not necessarily require a Multi Reset device to reside somewhere in the chain, but relies on any devices in between to properly pass CC messages through.

### Palette editor

The Palette editor shows a preview of the selected palette in left-to-right, top-to-bottom order. The top row displays the currently selected velocity in its 7-bit binary form. You can select a velocity either by flipping the bits to reach a desired number, or by directly selecting a velocity from the preview (note that you will only be able to access half the velocities in this way, in order to access the other half you must flip the most significant bit of the velocity number).

The left column displays the amount of red, the bottom row displays the amount of green, and the right column displays the amount of blue of the currently selected velocity in their 6-bit binary forms. To modify the color, flip the bits of the color channels to reach a desired number. For the vertical values (red and blue), the top-most bit is most significant and the bottom-most bit is least significant.

To save and exit, press the Setup button.

You can also use the Palette Utility as part of the [Launchpad Utility](https://fw.mat1jaczyyy.com) to upload a Retina palette into one of the palette slots on the Launchpad. Simply import your Retina palette and select the desired Palette Index. Then, click Upload.

To download a palette from the Launchpad to the computer, press the Up key on your Launchpad to send the palette to the Palette Utility, where you can then export it into a file.

### Piano mode

The new Piano mode is similar to the Note mode, but employs a classical piano-like rows per octave layout. Note that it doesn't support Scale mode due to the vastly different layout. The navigation buttons change octaves and transpose the keys.

### Custom modes

The Custom modes turn the Launchpad's 8x8 grid into a highly customizable surface. Based on the custom mode framework seen on the Launchpad X, Mini MK3 and Pro MK3; this mode lets you run custom modes created in the [Novation Components app](https://components.novationmusic.com/) natively on your Launchpad Pro, even stored across sessions.

To load a custom mode from the Components app, first download it as a `*.syx` file to your computer. Then, import it into the [Launchpad Utility](https://fw.mat1jaczyyy.com/modes), select a slot to upload to and click Upload. You can also use this tool to export any custom modes from your Launchpad Pro back into a `*.syx` file, which can then be reloaded into Components for further editing. This is done by pressing the Up key on your Launchpad while in a custom mode to send that mode to the Launchpad Utility, similarly to how it's done for custom palettes.

The right hand side of the Launchpad allows for switching between 8 custom mode slots available on the firmware. If a slot lights up red, it's empty and unavailable. If it lights up green, the mode exists and is loaded properly. To delete existing modes, hold down the slot you want to delete and press the Delete key on the left hand side of the Launchpad 3 times.

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

### Linux

Install the GCC ARM toolchain through your package manager.
You'll also need a container runtime such as `podman` or `docker` to build part of the tooling.

Compile with:

```
make
```

#### Flashing

The MIDI SysEx utility, `amidi`, comes preinstalled on most Linux distributions
as a part of ALSA. You can use this to flash your Launchpad.

Start your Launchpad in bootloader mode, and in a terminal, enter `amidi -l`. 
Pay attention to the device column. These are the ports of your MIDI devices.

```
% amidi -l
Dir Device    Name
IO  hw:2,0,0  ...
IO  hw:3,0,0  ...
IO  hw:4,0,0  Launchpad Pro Launchpad Pro
IO  hw:5,0,0  ...
```

You should see your Launchpad listed. In my case the port number is `hw:4,0,0`.
Send the firmware to your Launchpad with `amidi -p [port] -s build/cfw.syx`.

```
amidi -p hw:4,0,0 -s build/cfw.syx
```

If you run into issues with the flashing being interrupted, 
try adding a few milliseconds of interval to `amidi` with the `-i ms` option.

```
amidi -p hw:4,0,0 -s build/cfw.syx -i 2
```

## Issues

Some Setup parameters found on the stock firmware are not implemented in order to simplify the firmware, including:

* Pad Lighting setting for stock Standalone modes

For any other issues or new feature requests, submit an issue report.

If you don't like the firmware and want to roll back, you can use [Novation's Firmware Updater](https://customer.novationmusic.com/support/downloads?brand=Novation&product_by_range=527&download_type=all) or the [Launchpad Utility](https://fw.mat1jaczyyy.com) with the `Launchpad Pro` option selected to roll back to the stock firmware.
