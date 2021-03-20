# N Body Simulator
This project is a N body simulator which simulates the gravitational forces that *N* bodies have on each other.

## Running the simulation
The project is compiled by running:
   
    make
To run, choose one of the executables and run the following:

    ./<EXECUTABLE> <OPTIONS>
The options are displayed in the **help** menu, to print the help menu, run:
    
    ./<EXECUTABLE> -h
Example of running the parallelized O(NlogN) barnes hut simulation with 10 000 bodies where body[0] is a god point (Really big mass) for 100 000 steps and drawing it all:

    ./para_fast -b 10000 -n 100000 -d -g

## Software and computing platform
The computer used to make the performance evaluations in this report.

### OS
**Ubuntu 20.04**

### CPU
Model: **Intel Core i7-2600**

Speed: **3.40GHz (Turbo 3.80GHz)**

Cores: **4** 

Threads: **8**

### RAM
Size: **8GiB**

Technology: **DDR3**

Speed: **1333MHz**

### Software
Parallelization library: **OpenMP**

Graphics library: **SDL2**

## Design and Implementation
This project includes four different simulation software.
All of the programs use the same library of functions to execute the simulation, this library can be found in **common.h**. 
The library contains a class *Bodies* and functions to initialize all simulated *bodies*, it also contains functions used in basic force calculations like *distance* which returns the distance between two points. The library also contains functions to read command line arguments.

The *Bodies* class contains data about all simulated *bodies*, their positions, velocities and mass. The class also contains a method to draw the bodies using **SDL2**.

The only real difference in the different simulation software is the way the forces on the bodies are calculated.
Below is a descripiton of the different simulation software.

1. *seq_slow*

This is a sequential brute force approch to the simulation, the forces are calculated by applying the forces two bodies have on each other two by two for all bodies.

2. *para_slow*

This calculates the forces in the same way as *seq_slow* but it is parallelized using **OpenMP**.

3. *seq_fast*

This version is a sequential variant using the **Barnes-Hut** algorithm to approximate some of the forces with a quad-tree datastructure.

4. *para_fast*

The fastest variant, this version uses the Barnes-hut algortihm and is parallelized.

## Performance Evaluation



## Conclusions
This project has been a lot of fun! The biggest challenge (by far) was getting the Barnes-Hut quad-tree to build correctly, but when it was working correctly it paid of, the speedup is amazing. It is really satisfying to watch the particles make little orbits and swirve around.