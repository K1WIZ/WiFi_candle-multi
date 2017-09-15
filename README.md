# WiFi_candle-multi
WiFi Flickering Candle using two LEDs

This version flickers two PWM channels on an ESP8266 type MCU.  
IP address is DHCP but can be hardcoded to allow for assignment and contact
from a home automation controller.  Use IP that is valid in
your network.

Control is possible by sending HTTP requests to /on  or /off
to keep things simple.

Two LEDs in a translucent shell create a realistic flame flickering
effect.  This can be put inside a frosted glass lantern with higher power
LEDs (3 or 10 watts) to create a stunning lantern effect.  If using 
higher power LEDs, you can use a generic N-Channel MOSFET to 
drive them.  

Enjoy & please let me know if you build one!  :)

John at wizworks dot net
