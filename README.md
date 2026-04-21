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
<img src = "https://github.com/klackerman00/miniGNI_plus/blob/main/parts.png" width="350px" height="500px">
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
- Computer Fan (NF-A4x10 FLX Computer Fan) - referenece (https://leemangeophysicalllc.github.io/mini-GNI/fan_mods.html)

### Hardware: 
- Clevis Pin (3/16 x 5/8 inches)
- Clevis Pin #2 (3/16 x 1 inch)
- Arm Spring (McMaster Carr)
- Wire for soldering together these parts
  
# Fritzing Diagram 

Connect the above parts using the following electrical diagram. 
![Fritzing Diagram.]([https://github.com/klackerman00/miniGNI_plus/blob/main/miniGNIplus_diagram_bb.png])


# Base Station 
The base station can be made in accordance with the original base station found in [this document](https://github.com/nugentlab/miniGNI/blob/master/miniGNI_Assembly/DocumentingAssembly.docx) on the original miniGNI page. Ensure that the frequency set on your miniGNI+ and the matching base station are the same! 
