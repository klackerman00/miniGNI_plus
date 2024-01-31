# miniGNI+ - An update to the original miniGNI 
Welcome to the miniGNI+, an extension of the original [miniature giant nuclei impactor (mini-GNI)](https://github.com/nugentlab/miniGNI) created at the University of Hawai'i by the [Nugent Lab](http://alisonnugent.com/). More information on the original instrument can be found in this paper: 

> A New Instrument for Determining the Coarse-Mode Sea Salt Aerosol Size Distribution - [Taing et al., 2021](https://journals.ametsoc.org/view/journals/atot/38/11/JTECH-D-20-0197.1.xml)

while the microscope methods are explained in this paper: 

> The Giant Nucleus Impactor (GNI)—A System for the Impaction and Automated Optical Sizing of Giant Aerosol Particles with Emphasis on Sea Salt. Part I: Basic Instrument and Algorithms - [Jensen et al., 2020](https://journals.ametsoc.org/view/journals/atot/37/9/jtechD190109.xml?rskey=7VVZ45&result=6)

Additional information about the observations gathered by the miniGNI V1.0 are in this corresponding paper: 

> Mechanisms controlling giant sea salt aerosol size distributions along a tropical orographic coastline - [Ackerman et al., 2023](https://acp.copernicus.org/articles/23/13735/2023/)

## Updates from the miniGNIv1.0 to the miniGNI+  
### The Door  
The door was removed from the miniGNIv1 to the miniGNI+ and replaced with an encapsulating cover. This cover offers minimal resistance against the micro-servo motor resulting in fewer failures of the door closure system (a common issue with version 1). For over 100 collected samples, the miniGNI+ closure system has never failed. Additionally, this new closure ensures no airflow around the polycarbonate slide during transport, offering increased protection from contamination over the prior model. 

### The RH Sensor
The relative humidity sensor of miniGNIv1 utilized Adafruits DHT22/2301 sensor but has been replaced with Adafruits 2301B. The 2301B sensor is practically the same, however, utilizes an I<sup>2<\sup>C interface! This results in some minor changes to the wiring. 

### Detachable Wing
The wing of the miniGNI+ clips on for easy storage of the instruments. Several STL files are included at various lengths. 

### Anemometer
The largest upgrade to the miniGNI+ is the addition of an in-situ anemometer. A computer fan has been converted to an anemometer to allow for in-situ wind measurements, rather than utilizing a ground station with a wing power law conversion to calculate wind aloft. 

# In-situ Instrumentation: 
- Temperature (+/- 0.3 \deg C)
- Relative Humidity (+/- 2%)
- Pressure/Altimetry (+/- 1.5 Pa, ~0.3 m)
- Wind Speed (+/- 0.5 m s<sup>-1<\sup>
- Sea Salt Aerosol Size Distribution 

### Optional Add-On
- Orientation (Euler Angles)

# Fritzing Diagram 
![Fritzing Diagram.](https://github.com/klackerman00/miniGNI_plus/blob/main/miniGNIplus_fritzing.png)


