CONTENTS OF THIS FILE
---------------------

 * NOTE
 * INTRODUCTION
 * HOW TO USE 
 * SAMPLE OUTPUT
 * BELADY'S ANOMALY EXPLAINATION

NOTE
------------
Please ensure this program is being run in a linux environment with g++ compiler installed.

After execution of the memory_manager  a result file named result.txt is created which contains all the output displayed in the terminal window during execution.

INTRODUCTION
---------------
Made by: Hriday Kondru (B20CS021)

There one .cpp program named memory_manager.cpp provided in the folder.

memory_manager.cpp is the memory manager that calculates the number of page faults of processes using FIFO, LRU and Optimal.

HOW TO USE
------------
Compile the .cpp file using the g++ compiler.
Enter the inputs as directed by the program. The program allows to enter different reference strings every time the number of 
allocated frames for the process increases(in the loop from 1 to the entered maximum number of frames).
A result file named result.txt is created which contains all the output displayed in the terminal window during execution.

SAMPLE OUTPUT
---------------

Getting page faults for 1 number of frames allocated to the process
With Reference String:
0 1 2 3 0 1 4 0 1 2 3 4 
Page faults using FIFO algorithm: 12
Page faults using LRU algorithm: 12
Page faults using Optimal algorithm: 12

Getting page faults for 2 number of frames allocated to the process
With Reference String:
0 1 2 3 0 1 4 0 1 2 3 4 
Page faults using FIFO algorithm: 12
Page faults using LRU algorithm: 12
Page faults using Optimal algorithm: 9

Getting page faults for 3 number of frames allocated to the process
With Reference String:
0 1 2 3 0 1 4 0 1 2 3 4 
Page faults using FIFO algorithm: 9
Page faults using LRU algorithm: 10
Page faults using Optimal algorithm: 7

Getting page faults for 4 number of frames allocated to the process
With Reference String:
0 1 2 3 0 1 4 0 1 2 3 4 
Page faults using FIFO algorithm: 10
Page faults using LRU algorithm: 8
Page faults using Optimal algorithm: 6

BELADY'S ANOMALY EXPLAINATION
---------------------------------

From the sample output given above, we can see that the number of page faults increases from 9 to 10 when the number of allocated frames
increases from 3 to 4 when the reference string is 0 1 2 3 0 1 4 0 1 2 3 4 for FIFO.