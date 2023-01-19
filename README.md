# esp32_button_remapper
I don´t actually know anything about esp32, but it's a nice project that i need for my jamma boards

I want to be able to switch jamma boards without having to struggle on button config.
On Naomi Netboot setups button config is also needed, so the magic here is to wire the buttons like so

# Button Config
This is the default for Capcom 6-button fighters

> [1] [2] [3]
> [4] [5] [6]

The goal is to change the game from a 6 button fighter to a 4 or 5 button fighter without having to change any wiring.

Will this be with a web based config? maybe..

For now i just have some dictionaries and arrays to start working :)

# ESP32 Details

**Input only pins:**
- GPIO 34
- GPIO 35
- GPIO 36
- GPIO 39

**Input OK pins:**
- GPIO 2
- GPIO 4
- GPIO 5
- GPIO 13
- GPIO 14 (Avoid, See Below)
- GPIO 15 (Avoid, See Below)
- GPIO 16
- GPIO 17
- GPIO 18
- GPIO 19
- GPIO 21
- GPIO 22
- GPIO 23
- GPIO 25
- GPIO 26
- GPIO 27
- GPIO 32
- GPIO 33
- GPIO 34
- GPIO 35
- GPIO 36
- GPIO 39

**Output OK Pins:**
- GPIO 2
- GPIO 4
- GPIO 5
- GPIO 12
- GPIO 13
- GPIO 14 (Avoid, See Below)
- GPIO 15 (Avoid, See below)
- GPIO 16
- GPIO 17
- GPIO 18
- GPIO 19
- GPIO 21
- GPIO 22
- GPIO 23
- GPIO 25
- GPIO 26
- GPIO 27
- GPIO 32
- GPIO 33

**Considerations:**

Pins HIGH at Boot (Avoid)

Some GPIOs change their state to HIGH or output PWM signals at boot or reset. This means that if you have outputs connected to these GPIOs you may get unexpected results when the ESP32 resets or boots.

- GPIO 1
- GPIO 3
- GPIO 5
- GPIO 6 to GPIO 11 (connected to the ESP32 integrated SPI flash memory – not recommended to use).
- GPIO 14
- GPIO 15

## Final Result?

Given that we can't use all the pins from the GPIO, we must consider using All the pins that we can.

### Lets start with Input pins first

**1P Side (Inputs)**
- GPIO 3 (RX0)
- GPIO 22 (D22)
- GPIO 23 (D23)
- GPIO 25 (D25)
- GPIO 26 (D26)
- GPIO 27 (D27)
  
**2P Side (Inputs)**
- GPIO 32 (D32)
- GPIO 33 (D33)
- GPIO 34 (D34)
- GPIO 35 (D35)
- GPIO 36 (VP)
- GPIO 39 (VN)

### Let's Continue with Possible Outputs
**1P Side (Outputs)**
- GPIO 2 (D2)
- GPIO 4 (D4)
- GPIO 5 (D5)
- GPIO 12 (D12)
- GPIO 13 (D13)
- GPIO 16 (RX2)
 
**2P Side (Outputs)**

- GPIO 17 (TX2)
- GPIO 18 (D18)
- GPIO 19 (D19)
- GPIO 1 (TX0)
- GPIO 21 (D21)
- GPIO 14 (D14)
  
**NOTES**
- GPIO 1, 3 and 14 must have some kind of clearance at boot time, as they might output some stuff and we don't want to damage the Jamma Board (using the output)