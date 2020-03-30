# SpottyWelder
## NOTE NOTE NOTE NOTE
**Not only can this project kill you, you will be in pain the entire time you're dying.  Please don't do this project unless you are capable of working on high voltages.  Consider this your disclaimer.  I am not to be held liable for your lack of safety.**


## Intro
A project I wrote to retrofit my Sunkko 737g spot welder.  Enables me to customize number of pulses, duration of those pulses, sleep time between pulses, and eventually if I get around to it I'll have a way to vary the amount of power output (coded, but not physically implemented).

## Part list (original unit)
| Part | Function | Link |
| ---- | -------- | ---- |
| Sunkko 737G | Spot Welder | https://www.amazon.com/gp/product/B07CV68VZD |
| Sunkko 719A | Mobile pen attachment for welder | https://www.amazon.com/gp/product/B076VHZ14T |

## Part list (Retrofit)
| Part | Function | Link |
| ---- | -------- | ---- |
| Featherwing OLED | Display | https://www.adafruit.com/product/2900 |
| Feather Huzzah | Microcontroller | https://www.adafruit.com/product/2821 |
| Rotary push-button encoder | changing values, changing menu | https://www.adafruit.com/product/377 |
| 25A Solid State Relay | Triggering power to transformer | https://www.amazon.com/SSR-25-DC-AC-Solid-State-Relay/dp/B07FVR37QN |
| Wago/lever-nut 2-wire connector | to reattach neutral wires together for transformer | https://www.amazon.com/gp/product/B078LXD3CF |

## Pinouts
| ESP pin | Function |
| ------- | -------- |
| 14 | Encoder Pin A |
| 12 | Encoder Pin B |
| 15 | Rotary button |
| 0 | Featherwing OLED Button A |
| 16 | Featherwing OLED Button B |
| 2 | Featherwing OLED Button C (and borrowed for firing welder) |
| 3 | Relay sink pin (Attach relay ground to this) |
| VBUS/VCC | 5v from usb, attach to relay positive |

## Physical build log/notes

  * Initial unit came from factory working well for spot welding from the onboard terminals, but the welder pen wouldn't work well.  My diagnosis: needed longer amount of time for current application so pen could work better.
  * Plan of attack: completely retrofit interior electronics, essentially making use only of existing transformer and wiring
  * design idea: use solid state relay to switch on transformer for length of time necessary for positive weld. 
    * sub-idea: replace the relay with a power mosfet to vary the amount of current the transformer gets.  This is how the Sunkko737g varies its power output currently, but you can't change length-of-time from the front panel.  If I could have, this project would not have been necessary
  * welding pen grub screws were complete shit.  I replaced them with better screws.
  * In my implementation I physically cut the four AC wires going to the board.  The front panel is completely useless now, but I left it in in case I ever wanted to restore the unit to original functionality.
    * In my retrofit I attached the neutral wires with a lever-nut (wago-style) connector

## How do I make it do?
  * Download this SpottyWelder.ino.
  * Compile it in arduino for your esp8266 board.
  * try not to electrocute yourself in the process of wiring things up.

## Known issues
 * I think I actually pulse twice even if the pulses is set to 1, occasionally.  Probably a code bug.
 * When the microcontroller powers up, it briefly sinks the relay pin.  This causes the transformer to pulse.  You probably want to have the transformer unplugged until the microcontroller is initialized.
 * I have the microcontroller living outside of the box currently, I should really mount it internally.
 * There is no power switch, so if it is plugged in assume it is a little box of death waiting to vaporize you.