# LoraWan Arduino tests on Heltec LORA 32

## Setup of VSCode

1) Install Arduino IDE
2) Install VSCode
3) Install Arduino extension from Microsoft
4) In Arduino IDE, install Heltec Lora 32 extension: https://heltec-automation-docs.readthedocs.io/en/latest/esp32/quick_start.html

## Setup project

1) Create a new workspace.
2) Create a folder for the project. Folder must contain subb directory named 'build' (this one will be used for Arduino build, it will speed up subsequent compilations). Add the project folder to workspace. 
3) Setup Git repo (don't forget to create a .gitignore to exclude the build folder, .vscode folder, etc).
4) Setup the Heltec Lora 32 board (shift+cmd+P, Arduino:Board Config), and the serial port to use (shif+cmd+P, Arduino:Select Serial Port).
5) Setup libraries (shift+cmd+P, Arduino: Library Manager): you will need MCCI LoraWan LMIC Libray (https://github.com/mcci-catena/arduino-lmic) and U8g2 for screen (https://github.com/olikraus/u8g2).

## References/articles
### Articles used to develop this test
TTN Howto with Heltec Lora 32 and the LMIC library: https://nathanmcminn.com/2018/09/12/tutorial-heltec-esp32-board-the-things-network/

ESP32 internal temperature sensor example: https://circuits4you.com/2019/01/01/esp32-internal-temperature-sensor-example/

Arduino libraries folder location: https://forum.arduino.cc/index.php?topic=88380.0

### Misc other articles
https://robotzero.one/heltec-lora32-lorawan-node/


