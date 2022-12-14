CONTENTS OF THIS FILE
---------------------

 * INTRODUCTION 
 * no_deadlock.c Explaination
 * with_deadlock.c Explaination
 * reader_writer_no_starvation.c Explaination

INTRODUCTION 
-------------
Made by: Hriday Kondru (B20CS021)

There are 3 .c programs named no_deadlock.c, with_deadlock.c and reader_writer_no_starvation.c provided in the folder.

no_deadlock.c is the dining philosophers' problem solution with no deadlock occurance.

with_deadlock.c is the dining philosophers' problem solution with deadlock recovery mechanism after deadlock occurance.

reader_writer_no_starvation.c is the reader-writer's problem solution with no starvation of the writer.

NOTE: All the simulations are done using threads.

no_deadlock.c Explaination
---------------------------
Each chopstick(or fork) has a binary semaphore. A philosopher only picks the chopsticks if both the chopsticks are available.
For a philosopher numbered i, the chopsticks that can be used are numbered i and (i+1)%5 (the total number of philosophers are 5).

with_deadlock.c Explaination
-----------------------------
Each chopstick(or fork) has a binary semaphore. Every philosopher i picks the chopstick i first, and then the chopstick (i+1)%5.
There is a possibility of deadlock if all the philosophers first pick the ith chopstick. This state is detected using another thread 
which then preempts the first philosopher. Thus, the deadlock is broken.

reader_writer_no_starvation.c Explaination
--------------------------------------------
The normal solution to the reader-writer's problem can cause the writer to starve, because as long as a reader is reading, any number of readers
can enter the critical section, which causes the writer to not be able to enter the critical section. 
This modified solution ensures that the writer doesn't starve by adding another layer of a semaphore lock. The sync_lock is used here to allow the 
writer to enter the critical section if it wishes to, and stops further readers from entering the critical section.
