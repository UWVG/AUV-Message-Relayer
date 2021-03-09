# AUV-Message-Relayer 
![GPL License](https://img.shields.io/github/license/UWVG/AUV-Message-Relayer) ![size](https://img.shields.io/github/repo-size/UWVG/AUV-Message-Relayer) ![lines](https://img.shields.io/tokei/lines/github.com/UWVG/AUV-Message-Relayer) ![score](https://img.shields.io/codefactor/grade/github/UWVG/AUV-Message-Relayer/STM32H743ZIT6)

It runs on stm32 cortex-M series MCU that relays data between the AUV Master and sensors or actuators of AUV.
# How to use
```
git clone git@github.com:UWVG/AUV-Message-Relayer
git submodule init
git submodule update
```
Then a simple `STM32CubeIde` should do.
# Features
1. use freeRTOS as operation system
2. use rosserial protocal to communicate with ROS master
3. more and more common sensor drivers
# How to transplant to other MCU
1. rosserial,B30,P30 [reference](https://blog.csdn.net/Devout_programming/article/details/114098117)
# How to contribute
1. Fork this branch.
2. Commit your changes.
3. Send pull requests.
