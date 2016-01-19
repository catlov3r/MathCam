# MathCam
This is an arduino project that uses an LCD and Keypad to trigger a camera to take a photobooth style picture.


This uses
-an Arduino Uno (https://www.sparkfun.com/products/11021, but any Arduino will work)
-a 12 Button Keypad (https://www.sparkfun.com/products/8653)
-a Serial enabled LCD (https://www.sparkfun.com/products/11021)
-2 RJ45 Breakout boards (https://www.sparkfun.com/products/716)
-2 RJ45 8-pin connectors (https://www.sparkfun.com/products/643)

The RJ45’s are so that the keypad and LCD can be located further away from the micro controller and camera easier. If you want to use standard wires to run everything that is fine too. 

The code has the ability to generate a math problem where you have to add/subtract 2 or 3 numbers. The code to do the three number version is commented out to enhance the speed of the code and because the children I tested this with were to young to do the problems in their head.

- - - - - 

  This is designed to hook up to a serial enabled LCD,
  a 12 button keypad, and an optoisolator to control
  a Canon Rebel digital SLR.

  The software serial library is used to connect the LCD
  to pints 9 and 10. The keypad is connected to pins
  2, 3, 4, 5 (rows), 6, 7, 8 (columns). This is connected
  using an RJ45 cable so the key pad can be easily moved
  to a convenient place away from the microcontroller.
  Pin 12 is connected to the opto-isolator to control the
  shutter release.

  For the opto isolator pin 12 on the arduino is conencted
  to pin 2 on the opto. Pin 1 on the opto goes thought a
  10k resistor to ground. Pins 4 and 5 on the opto are
  connected to the camera via serio jack that fits the
  camera. Experimentation will be needed to figure out
  which 2 of the 3 wires in the cable release the
  shutter. The opto isolator is used to prevent damage
  to the camera. Also, the camera is designed so when
  two pins are connected it either focuses or releases
  the shutter, meaning no external power is needed.

  Analog 0 is used to help with the random number
  generator. The basic idea is to have a 2 or 3
  number math equation that has a positive answer
  generated and displayed on the screen. The keypad
  is then used as input (which is displayed on the
  screen) to answer the math problem. If the problem
  is answered correctly the camera will take a picture.
  Typically the camera is hooked up to a large TV or
  projector so that the image captured is displayed.
  The result is kept positive since the keypad used
  does not have a labeled minus sign.

