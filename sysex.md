# SysEx message list

All SysEx messages start with the standard MIDI SysEx Start code, and end with the standard MIDI SysEx End code. This is the bare minimum required to represent a SysEx message and for any device to interpret it as such.

`F0 ... F7`<sub><sub><sup>(16)</sup></sub></sub><br>
`240 ... 247`<sub><sub><sup>(10)</sup></sub></sub>

Within representations of SysEx messages in this document:

- `<variable>` is a single-byte arbitrary value at the discretion of the sender.
- `<variable>...` is a variable-length list of single-byte arbitrary values at the discretion of the sender.
- `<variable(n)>` is a *n*-byte arbitrary value.
- `<<variable1> <variable2>>` is a group of variables.
- `<<variable1> <variable2>>...` is a variable-length list of groups of variables.

Common variables used in SysEx messages in this document include:

- `<XY>` refers to the X and Y coordinates of an LED, where X is the row and Y is the column. The value `63`<sub><sub><sup>(16)</sup></sub></sub> `99`<sub><sub><sup>(10)</sup></sub></sub> is used to represent the side LED on the bottom edge of the unit (colloquially refered to as the Mode Light).

- `<color>` refers to the color of an LED as if controlled via regular MIDI note velocity. The stock Novation palette is always used to represent the color, except in Performance mode where the user-selected palette is used instead.

- `<R>`, `<G>`, and `<B>` are the 6-bit red, green, and blue color components, respectively. The value `0`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> represents the minimum intensity, and the value `3F`<sub><sub><sup>(16)</sup></sub></sub> `63`<sub><sub><sup>(10)</sup></sub></sub> represents the maximum intensity.

Assume the custom firmware will not respond to any SysEx message, documented here or unknown, unless stated otherwise.

The largest SysEx message that can be received or sent is 320 bytes, including the SysEx Start and End codes.

## Standard Identification messages

### Identity Request (Device Inquiry)

The Identity Request is an universal SysEx message that is used to determine what kind of device is behind a MIDI port.

Request:

`7E 7F 06 01 F7`<sub><sub><sup>(16)</sup></sub></sub><br>
`126 127 6 1 247`<sub><sub><sup>(10)</sup></sub></sub>

Response:

`7E 00 06 02 00 20 29 <model> 00 00 00 00 <fw_version(3)>`<sub><sub><sup>(16)</sup></sub></sub><br>
`126 0 6 2 0 32 41 <model> 0 0 0 0 <fw_version(3)>`<sub><sub><sup>(10)</sup></sub></sub>

- `<model>` is the model of Launchpad. This value will always be `51`<sub><sub><sup>(16)</sup></sub></sub> `81`<sub><sub><sup>(10)</sup></sub></sub> for the Launchpad Pro.
- `<fw_version(3)>` is the firmware version number.

### Version Inquiry

The Version Inquiry is a Novation-specific SysEx message that is used to get more information about the Launchpad.

Request:

`00 20 29 00 70`<sub><sub><sup>(16)</sup></sub></sub><br>
`0 32 41 0 112`<sub><sub><sup>(10)</sup></sub></sub>

Response:

`00 20 29 00 70 00 00 <bl_version(3)> 00 00 <fw_version(3)> <bl_size> 01`<sub><sub><sup>(16)</sup></sub></sub><br>
`0 32 41 0 112 0 0 <bl_version(3)> 0 0 <fw_version(3)> <bl_size> 1`<sub><sub><sup>(10)</sup></sub></sub>

- `<bl_version(3)>` is the bootloader version number.
- `<fw_version(3)>` is the firmware version number.
- `<bl_size>` is the size of the bootloader region in kilobytes.

### Reboot to bootloader

Reboots the Launchpad into the bootloader.

`00 20 29 00 71 00 51`<sub><sub><sup>(16)</sup></sub></sub><br>
`0 32 41 0 113 0 81`<sub><sub><sup>(10)</sup></sub></sub>

*Currently not implemented in the custom firmware (planned)*

## Supported stock firmware Novation messages

All stock firmware Novation request and response messages begin with the same standard header:

`00 20 29 02 10`<sub><sub><sup>(16)</sup></sub></sub><br>
`0 32 41 2 16`<sub><sub><sup>(10)</sup></sub></sub>

### Mode selection

Request:

`21 <mode>`<sub><sub><sup>(16)</sup></sub></sub><br>
`33 <mode>`<sub><sub><sup>(10)</sup></sub></sub>

`<mode>` can be:
- `00`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> for Ableton Live mode,
- `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> for Standalone mode.

Response if the mode was changed successfully:

`2D <mode>`<sub><sub><sup>(16)</sup></sub></sub><br>
`45 <mode>`<sub><sub><sup>(10)</sup></sub></sub>

### Standalone layout selection

Request:

`2C <layout>`<sub><sub><sup>(16)</sup></sub></sub><br>
`44 <layout>`<sub><sub><sup>(10)</sup></sub></sub>

`<layout>` can be:
- `00`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> for Note mode,
- `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> for Drum mode,
- `02`<sub><sub><sup>(16)</sup></sub></sub> `2`<sub><sub><sup>(10)</sup></sub></sub> for Fader mode,
- `03`<sub><sub><sup>(16)</sup></sub></sub> `3`<sub><sub><sup>(10)</sup></sub></sub> for Programmer mode,
- `04`<sub><sub><sup>(16)</sup></sub></sub> `4`<sub><sub><sup>(10)</sup></sub></sub> for Performance mode,
- `05`<sub><sub><sup>(16)</sup></sub></sub> `5`<sub><sub><sup>(10)</sup></sub></sub> for Piano mode,
- `06`<sub><sub><sup>(16)</sup></sub></sub> `6`<sub><sub><sup>(10)</sup></sub></sub> for Chord mode,
- `07`<sub><sub><sup>(16)</sup></sub></sub> `7`<sub><sub><sup>(10)</sup></sub></sub> through `09`<sub><sub><sup>(16)</sup></sub></sub> `9`<sub><sub><sup>(10)</sup></sub></sub> are reserved for future use,
- `0A`<sub><sub><sup>(16)</sup></sub></sub> `10`<sub><sub><sup>(10)</sup></sub></sub> through `11`<sub><sub><sup>(16)</sup></sub></sub> `17`<sub><sub><sup>(10)</sup></sub></sub> for Custom modes 1 through 8.

Response if the layout was changed successfully:

`2F <layout>`<sub><sub><sup>(16)</sup></sub></sub><br>
`47 <layout>`<sub><sub><sup>(10)</sup></sub></sub>

### Ableton Live Control Surface Challenge

Request:

`40 <challenge(4)>`<sub><sub><sup>(16)</sup></sub></sub><br>
`64 <challenge(4)>`<sub><sub><sup>(10)</sup></sub></sub>

Ableton Live's Control Surface challenges the Launchpad Pro to prove it is a genuine Novation device. This is done to prevent counterfeit/unauthorized devices from interfacing with Ableton Live.

As this algorithm is implemented in the Launchpad Pro's bootloader, the custom firmware does not reimplement it and instead calls the function in the bootloader to generate the response.

Response:

`40 <response(2)>`<sub><sub><sup>(16)</sup></sub></sub><br>
`64 <response(2)>`<sub><sub><sup>(10)</sup></sub></sub>

The Launchpad Pro responds with the correct response to this challenge in order to be recognized by Ableton Live and in order for Control Surface functionality to be enabled.

### Ableton Live Quit Message

`40`<sub><sub><sup>(16)</sup></sub></sub><br>
`64`<sub><sub><sup>(10)</sup></sub></sub>

Changes the default mode to Performance mode.

### Ableton Live Layout selection

Request:

`22 <layout>`<sub><sub><sup>(16)</sup></sub></sub><br>
`34 <layout>`<sub><sub><sup>(10)</sup></sub></sub>

`<layout>` can be:
- `00`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> for Session mode,
- `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> for Drum Rack mode,
- `02`<sub><sub><sup>(16)</sup></sub></sub> `2`<sub><sub><sup>(10)</sup></sub></sub> for Chromatic Note mode,
- `03`<sub><sub><sup>(16)</sup></sub></sub> `3`<sub><sub><sup>(10)</sup></sub></sub> for User (Drum) mode,
- `04`<sub><sub><sup>(16)</sup></sub></sub> `4`<sub><sub><sup>(10)</sup></sub></sub> for Audio (Blank) mode,
- `05`<sub><sub><sup>(16)</sup></sub></sub> `5`<sub><sub><sup>(10)</sup></sub></sub> for Fader mode,
- `06`<sub><sub><sup>(16)</sup></sub></sub> `6`<sub><sub><sup>(10)</sup></sub></sub> for Record Arm (Session) mode,
- `07`<sub><sub><sup>(16)</sup></sub></sub> `7`<sub><sub><sup>(10)</sup></sub></sub> for Track Select (Session) mode,
- `08`<sub><sub><sup>(16)</sup></sub></sub> `8`<sub><sub><sup>(10)</sup></sub></sub> for Mute (Session) mode,
- `09`<sub><sub><sup>(16)</sup></sub></sub> `9`<sub><sub><sup>(10)</sup></sub></sub> for Solo (Session) mode,
- `0A`<sub><sub><sup>(16)</sup></sub></sub> `10`<sub><sub><sup>(10)</sup></sub></sub> for Volume (Fader) mode,
- `0B`<sub><sub><sup>(16)</sup></sub></sub> `11`<sub><sub><sup>(10)</sup></sub></sub> for Pan (Fader) mode,
- `0C`<sub><sub><sup>(16)</sup></sub></sub> `12`<sub><sub><sup>(10)</sup></sub></sub> for Sends (Fader) mode,
- `0D`<sub><sub><sup>(16)</sup></sub></sub> `13`<sub><sub><sup>(10)</sup></sub></sub> for Stop Clip (Session) mode.

Response if the layout was changed successfully:

`2E <layout>`<sub><sub><sup>(16)</sup></sub></sub><br>
`46 <layout>`<sub><sub><sup>(10)</sup></sub></sub>

### Fader Setup

This message can only be called while in Standalone Fader mode, or one of the four available Ableton Live Fader modes.

`2B <<N> <type> <color> <initial>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`43 <<N> <type> <color> <initial>>...`<sub><sub><sup>(10)</sup></sub></sub>

- `<N>` is the number of fader to be configured, ranging from 0 to 7.
- `<type>` can be:
    - `00`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> for a volume fader,
    - `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> for a pan fader.
- `<color>` is the color of the fader.
- `<initial>` is the initial value of the fader.

### Light one or more LEDs

Overrides the color of the specific LED.

`0A <<XY> <color>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`10 <<XY> <color>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Flash one or more LEDs

The Launchpad supports flashing between its current color and the flash color. Flashing happens in time with the MIDI beat clock.

Overrides the flash color of the specific LED.

`23 <<XY> <color>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`35 <<XY> <color>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Pulse one or more LEDs

The Launchpad supports rhythmically pulsing a LED between 100% brightness and 25% brightness of a color. Pulsing happens in time with the MIDI beat clock.

Overrides the pulse color of the specific LED.

`28 <<XY> <color>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`40 <<XY> <color>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Light a column of LEDs

`0C <Y> <color>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`12 <Y> <color>...`<sub><sub><sup>(10)</sup></sub></sub>

`<Y>` is the column number, ranging from 0 to 9.

### Light a row of LEDs

`0D <X> <color>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`13 <X> <color>...`<sub><sub><sup>(10)</sup></sub></sub>

`<X>` is the row number, ranging from 0 to 9.

### Light all LEDs

`0E <color>`<sub><sub><sup>(16)</sup></sub></sub><br>
`14 <color>`<sub><sub><sup>(10)</sup></sub></sub>

### Light one or more LEDs (RGB)

The brightness of the individual red, green and blue color channels of the LED can be controlled to create any color.

`0B <<XY> <R> <G> <B>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`11 <<XY> <R> <G> <B>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Light a grid of LEDs (RGB)

A more efficient solution to updating the entire display, or just the central square pad core in a single message.

`0F <type> <<R> <G> <B>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`15 <type> <<R> <G> <B>>...`<sub><sub><sup>(10)</sup></sub></sub>

- `<type>` can be:
    - `00`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> for the entire 10 by 10 grid,
    - `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> for the square 8 by 8 grid.
- LEDs are assumed in sequence starting from the bottom-most left LED and the `<R> <G> <B>` values are repeated for each LED.

### Text Scrolling

Switches the Launchpad to a special text scrolling mode, like an intermission.

Request:

`14 <color> <loop> <char>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`20 <color> <loop> <char>...`<sub><sub><sup>(10)</sup></sub></sub>

- `<color>` is the color of the text.
- `<loop>` can be:
    - `00`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> to scroll the text once,
    - `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> to loop the text until the user presses any button.
- `<char>...` is the string to be displayed.
    - `20`<sub><sub><sup>(16)</sup></sub></sub> `32`<sub><sub><sup>(10)</sup></sub></sub> through `7F`<sub><sub><sup>(16)</sup></sub></sub> `127`<sub><sub><sup>(10)</sup></sub></sub> are used to represent standard ASCII characters.
    - `01`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> through `07`<sub><sub><sup>(16)</sup></sub></sub> `7`<sub><sub><sup>(10)</sup></sub></sub> are used to represent special characters.

Response after the text has finished scrolling:

`15`<sub><sub><sup>(16)</sup></sub></sub><br>
`21`<sub><sub><sup>(10)</sup></sub></sub>

### Stop Text Scrolling

Any active text scroll will be cancelled immediately.

`14`<sub><sub><sup>(16)</sup></sub></sub><br>
`20`<sub><sub><sup>(10)</sup></sub></sub>

## Custom palette data messages

This set of messages receives and stores a palette from the Launchpad Firmware Utility. All request and response messages begin with the same standard header:

`52 45 54 49 4E 41`<sub><sub><sup>(16)</sup></sub></sub><br>
`82 69 84 73 78 65`<sub><sub><sup>(10)</sup></sub></sub>

### Start download

Prepares the firmware for accepting a palette.

`7B`<sub><sub><sup>(16)</sup></sub></sub><br>
`123`<sub><sub><sup>(10)</sup></sub></sub>

### Write

Writes RGB colors to indexes in the palette.

`3D <<color> <R> <G> <B>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`61 <<color> <R> <G> <B>>...`<sub><sub><sup>(10)</sup></sub></sub>

- `<color>` is the index of the color in the palette, ranging from 0 to 127.

### End download

Applies the downloaded palette to the desired slot.

`7D <slot>`<sub><sub><sup>(16)</sup></sub></sub><br>
`125 <slot>`<sub><sub><sup>(10)</sup></sub></sub>

`<slot>` is the desired palette slot to store the palette in, ranging from 0 to 3.

### Upload

Multiple of these messages are sent *from the Launchpad to the host** when the user presses the upload button while editing a palette slot.

`<color> <R> <G> <B>`

## Custom mode data messages

This set of messages receives and stores a custom mode from the Launchpad Firmware Utility. All request and response messages begin with the same standard header:

`43 55 53 54 4F 4D`<sub><sub><sup>(16)</sup></sub></sub><br>
`67 85 83 84 79 77`<sub><sub><sup>(10)</sup></sub></sub>

### Start download

Prepares the firmware for accepting a custom mode in the desired slot.

`7B <slot>`<sub><sub><sup>(16)</sup></sub></sub><br>
`123 <slot>`<sub><sub><sup>(10)</sup></sub></sub>

`<slot>` is the desired custom mode slot to store the custom mode in, ranging from 0 to 7.

### Write

Writes the raw custom mode data to the firmware.

`3D <data>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`61 <data>...`<sub><sub><sup>(10)</sup></sub></sub>

### End download

Finalizes the downloaded custom mode.

`7D`<sub><sub><sup>(16)</sup></sub></sub><br>
`125`<sub><sub><sup>(10)</sup></sub></sub>

### Upload

Multiple of these messages are sent **from the Launchpad to the host** when the user presses the upload button while in a custom mode.

`<size[2]> <data>...`

`<size[2]>` is the size of the custom mode data in bytes, in big-endian format, and is only sent for the first chunk of data. For subsequent chunks, this variable is omitted.

## Custom fast LED messages

These custom messages are intended to be used when the quickest possible processing time is required, usually due to a large amount of data being sent (for example, raw Apollo Studio light effects).

They do not have an additional header apart from the standard SysEx Start code.

### Light one or more LEDs (RGB)

`6F <<XY> <R> <G> <B>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`111 <<XY> <R> <G> <B>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Flash one or more LEDs (RGB)

`6B <<XY> <R> <G> <B>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`107 <<XY> <R> <G> <B>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Pulse one or more LEDs (RGB)

`6C <<XY> <R> <G> <B>>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`108 <<XY> <R> <G> <B>>...`<sub><sub><sup>(10)</sup></sub></sub>

### Light LEDs via repeatable compressed message (Apollo Studio - Heaven renderer)

`5F <<R> <G> <B> <N> <X>...>...`<sub><sub><sup>(16)</sup></sub></sub><br>
`95 <<R> <G> <B> <N> <X>...>...`<sub><sub><sup>(10)</sup></sub></sub>

Stock firmware RGB LED set messages use the format `<<XY> <R> <G> <B>>...`. With this format and the 320 byte message size limit, it's impossible to pack a full screen update.

The grid update message can be used, but it requires you do a full screen update every time when you sometimes want to leave certain LEDs untouched, and also doesn't support updating the side LED.

The compressed message format is as follows:

- `<R> <G> <B>` are the 6-bit RGB color components for this compressed update, respectively.

- `<N>` is the number of `<X>` that follow.

- If `<N>` is less than 8, an additional byte can be saved by using the 7th bits of `<R> <G> <B>` bytes to encode the value of `<N>` in big-endian format. This means that the 7th bit of `<R>` is the most significant bit, and the 7th bit of `<B>` is the least significant bit. In this case, `<N>` should be omitted from the message. If `<N>` is 8 or greater, `<N>` is sent as a separate byte.

- `<X>...` represent all the groups of LEDs that will be updated to the same color:
    - The value `0`<sub><sub><sup>(16)</sup></sub></sub> `0`<sub><sub><sup>(10)</sup></sub></sub> is used to represent the entire grid of LEDs.
    - The values `1`<sub><sub><sup>(16)</sup></sub></sub> `1`<sub><sub><sup>(10)</sup></sub></sub> through `63`<sub><sub><sup>(16)</sup></sub></sub> `99`<sub><sub><sup>(10)</sup></sub></sub> are used to represent individual LEDs, with `63`<sub><sub><sup>(16)</sup></sub></sub> `99`<sub><sub><sup>(10)</sup></sub></sub> representing the side LED.
    - The values `64`<sub><sub><sup>(16)</sup></sub></sub> `100`<sub><sub><sup>(10)</sup></sub></sub> through `6D`<sub><sub><sup>(16)</sup></sub></sub> `109`<sub><sub><sup>(10)</sup></sub></sub> are used to represent entire rows.
    - The values `6E`<sub><sub><sup>(16)</sup></sub></sub> `110`<sub><sub><sup>(10)</sup></sub></sub> through `77`<sub><sub><sup>(16)</sup></sub></sub> `119`<sub><sub><sup>(10)</sup></sub></sub> are used to represent entire columns.
    - The values `78`<sub><sub><sup>(16)</sup></sub></sub> `120`<sub><sub><sup>(10)</sup></sub></sub> through `7F`<sub><sub><sup>(16)</sup></sub></sub> `127`<sub><sub><sup>(10)</sup></sub></sub> are reserved for future use.
