# miniGNI+ - An update to the original miniGNI 
Welcome to the miniGNI+, an extension of the original [miniature giant nuclei impactor (mini-GNI)](https://github.com/nugentlab/miniGNI) created at the University of Hawai'i by the [Nugent Lab](http://alisonnugent.com/). This instrument utilizes the same methodology as the original miniGNI with an improved design that facilitates use. Additionally, we've included an in-situ anemometer. More information on the methods of the original instrument can be found in this paper: 

> A New Instrument for Determining the Coarse-Mode Sea Salt Aerosol Size Distribution - [Taing et al., 2021](https://journals.ametsoc.org/view/journals/atot/38/11/JTECH-D-20-0197.1.xml)

while the microscope methods are explained in this paper: 

> The Giant Nucleus Impactor (GNI)—A System for the Impaction and Automated Optical Sizing of Giant Aerosol Particles with Emphasis on Sea Salt. Part I: Basic Instrument and Algorithms - [Jensen et al., 2020](https://journals.ametsoc.org/view/journals/atot/37/9/jtechD190109.xml?rskey=7VVZ45&result=6)

Results using the observations gathered by the miniGNI V1.0 are in this corresponding paper: 

> Mechanisms controlling giant sea salt aerosol size distributions along a tropical orographic coastline - [Ackerman et al., 2023](https://acp.copernicus.org/articles/23/13735/2023/)

<img src="https://github.com/klackerman00/miniGNI_plus/blob/main/miniGNI_plus.jpg" width="350px" height="500px"><img src="https://github.com/klackerman00/miniGNI_plus/blob/main/miniGNI_plus_open.jpg" width="375px" height="500px">

## Updates from the miniGNIv1.0 to the miniGNI+  
### The Door  
The door was removed from the miniGNIv1 to the miniGNI+ and replaced with an encapsulating cover. This cover offers minimal resistance against the micro-servo motor resulting in fewer failures of the door closure system (a common issue with version 1). For over 100 collected samples, the miniGNI+ closure system has never failed. Additionally, this new closure ensures no airflow around the polycarbonate slide during transport, offering increased protection from contamination over the prior model. 

### The RH Sensor
The relative humidity sensor of miniGNIv1 utilized Adafruits DHT22/2301 sensor but has been replaced with Adafruits 2301B. The 2301B sensor is practically the same, however, utilizes an I<sup>2</sup>C interface! This results in some minor changes to the wiring. 

### Detachable Wing
The wing of the miniGNI+ clips on for easy storage of the instruments. Several STL files are included at various lengths. 

### Anemometer
The largest upgrade to the miniGNI+ is the addition of an in-situ anemometer. A computer fan has been converted to an anemometer to allow for in-situ wind measurements, rather than utilizing a ground station with a wing power law conversion to calculate wind aloft. 

# In-situ Instrumentation: 
- Temperature (+/- 0.3 &deg;C)
- Relative Humidity (+/- 2%)
- Pressure/Altimetry (+/- 1.5 Pa, ~0.3 m)
- Wind Speed (+/- 0.5 m s<sup>-1</sup>)
- Sea Salt Aerosol Size Distribution 

### Optional Add-On
- Orientation (Euler Angles)

# Assembly
<img src = "https://github.com/klackerman00/miniGNI_plus/blob/main/parts.png" width="500px" height="500px">
Gather the parts necessary for assembly. You'll need: 

### 3D Printed Parts: 
- Body (big purple body)
- Arm (small skinny)
- Anemometer house (square with circular cutout)

### Electronics: 
- Male to male pins; at least 16 continuous pieces long (less if you want to make your life harder)
- Feather M0 RFM9x LoRA radio sensor (with lipo battery and micro-usb ports)
- Adalogger Featherwing (with micro SD and CR1220 watch battery ports)
- Relative humidity sensor (black, Adafruit AM2301B)
- Altimeter (Adafruit, MPL3115A2)
- Resistor (Digikey, 10k ohm)
- Microserver motor (Adafruit, P169)
- MicroSD card
- Computer Fan (NF-A4x10 FLX Computer Fan) - [Reference to Leeman Geophysical, LLC](https://leemangeophysicalllc.github.io/mini-GNI/fan_mods.html)
- Wire for soldering parts together
- Antenna wire (specifically cut to 8.3 cm) 

### Hardware: 
- Clevis Pin (3/16 x 5/8 inches)
- Clevis Pin #2 (3/16 x 1 inch)
- Arm Spring (McMaster Carr)
- Wire for soldering together these parts
  
# Fritzing Diagram 
<img src = "https://github.com/klackerman00/miniGNI_plus/blob/main/miniGNIplus_diagram_bb.png" width="500px" height="500px">
Connect the above parts using the following electrical diagram. 

# Base Station 
The base station can be made in accordance with the original base station found in [this document](https://github.com/nugentlab/miniGNI/blob/master/miniGNI_Assembly/DocumentingAssembly.docx) on the original miniGNI page. Ensure that the frequency set on your miniGNI+ and the matching base station are the same! 

# Arduino IDE Software Upload
We're currently on the V2 of the miniGNI+! Please use this updated source code that accounts for the change in relative humidity sensor from the original analog sensor to the new I2C sensor. Both code for the miniGNI and miniGNI base station are provided. 

We utilized some outdated versions of specific libraries (which we provide below). We cannot guarantee that these will work with updated libraries. 

### Libraries in use and version numbers: 
- Ethernet by Various (2.0.0)
- Firmata by Firmata (2.5.8)
- Keyboard by Arduino (1.0.2)
- LiquidCrystal by Arduino, Adafruit (1.0.7)
- Mouse by Arduino (1.0.1)
- SD by Arduino, SparkFun (1.2.4)
- Servo by Michael (1.1.7)
- Stepper by Arduino (1.1.3)
- Temboo by Temboo (1.2.1)
- Adafruit AHTX0 by Adafruit (2.0.5)
- Adafruit BNO055 by Adafruit (1.4.3)
- Adafruit BusIO by Adafruit (1.7.2)
- Adafruit Circuit Playground by Adafruit (1.10.4)
- Adafruit GFX Library by Adafruit (1.10.6)
- Adafruit ILI9341 by Adafruit (1.5.6)
- Adafruit MPL3115A2 by Adafruit (1.2.1)
- Adafruit STMPE610 by Adafruit (1.1.3)
- Adafruit TouchScreen by Adafruit (1.1.1)
- Adafruit Unified Sensor by Adafruit (1.1.4)
- DHT sensor Library by Adafruit (1.4.1)
- FIR filter by Leeman Geophysical LLC by Adafruit (0.1.1)
- RF24 by TMRh20 (1.3.11)
- RTClib by Adafruit (1.12.5)
- RadioHead by Mike McCauley (1.112)
- TinyWireM by Adafruit (1.1.0)
- Bridge by Arduino (1.7.0)
- Esplora by Arduino (1.0.4)
- GSM by Arduino (1.0.6)
- Robot Control by Arduino (1.0.4)
- Robot IR Remote by Arduino (2.0.0)
- Robot Motor by Arduino (1.0.3)
- SpacebrewYun by Julio Terra (1.0.2)
- TFT by Arduino, Adafruit (1.0.6)
- WiFI by Arduino (1.2.7)

### Specific Boards to Download: 
- Adafruit SAMD Boards (1.7.17)

This can be added by going to the Arduino IDE in the top right corner > Settings, and then adding this link to the "Additional Boards Manager URL" 
https://adafruit.github.io/arduino-board-index/package_adafruit_index.json


