# Bricked it?

Your Launchpad should never get bricked if you're just regularly using the firmware, but if you're making modifications while developing, it can happen time to time. You can easily recognize a brick by the inability to boot into the bootloader. Having unbootable firmware means you just need to flash a fresh one, while being unable to reach the bootloader means you can't upload any code to your Launchpad anymore.

Please don't do this unless you actually know what you're doing!

## Required tools

* A bricked Launchpad Pro :(
* A cheap [ST-LINK V2](https://www.ebay.com/itm/ST-Link-V2-Mini-Debugger-und-Programmier-Emulator-fur-STM8-STM32-Arduino/392963947934)
* The [STM32 ST-LINK Utility](https://drive.google.com/file/d/1cXBo6Me6DnHjiTBbPt1cs_HqEM6Hhn2g/view)
* 4 female-to-male jumper wires
* A [backup of the Launchpad Pro's memory](https://cdn.discordapp.com/attachments/636554539096473600/826560546890579998/pro_backup.bin)

## Perform surgery on the hardware

Disassemble the Launchpad Pro, only detaching the power management module. Make sure not to lose the tiny power button! With the CPU exposed, connect the following points to the ST-LINK. You'll likely have to hold the connection to `SWDIO` yourself, rather than having it keep itself plugged in:

![ST-LINK Connection Diagram](https://cdn.discordapp.com/attachments/636554539096473600/826556448116310106/stlink.png)

## Reflash the memory

Run the ST-LINK Utility. Make sure your ST-LINK's firmware is updated:

![ST-LINK Firmware Update](https://cdn.discordapp.com/attachments/636554539096473600/826559020504186961/unknown.png)

Connect to the bricked Launchpad Pro:

![A trashed bootloader](https://cdn.discordapp.com/attachments/636554539096473600/826557447958823012/unknown.png)

Program & Verify the memory backup file:

![Fixing it...](https://media.discordapp.net/attachments/636554539096473600/826558543309570078/unknown.png)

After you're done flashing, disconnect the ST-LINK and the Launchpad should boot to Vegas mode straight away.

![Fixed!](https://media.discordapp.net/attachments/636554539096473600/826558594379415552/unknown.png)

Have fun reassembling it :)