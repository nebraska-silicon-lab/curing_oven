# curing_oven
This system utilizes a microcontroller, temperature sensor, household toaster oven, and solid state relay to create a high-resolution controlled-temperature environment within the oven. Due to the range of the temperature sensor, the oven can maintain up to around 90 C; to change the ideal temperature for the oven to maintain, change the "setpoint" in the code twice, along with adjusting the temperatures within the preheat function (labeled in code).

**Part numbers:**
- Toaster oven: Any toaster oven should work; this project utilizes the Hamilton Beach 31401
- Temperature sensor: Adafruit ADT7410
- Microcontroller: Arduino UNO or compatible
- Solid state relay: Omega Engineering SSRL240DC10
- LCD screen: Adafruit 16x2 RGB LCD with shield

**Circuit diagram:**
- LCD shield wiring can be found through the following link: https://learn.adafruit.com/rgb-lcd-shield/using-the-rgb-lcd-shield
- The push button is now integrated into the LCD shield as the "SELECT" button
- The temperature sensor shares  data and power pins with the shield
- Relay wiring accurately depicted below, output pin defined in code

![Imgur](https://i.imgur.com/ZhhKUME.png)
