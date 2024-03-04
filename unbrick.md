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

![ST-LINK Connection Diagram](https://github.com/mat1jaczyyy/lpp-performance-cfw/assets/13300194/a72f6719-66b2-4c07-9317-ac7fe0d79ba5)

## Reflash the memory

Run the ST-LINK Utility. Make sure your ST-LINK's firmware is updated:

![ST-LINK Firmware Update](https://github.com/mat1jaczyyy/lpp-performance-cfw/assets/13300194/b7d47c42-89dd-40d1-bca6-261277197773)

Connect to the bricked Launchpad Pro:

![A trashed bootloader](https://github.com/mat1jaczyyy/lpp-performance-cfw/assets/13300194/936f0f44-ede6-49e7-b641-64357cc80149)

Program & Verify the memory backup file:

![Fixing it...](https://github.com/mat1jaczyyy/lpp-performance-cfw/assets/13300194/9f1df675-5c1d-4a8e-a341-aa32ec8b3361)

After you're done flashing, disconnect the ST-LINK and the Launchpad should boot to Vegas mode straight away.

![Fixed!](https://github.com/mat1jaczyyy/lpp-performance-cfw/assets/13300194/3207efd6-65c6-4664-ac38-dc843748d49c)

Have fun reassembling it :)
