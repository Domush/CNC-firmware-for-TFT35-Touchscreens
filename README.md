# How to install vscode and platformio for compile
- Install the compilation environment, We recommend vscode + platformio. If you have already installed it, please skip this step.  If not, please see [here](https://github.com/bigtreetech/Document/blob/master/How%20to%20install%20VScode+Platformio.md).
# how to compile firmware

- Please see the picture(①), which is the PlatformIO plugin, and then click ② Open Project to open the project.
![image](https://user-images.githubusercontent.com/25599056/56637513-6b258e00-669e-11e9-9fad-d0571e57499e.png)
 
- Find the TFT firmware source directory , then click Open
![image](https://user-images.githubusercontent.com/25599056/56637532-77115000-669e-11e9-809b-f6bc25412f75.png)

- After opening the project, go to the platformio.ini file and change the default environment .
the “env_default” can be “BIGTREE_TFT35_V1_2”, “BIGTREE_TFT35_V3_0”, and “BIGTREE_TFT35_V2_0”
it depends on your hardware.
![image](https://user-images.githubusercontent.com/25599056/56637542-7d073100-669e-11e9-9e9e-1efa6b73be5d.png)

- After the modification is complete, press Ctrl+Alt+B, and platformio will automatically download the compiled component and compile it.
![image](https://user-images.githubusercontent.com/25599056/56637550-809ab800-669e-11e9-99d3-6b502e294688.png)
 
- After the compilation is successful, a BIQU_TFT35_V1.x.x.bin file will be generated in the .pioenvs\BIGTREE_TFT35_V1_x directory. We need copy this file to the SD card, and insert this SD Card to TFT35 Touch Screen, then reset the Touch Screen,  the firmware will be updated automatically.

# how to update firmware
copy firmware(.bin), fonts(TFT35/font) and icons(TFT35/bmp) to SD Card root directory, insert SD card to touch screen, reset touch screen and then it will be updated automatically.

The structure of the files in SD Card should be as follows
![image](https://user-images.githubusercontent.com/25599056/61274462-75a77d80-a7de-11e9-8e86-04d3d2abfb4d.png)
