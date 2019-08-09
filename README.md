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

**Final project**

There are some images of the final setup below; each image is followed by a caption describing the materials and assembly.

![Imgur](https://i.imgur.com/e4JDnOf.jpg)
- This image is a front photo of the full setup after it has been moved into a box. You can see here the LCD screen, the grey cabling leading to the temperature sensor (which is still in a breadbaord), and the black cable which provides a USB connection between the Arduino and a computer. The holes in the front of the box provide access to the "SELECT" button and potentiometer of the LCD shield, and the top hole provides access to the switch on the Arduino battery pack.

![Imgur](https://i.imgur.com/j0GKSqz.jpg)
- This image is a back photo of the electronics box, showing the two plugs integrated into the system: the standard power plug allows you to plug in the toaster oven, and the alternative style is simply a power cord leading to a wall outlet. These plugs are connected directly to the relay's load terminals, as seen in the following image.

![Imgur](https://i.imgur.com/o7EtDHE.jpg)
- This image shows the internal wiring of the box. The flow of power from the plugs is intercepted by the load terminal of the relay, while the control voltage comes from the Arduino. The black box towards the top is the battery pack mentioned before, and you can see the back of the Arduino Uno-compatible used with this project.
