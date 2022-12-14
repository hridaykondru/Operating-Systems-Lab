CONTENTS OF THIS FILE
---------------------

 * NOTE
 * INTRODUCTION
 * HOW TO USE
 * SUPPORTED MATH EXPRESSION FORMAT
 * Result after running RR scheduler
 * Result after running PRR scheduler
 * SCHEDULER COMPARISION

NOTE
------------
Please ensure this program is being run in a linux environment with g++ compiler installed.

The generator requires xterm to be installed. Use sudo apt install xterm

The scheduler.cpp needs to be compiled using this command in the terminal: g++ scheduler.cpp -o scheduler -lpthread
The parameters for the processes have been changed from what was given in the question taking into consideration the amount of time taken to execute the processes.

The new parameters for CPU bound processes is: Priority: 1-5, Rounds: 2000, I/O(sleep) prob: 0.01, I/O(sleep) time: 1sec

The new parameters for I/O bound processes is: Priority: 6-10, Rounds: 1000, I/O(sleep) prob: 0.1, I/O(sleep) time: 2sec

Each iteration of a process takes 10 milliseconds(ms). Which means total CPU burst time for:
    -> CPU bound processes must be 10*2000 ms = 20 seconds(s)
    -> I/O bound processes must be 10*1000 ms = 10 seconds(s)

All the times were calculated in milliseconds which were then converted to seconds while writing to the result files. Hence, there may be inaccuracies of around 1-2 seconds.

After execution of the scheduler and processes complete, a result file named result_RR.txt or result_PRR.txt is created depending on the scheduler algorithm given as the arguement to the scheduler. A single result.txt file is not created as asked in the question to enable the user to view the results from both the algorithms side-by-side after executing both the schedulers for better comparision.

INTRODUCTION
------------
Made by: Hriday Kondru (B20CS021)

There are three .cpp programs provided in the folder with file names: scheduler.cpp, process.cpp, generator.cpp.

scheduler.cpp is the scheduler that can perform scheduling using Round Robin and Priority Based Round Robin.
To compile the file use: g++ scheduler.cpp -o scheduler -lpthread

process.c is the process that takes the parameters in the order: <rounds> <priority> <sleep_prob> <sleep_time>.
To compile the file use: g++ process.cpp -o process

generator.cpp is a program which generates 4 processes in four separate xterm windows; P1 and P2 are CPU bound processes, P3 and P4 are I/O bound processes
To compile the file use: g++ generator.cpp -o generator

HOW TO USE
------------
Compile the .cpp files using the g++ compiler.
First run the scheduler program in one terminal with RR or PRR number as the arguement.
Then run the generator program in a separate terminal. The parameters of the processes will be printed in this terminal.
The generator creates 4 xterms named P1, P2, P3, P4 with a gap of 1sec in between each process creation.
Each program will give a confirmation that they have connected to the scheduler and will start running.
The scheduler also gives confirmation that the process has connected to it along with the process pid and priority.
The scheduler and generator will terminate when all the four processes terminate and the scheduler will create a result file 
named result_RR.txt or result_PRR.txt depending on the scheduler algorithm given as the arguement to the scheduler.

Result after running RR scheduler
----------------------------------
P1 rounds: 2000, priority: 4, I/O probability: 0.01, I/O time: 1
P2 rounds: 2000, priority: 2, I/O probability: 0.01, I/O time: 1
P3 rounds: 1000, priority: 8, I/O probability: 0.1, I/O time: 2
P4 rounds: 1000, priority: 10, I/O probability: 0.1, I/O time: 2
Scheduler used: RR
Note 1: Total waiting time includes time spent on I/O
Note 2: All times are in seconds
P4 79728 Response time: 0
P4 79728 Total waiting time: 221
P4 79728 Turn-around time: 230
P3 79723 Response time: 0
P3 79723 Total waiting time: 228
P3 79723 Turn-around time: 238
P2 79720 Response time: 0
P2 79720 Total waiting time: 24
P2 79720 Turn-around time: 45
P1 79717 Response time: 0
P1 79717 Total waiting time: 31
P1 79717 Turn-around time: 51
Average Response time: 0
Average Total waiting time: 126
Average Turn-around time: 141

Analysis: 
Total Burst time = Turn-around time - Total waiting time
We can see here that for:
    -> P1: Calculated burst time = 51-31 = 20s
    -> P2: Calculated burst time = 45-24 = 21 ~ 20s
    -> P3: Calculated burst time = 238-228 = 10s
    -> P4: Calculated burst time = 230-221 = 9 ~ 10s
All the Response times are 0 because all the processes could get hold of the CPU within 1sec of their arrival. 
This happens due to the high number of I/O operations being done which leads to the CPU being open to accomodating new processes.

Result after running PRR scheduler
----------------------------------
P1 rounds: 2000, priority: 1, I/O probability: 0.01, I/O time: 1
P2 rounds: 2000, priority: 4, I/O probability: 0.01, I/O time: 1
P3 rounds: 1000, priority: 10, I/O probability: 0.1, I/O time: 2
P4 rounds: 1000, priority: 10, I/O probability: 0.1, I/O time: 2
Scheduler used: PRR
Note 1: Total waiting time includes time spent on I/O
Note 2: All times are in seconds
P4 79945 Response time: 0
P4 79945 Total waiting time: 221
P4 79945 Turn-around time: 231
P3 79942 Response time: 0
P3 79942 Total waiting time: 235
P3 79942 Turn-around time: 245
P2 79939 Response time: 0
P2 79939 Total waiting time: 24
P2 79939 Turn-around time: 45
P1 79936 Response time: 0
P1 79936 Total waiting time: 31
P1 79936 Turn-around time: 51
Average Response time: 0
Average Total waiting time: 128
Average Turn-around time: 143

Analysis: 
Total Burst time = Turn-around time - Total waiting time
We can see here that for:
    -> P1: Calculated burst time = 51-31 = 20s
    -> P2: Calculated burst time = 45-24 = 21 ~ 20s
    -> P3: Calculated burst time = 245-235 = 10s
    -> P4: Calculated burst time = 231-221 = 10s

All the Response times are 0 because all the processes could get hold of the CPU within 1sec of their arrival. 
This happens due to the high number of I/O operations being done which leads to the CPU being open to accomodating new processes.

SCHEDULER COMPARISION
------------------------
We can not see much difference between the two algorithms due to the hign number of I/O operations done. 
This disrupts the order of the ready queue by making any process which is already running to go to the waiting state.
When a high priority process goes for I/O, any lower priority process can take over the CPU, which makes the difference between PRR and RR performances reduce.

We can better see the difference when I/O is not present. Below is the reports done for the four processes 
with I/O(sleep) probability as 0.

RR scheduling:
P1 rounds: 2000, priority: 4, I/O probability: 0, I/O time: 1
P2 rounds: 2000, priority: 1, I/O probability: 0, I/O time: 1
P3 rounds: 1000, priority: 8, I/O probability: 0, I/O time: 2
P4 rounds: 1000, priority: 6, I/O probability: 0, I/O time: 2
Scheduler used: RR
Note 1: Total waiting time includes time spent on I/O
Note 2: All times are in seconds
P4 80505 Response time: 27
P4 80505 Total waiting time: 48
P4 80505 Turn-around time: 58
P3 80501 Response time: 18
P3 80501 Total waiting time: 49
P3 80501 Turn-around time: 59
P2 80498 Response time: 9
P2 80498 Total waiting time: 40
P2 80498 Turn-around time: 60
P1 80495 Response time: 0
P1 80495 Total waiting time: 31
P1 80495 Turn-around time: 51
Average Response time: 13
Average Total waiting time: 42
Average Turn-around time: 57

PRR scheduling:
P1 rounds: 2000, priority: 3, I/O probability: 0, I/O time: 1
P2 rounds: 2000, priority: 4, I/O probability: 0, I/O time: 1
P3 rounds: 1000, priority: 7, I/O probability: 0, I/O time: 2
P4 rounds: 1000, priority: 10, I/O probability: 0, I/O time: 2
Scheduler used: PRR
Note 1: Total waiting time includes time spent on I/O
Note 2: All times are in seconds
P4 80895 Response time: 48
P4 80895 Total waiting time: 48
P4 80895 Turn-around time: 58
P3 80892 Response time: 38
P3 80892 Total waiting time: 38
P3 80892 Turn-around time: 49
P2 80889 Response time: 19
P2 80889 Total waiting time: 19
P2 80889 Turn-around time: 39
P1 80886 Response time: 0
P1 80886 Total waiting time: 0
P1 80886 Turn-around time: 20
Average Response time: 26
Average Total waiting time: 26
Average Turn-around time: 42

We can see that for higher priority processes (CPU bound: priority 1-5) we get lesser waiting and turnaround times using PRR scheduling compared to RR scheduling. And, for lower priority processes (I/O bound: priority 6-10) we get higher waiting and turnaround times using PRR scheduling compared to RR scheduling. This makes sense as in PRR scheduling, higher priority processes in the ready queue are scheduled before the lower priority processes in the ready queue and in RR scheduling, priority is completely ignored and any process that is in the front of the ready queue is scheduled next in a FIFO manner. RR scheduling is a like special case of PRR scheduling where the priority of all the processes are assumed to be equal. We can also see here that, the response time is not 0 for all the processes. That means, that not all processes could get hold of the CPU within one second of their arrival.
