# ESPCAM-LicensePlate


#### Notes:
- When using SD-Card remember to format the card to fat32. 
- If you are using a AI-Tinker board remember to enable PSRAM - Found in menuconfig->components->esp32-specific
- When using tools you need to create a virtualenv. 
    1. When created source the activate script to enter the venv.
    2. To start the script you need to add folder tools to pythonpath. 
        1. run `PYTHONPATH=$PYTHONPATH:pwd python tools/toolEspPicture.py`
        2. To see options for tool use _--help_


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
