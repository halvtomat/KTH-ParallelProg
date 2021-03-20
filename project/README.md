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

## Performance Tests

Execution times from different runs are found in the table below.

In these tests the parallel simulators used 4 workers (threads) and the barnes hut variants had the *far* cutoff set to 0.5.
The tests were run with *n* bodies and ran for 75000 iterations each.

| *n* | seq_slow | para_slow | seq_fast | para_fast |
|-----|----------|-----------|----------|-----------|
| 120 | 14,7973  | 7,6025    | 0,802614 | 0,652334  |
| 180 | 33,0113  | 16,2822   | 1,23728  | 0,946102  |
| 240 | 58,3416  | 28,4619   | 1,6818   | 1,26323   |

Further, the speedup of the parallel simulators was tested, the table below displays the execution times of the para_slow program with workers ranging from 1-4.

| *n* |    1    |    2    |    3    |    4    |
|-----|---------|---------|---------|---------|
| 120 | 14,6127 | 11,4769 | 8,7737  | 7,6025  |
| 180 | 32,3998 | 25,2829 | 19,397  | 16,2822 |
| 240 | 57,2053 | 44,328  | 34,006  | 28,4619 |

The table below displays the execution times of the para_fast program with workers ranging from 1-4.

| *n* |    1    |    2     |    3     |    4     |
|-----|---------|----------|----------|----------|
| 120 | 0,80037 | 0,719045 | 0,665415 | 0,652334 |
| 180 | 1,26338 | 1,04546  | 0,971178 | 0,946102 |
| 240 | 1,71769 | 1,38587  | 1,26489  | 1,26323  |

### Results Discussion
The results show what was expected from the different programs with the Barnes-Hut algorithm being the most important performance upgrade. The brute force approach is running with a complexity of O(N^2) while the Barnes-Hut algorithm has a complexity of O(NlogN) which is significantly faster.

![Performance chart](/images/performance.png)

The parallelization speedup is less, but still significant, especially when using the brute force algorithm. 

![para_slow speedup chart](/images/para_slow.png)

The parallelization of the Barnes-Hut algorithm is less significant because a bigger part of the algorithm is hard to parallelize. Only the force calculation part of the program is parallel in this version, building the tree and movies the bodies is still run in sequens.
![para_fast speedup chart](/images/para_fast.png)


## Conclusions
This project has been a lot of fun! The biggest challenge (by far) was getting the Barnes-Hut quad-tree to build correctly, but when it was working correctly it paid of, the speedup is amazing. It is really satisfying to watch the particles make little orbits and swirve around.