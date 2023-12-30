# Embedded-System-Design
TCES 460.

This is an extension of Project_2b. The objective of this project is to read the measured temperature on a laptop/cell phone. We used UART5 to connect the Bluetooth module (ZS-040) with the TM4C123 microcontroller and the software 'Bluetooth Serial Terminal' to read the temperature on the laptop. ZS-040 is from the same family as HC-05. Therefore, the PIN orientation, initialization, and connection of ZS-040 are very similar to HC-05.

Using the UART communication protocol to connect with TM4C123, we first selected the UART module (Tx and RX pins) and connected 3.3 V and GND. The program satisfies the following conditions:

(i) If you send the 'A' message from the app to the MC, the TM4C123 MC should respond with the current measured temperature (as temperature is measured every 10 seconds). The response should be "Current Temperature is:" followed by "the value". Additionally, it should print "Temperature increase:", followed by "difference" or "Temperature decrease:", followed by "difference" or "Temperature unchanged." This is the temperature difference of the last two measures. We can assume this property as an app/user-initiated activity.

(ii) The second feature would be that if the difference between the current and last temperature is 2 degrees or less, the RED LED will blink twice (assume the heater will turn ON). Similarly, in the opposite case, the Green LED will blink twice (assume the cooler will turn ON). As in the background, periodic ADC interrupt measures temperature all the time, TM4C123 would initiate this activity. 
