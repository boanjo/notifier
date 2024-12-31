# Firmware
The firmware is pretty simple and can be extended or changed as you like:
* Connect to WiFi
* Connect to MQTT Gateway
* Listen for incoming commands
* Periodically report status and acknowledge commands

## Prerequisite
To be able to use the source code straight up, I really recommend installing PlatformIO in VSCode. Download and install VSCode, then search for and install PlatformIO. Of course, you can copy and paste the content from the `src` and `include` folders into the Arduino IDE too.

## Code updates needed
All code You need is to change are a few lines in the include/details.h file. 
* Wifi credentials - goes without saying. 
* Add the IP address of your MQTT gateway (maybe the same as your homeassistant).  
* Select a basetopic i.e. the topic that is what you send over your MQTT bus. It can be a logical name with underscore like 'basement' or 'food_alert' etc.
* Then remove the "define ERROR" since it's only there to ensure you just don't donwload and build without changing this personal stuff 

## API
There is little or no error handling for the API to not add more code than recessary so provide the json attributes as below and you should be fine:

### mode
* on - All LEDs on
* all LEDs off
* worm - the LEDs in the ring will move in a worm like circle
* flash - Flashing the LEDs

### seconds
* Number of seconds to repeat the mode range 1 to 4294967 (like 49 days)

### red
* The RGB R value 0-255

### green
* The RGB G value 0-255

### blue
* The RGB B value 0-255

### req_id
* Id to be returned in ack message (i.e. if you want to verify and keep track of which command is acknowledged)

## Example
Flash green light for 10 seconds for the topic/device oliver_food (note it's ok to have multiple devices listeing to the same topic)
```
oliver_food/cmd {'mode'='flash','seconds'=10,'red'=0,'green'=255,'blue'=0,'req_id'=0}
```