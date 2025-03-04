
## **2/14/24: Finding Compatible Parts (Camera Module)**

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

## **2/17/24: Planning GPIO Pin Routing for Camera Module**

