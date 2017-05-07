# MegaCore
An Arduino core for ATmega64, ATmega128, ATmega640, ATmega1280, ATmega1281, ATmega2560 and ATmega2561, all running a [modified version of Optiboot](#write-to-own-flash). Major libraries such as SD, Servo, SPI and Wire are modified to work with this core. Still, a large amount of third-party libraries often works without any modifications. <br/>
This core requires at least Arduino IDE v1.6, where v1.6.12+ is recommended. <br/>
If you're into "pure" AVR programming, I'm happy to tell you that all relevant keywords are being highlighted by the IDE through a separate keywords file. Make sure to test the [example files](https://github.com/MCUdude/MegaCore/tree/master/avr/libraries/AVR_examples/examples) (File > Examples > AVR C code examples).


# Table of contents
* [Supported microcontrollers](#supported-microcontrollers)
* [Supported clock frequencies](#supported-clock-frequencies)
* [BOD option](#bod-option)
* [Link time optimization / LTO](#link-time-optimization--lto)
* [Programmers](#programmers)
* [Write to own flash](#write-to-own-flash)
* **[How to install](#how-to-install)**
	- [Boards Manager Installation](#boards-manager-installation)
	- [Manual Installation](#manual-installation)
* **[Getting started with MegaCore](#getting-started-with-megacore)**
* [Wiring reference](#wiring-reference)	
* **[Pinout](#pinout)**
* **[Minimal setup](#minimal-setup)**


## Supported microcontrollers:
* ATmega2561
* ATmega2560
* ATmega1281
* ATmega1280
* ATmega640
* ATmega128
* ATmega64
  
(All variants - A, L, V)
<br/> <br/>
Can't decide what microcontroller to choose? Have a look at the specification table below:

|              | Mega2560 | Mega1280 | Mega640 | Mega2561 | Mega1281 | Mega128 | Mega64 | 
|--------------|----------|----------|---------|----------|----------|---------|--------|
| **Flash**    | 256kB    | 128kB    | 64kB    | 128kB    | 128kB    | 128kB   | 64kB   |
| **RAM**      | 8kB      | 8kB      | 8kB     | 8kB      | 8kB      | 4kB     | 4kB    |
| **EEPROM**   | 4kB      | 4kB      | 4kB     | 4kB      | 4kB      | 4kB     | 2kB    |
| **IO pins**  | 86 *     | 86 *     | 86 *    | 54       | 54       | 53      | 53     |
| **PWM pins** | 15       | 15       | 15      | 8        | 8        | 7       | 7      |
| **LED pin**  | PB7      | PB7      | PB7     | PB5      | PB5      | PB5     | PB5    |

<b>*</b> 86 IO pins is only available with the [*AVR pinout*](#atmega64012802560) selected


## Supported clock frequencies
* 16 MHz external oscillator (default)
* 20 MHz external oscillator
* 18.432 Mhz external oscillator <b>*</b>
* 12 MHz external oscillator
* 8 MHz external oscillator
* 8 MHz internal oscillator <b>**</b>
* 1 MHz internal oscillator

Select your microcontroller in the boards menu, then select the clock frequency. You'll have to hit "Burn bootloader" in order to set the correct fuses and upload the correct bootloader. <br/>
Make sure you connect an ISP programmer, and select the correct one in the "Programmers" menu. For time critical operations an external oscillator is recommended. 
</br></br>

<b>*</b> When using the 18.432 MHz option (or any frequency by which 64 cannot be divided evenly), micros() is 4-5 times slower (~110 clocks). It reports the time at the point when it was called, not the end.
This clock frequency is not recommended if your application relies on accurate timing, but is [superb for UART communication](http://wormfood.net/avrbaudcalc.php?bitrate=300%2C600%2C1200%2C2400%2C4800%2C9600%2C14.4k%2C19.2k%2C28.8k%2C38.4k%2C57.6k%2C76.8k%2C115.2k%2C230.4k%2C250k%2C.5m%2C1m&clock=18.432&databits=8). 
millis() is not affected, only micros() and delay(). Micros() executes equally fast at all clock speeds, but returns wrong values with anything that 64 doesn't divide evenly by.
</br></br>

<b>**</b> There might be some issues related to the internal oscillator. It's factory calibrated, but may be a little "off" depending on the calibration, ambient temperature and operating voltage. If uploading failes while using the 8 MHz internal oscillator you have three options:
* Edit the baudrate line in the [boards.txt](https://github.com/MCUdude/MegaCore/blob/11fd19fd0a5dcc808f4e8fc0b627c4477df515c8/avr/boards.txt#L59) file, and choose either 115200, 57600, 38400 or 19200 baud.
* Upload the code using a programmer (USBasp, USBtinyISP etc.) or skip the bootloader
* Use the 1 MHz option instead 


## BOD option
Brown out detection, or BOD for short lets the microcontroller sense the input voltage and shut down if the voltage goes below the brown out setting. To change the BOD settings you'll have to connect an ISP programmer and hit "Burn bootloader". Below is a table that shows the available BOD options:
<br/>

| Mega2560 | Mega1280 | Mega640  | Mega2561 | Mega1281 | Mega128  | Mega64  |
|----------|----------|----------|----------|----------|----------|---------|
| 4.3v     | 4.3v     | 4.3v     | 4.3v     | 4.3v     | 4.3v     | 4.3v    |
| 2.7v     | 2.7v     | 2.7v     | 2.7v     | 2.7v     | 2.7v     | 2.7v    |
| 1.8v     | 1.8v     | 1.8v     | 1.8v     | 1.8v     | -        | -       |
| Disabled | Disabled | Disabled | Disabled | Disabled | Disabled | Disabled|


## Link time optimization / LTO
After Arduino IDE 1.6.11 where released, there have been support for link time optimization or LTO for short. The LTO optimizes the code at link time, making the code (often) significantly smaller without making it "slower". In Arduino IDE 1.6.11 and newer LTO is enabled by default. I've chosen to disable this by default to make sure the core keep its backwards compatibility. Enabling LTO in IDE 1.6.10 and older will return an error. 
I encourage you to try the new LTO option and see how much smaller your code gets! Note that you don't need to hit "Burn Bootloader" in order to enable LTO. Simply enable it in the "Tools" menu, and your code is ready for compilation. If you want to read more about LTO and GCC flags in general, head over to the [GNU GCC website](https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)!


## Programmers
MegaCore does not adds its own copies of all the standard programmers to the "Programmer" menu. Just select one of the stock programmers in the "Programmers" menu, and you're ready to "Burn Bootloader" or "Upload Using Programmer".
 
Select your microcontroller in the boards menu, then select the clock frequency. You'll have to hit "Burn bootloader" in order to set the correct fuses and upload the correct bootloader. <br/>
Make sure you connect an ISP programmer, and select the correct one in the "Programmers" menu. For time critical operations an external oscillator is recommended.
 
 
## Write to own flash
A while ago [@majekw](https://github.com/majekw) announced that he'd [successfully modified the Optiboot bootloader](http://forum.arduino.cc/index.php?topic=332191.0) to let the running program permanently store content in the flash memory.
The flash memory is much faster than the EEPROM, and can handle about 10 000 write cycles. <br/>
With help from [majek](https://github.com/MCUdude/MegaCore/issues/6) this feature is working perfectly with the MegaCore! To enable this feature the bootloader needs to be replaced by the new one. Simply hit "Burn Bootloader", and it's done! <br/>
Please check out the [Optiboot flasher example](https://github.com/MCUdude/MegaCore/blob/9793029aea382b41fc5dd81aa8af909fbf244026/avr/libraries/Optiboot_flasher/examples/SerialReadWrite/SerialReadWrite.ino) for more info about how this feature works, and how you can try it on your MegaCore compatible microcontroller.


## How to install
#### Boards Manager Installation
This installation method requires Arduino IDE version 1.6.4 or greater.
* Open the Arduino IDE.
* Open the **File > Preferences** menu item.
* Enter the following URL in **Additional Boards Manager URLs**: `https://mcudude.github.io/MegaCore/package_MCUdude_MegaCore_index.json`
* Open the **Tools > Board > Boards Manager...** menu item.
* Wait for the platform indexes to finish downloading.
* Scroll down until you see the **MegaCore** entry and click on it.
  * **Note**: If you are using Arduino IDE 1.6.6 then you may need to close **Boards Manager** and then reopen it before the **MegaCore** entry will appear.
* Click **Install**.
* After installation is complete close the **Boards Manager** window.

#### Manual Installation
Click on the "Download ZIP" button in the upper right corner. Exctract the ZIP file, and move the extracted folder to the location "**~/Documents/Arduino/hardware**". Create the "hardware" folder if it doesn't exist.
Open Arduino IDE, and a new category in the boards menu called "MegaCore" will show up.


## Getting started with MegaCore
Ok, so you're downloaded and installed MegaCore, but do I get the wheels spinning? Here's a quick start guide:
* Hook up your microcontroller as shown in the [pinout diagram](#pinout).
	- If you're not planning to use the bootloader (uploading code using a USB to serial adapter), the FTDI header and the 100 nF capacitor on the reset pin can be omitted. 
* Open the **Tools > Board** menu item, and select **ATmega64**, **ATmega128**, **ATmega1281**, **ATmega2561**, **ATmega640**, **ATmega1280** or **ATmega2560**.
* Select your prefered clock frequency. **16 MHz** is standard on most Arduino boards.
* Select what kind of programmer you're using under the **Programmers** menu.
* Hit **Burn Bootloader**. If an LED is connected to pin PB5/PB7, it should flash twice every second.
* Now that the correct fuse settings is sat and the bootloader burnt, you can upload your code in two ways:
	- Disconnect your programmer tool, and connect a USB to serial adapter to the microcontroller, like shown in the [pinout diagram](#pinout). Then select the correct serial port under the **Tools** menu, and click the **Upload** button. If you're getting some kind of timeout error, it means your RX and TX pins are swapped, or your auto reset circuity isn't working properly (the 100 nF capacitor on the reset line).
	- Keep your programmer connected, and hold down the `shift` button while clicking **Upload**. This will erase the bootloader and upload your code using the programmer tool.

Your code should now be running on your microcontroller! If you experience any issues related to bootloader burning or serial uploading, please use *[this forum post](https://forum.arduino.cc/index.php?topic=386733.0)* or create an issue on Github.


## Wiring reference
To extend this core's functionality a bit futher, I've added a few missing Wiring functions. As many of you know Arduino is based on Wiring, but that doesn't mean the Wiring development isnt active. These functions is used as "regular" Arduino functions, and there's no need to include an external library.<br/>
I hope you find this useful, because they really are!

### Function list
* portMode()
* portRead()
* portWrite()
* sleepMode()
* sleep()
* noSleep()
* enablePower()	
* disablePower()

### For further information please view the [Wiring reference page](https://github.com/MCUdude/MegaCore/blob/master/Wiring_reference.md)!


## Pinout

### ATmega64/128/1281/2561
Since there are no standarized Arduino pinout for the ATmega64/128/1281/2561, I decided to create my own. I've tried to make it as simple and logical as possible. This pinout makes great sense if you're buying this [cheap breakout boards](http://www.ebay.com/itm/381547311629) at Ebay or AliExpress (just make sure to remove C3 in order to get auto reset working). The standard LED pin is assigned to Arduino pin 13, and will blink twice if you hit the reset button.

### ATmega640/1280/2560
Beside including the original Arduino Mega pinout for the ATmega640/1280/2560, I've also added what I call *The AVR pinout*, which is a more straight forward and logical pinout if you're not working with the Arduino Mega board. For the default Arduino Mega pinout, the standard LED pin is assigned to Arduino pin 13, and for the AVR pin it's assigned to pin 22.
<b>Click to enlarge:</b> <br/>
<img src="http://i.imgur.com/ez5MyvI.jpg" width="280"> <img src="http://i.imgur.com/k00WGLk.jpg" width="280"> <img src="http://i.imgur.com/DfR7arD.jpg" width="280">



## Minimal setup
Here's some simple schematics for the ATmega64/128/1281/2561 and ATmega640/1280/2560 showing a minimal setup using an external crystal. Omit the crystal and the two 22pF capacitors if you're using the internal oscillator. <br/> 
<b>Click to enlarge:</b> <br/>
<img src="http://i.imgur.com/h9J6rxg.png" width="400">    <img src="http://i.imgur.com/gQS1ORv.png" width="400">

