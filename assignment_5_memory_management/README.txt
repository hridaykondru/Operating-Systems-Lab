CONTENTS OF THIS FILE
---------------------

 * NOTE
 * INTRODUCTION & OUTPUT EXPLAINATON
 * HOW TO USE 
 * SAMPLE OUTPUT

NOTE
------------
Please ensure this program is being run in a linux environment with g++ compiler installed.

The memory_manager.cpp needs to be compiled using this command in the terminal: g++ memory_manager.cpp -o memory_manager

To run the memory manager use this command in the terminal along with the following arguements: ./memory_manager <Total no. of processes(k)> <max pages required per process(m)> <total number of frames in main memory(f)> <size of TLB(s)>
Where, m>f and f>s.

After execution of the memory_manager  a result file named result.txt is created which contains all the output displayed in the terminal window during execution.

It is assumed that one process completes execution befor the start of execution of another process (this is as per the requirements given in the question).

INTRODUCTION & OUTPUT EXPLAINATON
----------------------------------
Made by: Hriday Kondru (B20CS021)

There one .cpp program named memory_manager.cpp provided in the folder.

memory_manager.cpp is the memory manager that can performs the memory management of processes. The processes are simulated by the memory_manager.

The processes are given a random unique pid between (1,k)

The processes are randomly assigned their required number of pages (mi) between (1,m).

The processes are allocated number of frames in the main memory proportionately to their requirement of pages using the formula: (mi/M)*f where M is the summation of mi for all processes.

For each scheduled process, a reference string of length lying in the range (2*mi,10*mi) is generated.

For each process, the page numbers are generated in the range (1,mi)

THe frame numbers are in the range (1,f)

For each logical page reference the various steps are:
    ->the TLB is checked
        ->HIT: OUTPUT: "TLB hit: Page <page no.> Frame <frame no.>"
        ->MISS: page table is checked next
            ->NO PAGE FAULT: OUTPUT: "TLB miss with no page fault on page: <page no.> frame: <frame no.>"
                ->TLB updated
                    ->TLB size = s (TLB full): LRU used to replace an entry from TLB
                    ->TLB size < s( Space available): Entry added to TLB
            ->PAGE FAULT: OUTPUT: "TLB miss with page fault on page: <page no.>"
                ->Valid and free frame available: Frame assigned to page in page table, OUTPUT: "Page <page no.> now assigned to frame <frame no.>"
                    ->TLB updated
                        ->TLB size = s (TLB full): LRU used to replace an entry from TLB
                        ->TLB size < s (Space available): Entry added to TLB
                ->No frame available: LRU used to get a frame by doing local page replacement and assign it to the page in the page table, OUTPUT: "Page <page no.> now assigned to frame <frame no.>"
                    ->TLB updated
                        ->TLB size = s (TLB full): LRU used to replace an entry from TLB
                        ->TLB size < s (Space available): Entry added to TLB

After each process completes execution, the number of page faults are shown in the format "P<pid>: Page fault count= <page fault count>"

HOW TO USE
------------
Compile the .cpp file using the g++ compiler.
Run using the command with necessary arguements: ./memory_manager <Total no. of processes(k)> <max pages required per process(m)> <total number of frames in main memory(f)> <size of TLB(s)>
A result file named result.txt is created which contains all the output displayed in the terminal window during execution.

SAMPLE OUTPUT
---------------
This output is for the following input: ./memory_manager 3 10 7 6

Allocating 2 frames for process with pid 3 which needs 6 pages totally
Allocating 1 frames for process with pid 1 which needs 4 pages totally
Allocating 2 frames for process with pid 2 which needs 6 pages totally

P3: Start of execution
Reference string: 1 4 2 3 2 3 2 6 5 1 1 5 5 6 3 4 4 3 3 3 2 2 2 6 1 1 1 6 4 2 5 2 5 4 4 4 6 3 2 3 3 6 1 6 5 2 3 3 4 3 3 5 2 4 4 3 4 3 
TLB miss with page fault on page: 1
Page 1 now assigned to frame 1
TLB miss with page fault on page: 4
Page 4 now assigned to frame 2
TLB miss with page fault on page: 2
Page 2 now assigned to frame 1
TLB miss with page fault on page: 3
Page 3 now assigned to frame 2
TLB hit: Page 2 Frame 1
TLB hit: Page 3 Frame 2
TLB hit: Page 2 Frame 1
TLB miss with page fault on page: 6
Page 6 now assigned to frame 2
TLB miss with page fault on page: 5
Page 5 now assigned to frame 1
TLB miss with page fault on page: 1
Page 1 now assigned to frame 2
TLB hit: Page 1 Frame 2
TLB hit: Page 5 Frame 1
TLB hit: Page 5 Frame 1
TLB miss with page fault on page: 6
Page 6 now assigned to frame 2
TLB miss with page fault on page: 3
Page 3 now assigned to frame 1
TLB miss with page fault on page: 4
Page 4 now assigned to frame 2
TLB hit: Page 4 Frame 2
TLB hit: Page 3 Frame 1
TLB hit: Page 3 Frame 1
TLB hit: Page 3 Frame 1
TLB miss with page fault on page: 2
Page 2 now assigned to frame 2
TLB hit: Page 2 Frame 2
TLB hit: Page 2 Frame 2
TLB miss with page fault on page: 6
Page 6 now assigned to frame 1
TLB miss with page fault on page: 1
Page 1 now assigned to frame 2
TLB hit: Page 1 Frame 2
TLB hit: Page 1 Frame 2
TLB hit: Page 6 Frame 1
TLB miss with page fault on page: 4
Page 4 now assigned to frame 2
TLB miss with page fault on page: 2
Page 2 now assigned to frame 1
TLB miss with page fault on page: 5
Page 5 now assigned to frame 2
TLB hit: Page 2 Frame 1
TLB hit: Page 5 Frame 2
TLB miss with page fault on page: 4
Page 4 now assigned to frame 1
TLB hit: Page 4 Frame 1
TLB hit: Page 4 Frame 1
TLB miss with page fault on page: 6
Page 6 now assigned to frame 2
TLB miss with page fault on page: 3
Page 3 now assigned to frame 1
TLB miss with page fault on page: 2
Page 2 now assigned to frame 2
TLB hit: Page 3 Frame 1
TLB hit: Page 3 Frame 1
TLB miss with page fault on page: 6
Page 6 now assigned to frame 2
TLB miss with page fault on page: 1
Page 1 now assigned to frame 1
TLB hit: Page 6 Frame 2
TLB miss with page fault on page: 5
Page 5 now assigned to frame 1
TLB miss with page fault on page: 2
Page 2 now assigned to frame 2
TLB miss with page fault on page: 3
Page 3 now assigned to frame 1
TLB hit: Page 3 Frame 1
TLB miss with page fault on page: 4
Page 4 now assigned to frame 2
TLB hit: Page 3 Frame 1
TLB hit: Page 3 Frame 1
TLB miss with page fault on page: 5
Page 5 now assigned to frame 2
TLB miss with page fault on page: 2
Page 2 now assigned to frame 1
TLB miss with page fault on page: 4
Page 4 now assigned to frame 2
TLB hit: Page 4 Frame 2
TLB miss with page fault on page: 3
Page 3 now assigned to frame 1
TLB hit: Page 4 Frame 2
TLB hit: Page 3 Frame 1
P3 : Page fault count= 30
P3: End of execution

P1: Start of execution
Reference string: 3 2 2 2 1 4 3 2 3 4 3 1 4 3 4 1 1 3 1 1 4 4 3 4 1 1 1 1 4 1 3 3 3 
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 2
Page 2 now assigned to frame 3
TLB hit: Page 2 Frame 3
TLB hit: Page 2 Frame 3
TLB miss with page fault on page: 1
Page 1 now assigned to frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 2
Page 2 now assigned to frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 1
Page 1 now assigned to frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB miss with page fault on page: 1
Page 1 now assigned to frame 3
TLB hit: Page 1 Frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 1
Page 1 now assigned to frame 3
TLB hit: Page 1 Frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB hit: Page 4 Frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB miss with page fault on page: 1
Page 1 now assigned to frame 3
TLB hit: Page 1 Frame 3
TLB hit: Page 1 Frame 3
TLB hit: Page 1 Frame 3
TLB miss with page fault on page: 4
Page 4 now assigned to frame 3
TLB miss with page fault on page: 1
Page 1 now assigned to frame 3
TLB miss with page fault on page: 3
Page 3 now assigned to frame 3
TLB hit: Page 3 Frame 3
TLB hit: Page 3 Frame 3
P1 : Page fault count= 23
P1: End of execution

P2: Start of execution
Reference string: 4 5 3 5 6 4 6 3 4 1 5 3 2 3 3 2 4 6 5 6 5 4 4 2 4 6 4 6 4 2 4 5 6 4 1 6 5 6 6 3 5 4 3 6 4 6 1 1 5 6 5 4 1 6 3 2 3 4 2 6 6 5 3 3 2 3 2 5 3 5 1 
TLB miss with page fault on page: 4
Page 4 now assigned to frame 4
TLB miss with page fault on page: 5
Page 5 now assigned to frame 5
TLB miss with page fault on page: 3
Page 3 now assigned to frame 4
TLB hit: Page 5 Frame 5
TLB miss with page fault on page: 6
Page 6 now assigned to frame 4
TLB miss with page fault on page: 4
Page 4 now assigned to frame 5
TLB hit: Page 6 Frame 4
TLB miss with page fault on page: 3
Page 3 now assigned to frame 5
TLB miss with page fault on page: 4
Page 4 now assigned to frame 4
TLB miss with page fault on page: 1
Page 1 now assigned to frame 5
TLB miss with page fault on page: 5
Page 5 now assigned to frame 4
TLB miss with page fault on page: 3
Page 3 now assigned to frame 5
TLB miss with page fault on page: 2
Page 2 now assigned to frame 4
TLB hit: Page 3 Frame 5
TLB hit: Page 3 Frame 5
TLB hit: Page 2 Frame 4
TLB miss with page fault on page: 4
Page 4 now assigned to frame 5
TLB miss with page fault on page: 6
Page 6 now assigned to frame 4
TLB miss with page fault on page: 5
Page 5 now assigned to frame 5
TLB hit: Page 6 Frame 4
TLB hit: Page 5 Frame 5
TLB miss with page fault on page: 4
Page 4 now assigned to frame 4
TLB hit: Page 4 Frame 4
TLB miss with page fault on page: 2
Page 2 now assigned to frame 5
TLB hit: Page 4 Frame 4
TLB miss with page fault on page: 6
Page 6 now assigned to frame 5
TLB hit: Page 4 Frame 4
TLB hit: Page 6 Frame 5
TLB hit: Page 4 Frame 4
TLB miss with page fault on page: 2
Page 2 now assigned to frame 5
TLB hit: Page 4 Frame 4
TLB miss with page fault on page: 5
Page 5 now assigned to frame 5
TLB miss with page fault on page: 6
Page 6 now assigned to frame 4
TLB miss with page fault on page: 4
Page 4 now assigned to frame 5
TLB miss with page fault on page: 1
Page 1 now assigned to frame 4
TLB miss with page fault on page: 6
Page 6 now assigned to frame 5
TLB miss with page fault on page: 5
Page 5 now assigned to frame 4
TLB hit: Page 6 Frame 5
TLB hit: Page 6 Frame 5
TLB miss with page fault on page: 3
Page 3 now assigned to frame 4
TLB miss with page fault on page: 5
Page 5 now assigned to frame 5
TLB miss with page fault on page: 4
Page 4 now assigned to frame 4
TLB miss with page fault on page: 3
Page 3 now assigned to frame 5
TLB miss with page fault on page: 6
Page 6 now assigned to frame 4
TLB miss with page fault on page: 4
Page 4 now assigned to frame 5
TLB hit: Page 6 Frame 4
TLB miss with page fault on page: 1
Page 1 now assigned to frame 5
TLB hit: Page 1 Frame 5
TLB miss with page fault on page: 5
Page 5 now assigned to frame 4
TLB miss with page fault on page: 6
Page 6 now assigned to frame 5
TLB hit: Page 5 Frame 4
TLB miss with page fault on page: 4
Page 4 now assigned to frame 5
TLB miss with page fault on page: 1
Page 1 now assigned to frame 4
TLB miss with page fault on page: 6
Page 6 now assigned to frame 5
TLB miss with page fault on page: 3
Page 3 now assigned to frame 4
TLB miss with page fault on page: 2
Page 2 now assigned to frame 5
TLB hit: Page 3 Frame 4
TLB miss with page fault on page: 4
Page 4 now assigned to frame 5
TLB miss with page fault on page: 2
Page 2 now assigned to frame 4
TLB miss with page fault on page: 6
Page 6 now assigned to frame 5
TLB hit: Page 6 Frame 5
TLB miss with page fault on page: 5
Page 5 now assigned to frame 4
TLB miss with page fault on page: 3
Page 3 now assigned to frame 5
TLB hit: Page 3 Frame 5
TLB miss with page fault on page: 2
Page 2 now assigned to frame 4
TLB hit: Page 3 Frame 5
TLB hit: Page 2 Frame 4
TLB miss with page fault on page: 5
Page 5 now assigned to frame 5
TLB miss with page fault on page: 3
Page 3 now assigned to frame 4
TLB hit: Page 5 Frame 5
TLB miss with page fault on page: 1
Page 1 now assigned to frame 4
P2 : Page fault count= 47
P2: End of execution