CONTENTS OF THIS FILE
---------------------
 * INTRODUCTION 
 * HOW TO RUN sleeping_barber.cpp 
 * HOW TO RUN bankers_algorithm.cpp

INTRODUCTION 
-------------
Made by: Hriday Kondru (B20CS021)

There are 2 .c programs named bankers_algorithm.cpp and sleeping_barber.cpp provided in the folder.

sleeping_barber.cpp is the sleeping barber's problem solution using Monitors.

bankers_algorithm.cpp is the implementation of Banker's algorithm for deadlock avoidance.

NOTE: These are c++ files. An appropriate compiler needs to be used to compile these files.

HOW TO RUN sleeping_barber.cpp 
--------------------------------
Enter the number of customers and the number of chairs. The simulation is done and appropriate description statements are printed at every step.
After all the customers leave, the customer falls asleep waiting for other new customers. The program needs to be manually terminated then, since no more customers are going to come.

HOW TO RUN bankers_algorithm.cpp
---------------------------------
Enter the necessary information as prompted by the program. The Banker's algorithm runs on the current state and checks whether thhe state is safe.
If the current state is safe, the program validates if the new request made can be allocated using Banker's algorithm on the updated state again.
