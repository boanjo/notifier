# notifier
A visual alarm (led) controller over MQTT - Let the kids know it's dinner, or time to walk the dog in a non intrusive manner without shouting. Less frustration for You and no risk of interfering any streaming...

## Background
When moving to a bigger house i quickly realized it wasn't as easy to communicate between the floor anymore, especially when headphones are on and gaming. My first solution was to use and automation with homeassistant and blinking the kids Philips hue lights. Even if it has worked fairly well for a year or two it has made the HUE lights to give up on me a number of times and i wanted something with more control over. The solution is to hook up a wemos D1 mini lite (Simple and cheap esp8266 chip) to my existing MQTT GW and let it control a few blinking schemes for a Adafruit Neopixel 12 LED ring. Case models and STL files are included to be 3D printed, shaped as a puck with mounting of the ESP chip and neopixel.

![1](https://github.com/boanjo/boanjo.github.io/blob/master/notifier11.jpg?raw=true "Pic")

In the Fusion 360 folder you have the full assembly if you want to do your own modifications or simple print files from the STL directory.
![2](https://github.com/boanjo/boanjo.github.io/blob/master/notifier12.jpg?raw=true "Fusion 360 Model")

## BOM
1. Wemos D1 mini lite (3.0.0) [Amazon](https://www.amazon.se/AZDelivery-ESP8266EX-WLAN-modul-kompatibel-inklusive/dp/B08BTRPMV1/ref=asc_df_B08BTRPMV1/?tag=shpngadsglede-21&linkCode=df0&hvadid=476429357278&hvpos=&hvnetw=g&hvrand=13859086852657585496&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1012321&hvtargid=pla-991254816887&mcid=9c4e8c5c234d3adea85e26105b407f85&th=1) Or if you are in Sweden [Kjell&Co](https://www.kjell.com/se/produkter/el-verktyg/elektronik/utvecklingskit/arduino/utvecklingskort/luxorparts-wemos-d1-mini-utvecklingskort-p87294)
2. Adafruit Neopixel 12 LED ring [Adafruit](https://www.adafruit.com/product/1643) Or if you are in Sweden [Electrokit ](https://www.electrokit.com/neopixel-ring-12-rgb-leds-37mm)
3. M3 (3pcs) threaded inserts (for the USB cable clamp and neopixel mounting)
4. M3 (3pcs) screws
5. M2 (2pcs) threaded inserts (for the wemos board)
6. M2 (2ocs) screws
7. 1000uF electrolytic capacitor
8. USB cable + charger with 1A capacity

## How Build
For more detailed instructions, please refer to the [README.md](../build_instructions/README.md) file.
 
