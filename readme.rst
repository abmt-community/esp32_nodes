========
Nodes for ESP32 runtime
========

Get the corresponding runtime from https://github.com/abmt-community/rt_esp32.

Examples
========

blinky
------
Blinking LED on GPIO8. Important: On some controllers GPIO8 is uses for flash and changing its value will cause a reset!!!

echo
----
Starts an UDP and a TCP server on port 12345 and replies every received message. Also UART echo can be enabled by uncommenting nodes.

http_led
--------
Slider App that controls GPIO8 pwm. Important: On some controllers GPIO8 is uses for flash and changing its value will cause a reset!!!

http_led
--------
REST-Command example. Open "http://esp32.local/cmd.html". You can also set an LED on GPIO8 with the command "led" and the value "true". On some controllers GPIO8 is uses for flash and changing its value will cause a reset!!!