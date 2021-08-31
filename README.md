# ESPCAM-LicensePlate


#### Notes:
- When using SD-Card remember to format the card to fat32. 
- Except when using CIF or lower resolution with JPEG, the driver requires PSRAM to be installed and activated.
    - Found in menuconfig->components->esp32-specific
- When using tools you need to create a virtualenv. 
    1. When created source the activate script to enter the venv.
    2. Run the command `pip install -e .` This will install necessary packages and make the script executable.
    3. Run `toolEspPicture --help` to see Arguments and options.


## Installation: 

### Visual studio code
  - Download and install visual studio code on your repspective OS
 ##### Recommended extensions for visual studio (Not specifically needed for ESP but nice to have) 
 - vscode-icons
 - Gitlens
 -Code Spell Checker
 - change-case
 - Bracket Pair Colorizer
 - C/C++ IntelliSense
 
 #### Install ESP extension 
  - Install Prerequisites: [Link](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-setup.html)
  - Cmake help see [Here](#cmake-installation-linux/unix) 
  
  
  #### Cmake installation Linux/Unix 
   - Download latets version on https://cmake.org/
   - Unpack the compressed folder
   - Open folder in terminal 
   - Run the following commands: 
     > ./bootstrap<br>make<br>make install
