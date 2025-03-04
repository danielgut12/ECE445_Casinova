
  

# Matthew Tzeng (mttzeng2, 668326215) Worklog

### **2/17/24: Worked on Creating our Project Proposal**
Worked with Danny to create and review our project proposal. We decided the basic layout of our design and the necessary modules and parts required to have our card dealer work. We also discussed the memory limitations of the ESP32-S3 when it came to image processing, and solutions for the issue

Our solution included the following
- Preprocess images so that the microcontroller will have less to process, saving memory
- Reduce the amount of pixels required for the front facing camera to only include the middle vertical 1/3 strip, as the left and right image data isn't required for the needs of the front facing camera (person detection)

### **2/19/24: Finding Compatible Parts (Camera Module)**

Looked into camera module parts for breadboard demo

  

**TOF10120 Distance Sensor \[[Link](https://www.amazon.com/HUABAN-TOF10120-Flight-Distance-Measuring/dp/B089SLWYZ9)\]**

- Able to sense distance from 5-180cm

- UART interface (works well with ESP32)

  

**OV2640 Camera \[[Link](https://www.amazon.com/FORIOT-160%C2%B0I2C-Wide-Angle-Megapixel-Support/dp/B0CJTNMXXF?crid=16GO757L87GEY&dib=eyJ2IjoiMSJ9.VAPdNR6AhUwajSD1-c8640rCYcfi4aIlIy5JA0B8H9hrbjvv2HHuAkLVu5IG3L6mVJAbinlTbEveBPrU3Ry7V3z5vj_wedqd0HloyFT3cYvDIAsfa7ImFoZkwY_6OTf2buawd7tlji2cseHQ_sv6VP4uh3wL4xyb7cunhBGpTti8YK2bQZN7bm6aHhqtMASnnnGI2s3DUEu-gmAgMtKT3XBpgIaOz1i1P-SANH0gvu5tggVyINBpIOFS0ot4tPVsGQGVVx1O1oF9oeTm6OnbsOtIhX2v--ePEBteaT4Z7XXSj0vxHqn0J8SUBCqYagWirHWFAMG2oWTgGmekEWLAm8B3eK74JUSikbCUx0DAguw.kKNsyZQ7h07s_YDiGwuIJRy_LRNTWK70FV4P8Am_6i0&dib_tag=se&keywords=ov2640&qid=1740619820&s=electronics&sprefix=ov2640%2Celectronics%2C113&sr=1-4&th=1)\]**

- 160 degree wide angle lens (good for up close card)

- 24 pin adapter

- Provides pinouts for specific model

  

**USB Camera Module \[[Link](https://www.amazon.com/Camera-Module-W202012HD-Compatible-Computer/dp/B0B78LHKPG?crid=3G7LJH8YKHWZL&dib=eyJ2IjoiMSJ9.FVVlnFujs4LI5Ro7zd7O-xBIGWeNg9w6kl3cWi-viThUx4Olhl65Qiw1T6ABograkvnSwIjJ1moGTK0J90LyqTyDvitwHyNg_jJnKe5Jd_f-Ifelz-yeLSr1AOia62L02x0SUMsP0lAiVo8cDVE9JmzmneXn_5dp1vu3Vz5BwkONRGwvY4i1tx3cL7Cg5Xe64xx-0WNxcRCqXsSvOPQa_koTOgczfeTMbujDYlImghnXR9JYfuIChSHv00zhXzI6OeJhrkMmjea9eHChC6-Ox6ExaKNBev_-IJmc-jmqgVQS5v5iuB2Dv8wGsMHUqG_mMQIXXKJjiMf_y_KCPjG75MXRAOJOhG-vfCeSqikdq58.-EXYxZJ_KwLhN7BNXfcYO3mMSVLxy__3YbJ05BFIE5w&dib_tag=se&keywords=usb+camera+module+ov2640&qid=1740623914&s=electronics&sprefix=usb+camera+module+ov2640%2Celectronics%2C104&sr=1-30&xpid=_GWS3coQVN7Fp)\]**

  

  

- Uses UART protocol (only requires two GPIO pins)

- Ideally used for front facing camera (towards player)

##

  

## **2/24/24: Planning GPIO Pin Routing for Camera Module**



### Resources utilized for today

- [OV2640 Datasheet](https://www.mpja.com/download/ov2640data%20sheet.pdf)

- ![Pinout](./media/ov2640_pinout.png)
- Decided that D0-7 will be connected to GPIO pins in range of 6-16 (High Speed)
- ![OV2640 Pin Schematic](./media/ov2640_sch.png)
- Pinout of the OV2640 connected to the GPIO pins of the ESP32
- 
![Camera Schematic](./media/camera_sch.png)


## **2/26/24: Ordered Parts For Project With Danny**
Danny and I discussed which parts we're necessary for the project, and we ended up ordering all the parts required for the breadboard demo on Amazon. These parts included
- Stepper Motor
- Stepper Motor Driver
- DC Motor Driver (We already have 2 DC Motors)

- ESP32-S3 Dev Board (x2)
    - Helpful for us both if we can each have a dev board to work on seperate modules concurrently
- OV2640 Camera (x3)
    - Came in pack of 3
- FFC 24-pin connector 0.5mm
    - For the OV2640 camera to connect to breadboard
- TOF10120 Distance Sensor
- USB 1MP Camera
- USB-A Breakout Board (Female)

With these parts, we should be able to complete the breadboard demo section of this project.