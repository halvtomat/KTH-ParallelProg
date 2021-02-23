# The Bear and the Honeybees Problem (multiple producers - one consumer)
Given are n honeybees and a hungry bear. They share a pot of honey. The pot is initially empty; its capacity is H portions of honey. The bear sleeps until the pot is full, then eats all the honey and goes back to sleep. Each bee repeatedly gathers one portion of honey and puts it in the pot; the bee who fills the pot awakens the bear.

Represent the bear and honeybees as concurrent processes or threads (i.e. a "bear" process and an array of "honeybee" processes), and the honey pot as a critical resource (a monitor) that can be accessed by at most one process at a time (either by the "bear" process or by one of the "honeybee" processes). 

Develop a monitor (with condition variables) to synchronize the actions of the bear and honeybees, i.e. develop a monitor that represents the pot of honey. Define the monitor's operations and their implementation. Implement a multithreaded application in Java or C++ to simulate the actions of the bear and honeybees represented as concurrent threads and the pot represented as as the monitor. Is your solution fair? Explain in comments in the source code.

## How to run
Run:

	make
to create the executable, then run:

	bees x y
to run the program, x is how much honey the bees need to gather before the bear eats it and y is how many bees there are, these are required arguments.
