### UNE Trimester 2, 2023 - COSC530 Parallel and Distributed Computing
# Assignment 2 - Addition and Subtraction with Threads 

* Worth 10% for COSC330, 7% for COSC530
* **An overall mark of at least 50% across across all assessment tasks is required to pass COSC330/530**

## Aims
* Revise some C programming fundamentals such as pointers and system calls.
* Develop a multi-threaded program that uses mutual exclusion and synchronisation to solve a complex problem.
* To explore and implement a system of *semaphores*

## The problem

This assignment is a variation on the readers and writers synchronisation problem. The classic readers and writers problem is that although any number of readers should be able to read shared data at a time, a writer should have exclusive access. This classic problem is elegantly solved by binary semaphores:

```no-highlight
shared semaphore count_lock

shared int readers_count initially 0 
shared semaphore data_lock
shared data

reader algorithm:
lock count_lock 

readers_count++

if( readers_count == 1 )
  lock data_lock

unlock count_lock
read the data
lock count_lock 

readers_count--

if( readers_count == 0 )
  unlock data_lock

unlock count_lock

```

That is, the first **reader** attempting access gains access to the data on behalf of all readers that follow. After reading the data, the last reader out releases access to the data.

```no-highlight
writer algorithm:
lock data_lock
write to the data
unlock data_lock

```

That is, a **writer** gets exclusive access to the data, locking out all other writers and readers.
Using pthreads, the semaphores can be implemented simply as pthread mutexs. The readers and writers will be threads executing different functions.

Now, the ***Addition/Subtraction*** problem:

The variation for this assignment is that there are 2 types of writers: incrementers, who add one to a sum; and decrementers, who deduct one from the sum.

Write a program in C that creates a random number of incrementer, decrementer and reader threads (using the pthreads). The shared data should be a `struct` with fields for the sum, last incrementer id, last decrementer id and number of writers so far. All threads should print their identity and their operation on the sum each time they access the shared data. The final state of the shared data is to be printed to the console.

This is my example:

```no-highlight
Mitchells-iMac:cosc330 mwelch8$ gcc rdrsincsdecs.c -pthread -Wall -o a2
Mitchells-iMac:cosc330 mwelch8$ ./a2
Incrementer 0 set sum = 1
Incrementer 2 set sum = 2
Incrementer 1 set sum = 3
Incrementer 3 set sum = 4
Incrementer 4 set sum = 5
Decrementer 0 set sum = 4
Decrementer 1 set sum = 3
Decrementer 2 set sum = 2
Decrementer 3 set sum = 1
Reader 0 got 1
Reader 1 got 1
Reader 2 got 1
Reader 3 got 1
Reader 4 got 1
Reader 5 got 1
Reader 6 got 1
Reader 7 got 1
Reader 8 got 1
Reader 9 got 1
There were 10 readers, 5 incrementers and 4 decrementers
The final state of the data is:
	last incrementer 4
	last decrementer 3
	total writers 9
	sum 1

```
## Submission

* Your assignment will need to be submitted through the `submit` program on turing.
* Please review the [instructions for the submit program](http://turing.une.edu.au/~cosc330/assignments/submit_instr.html) if you are not familiar with its use.
* Make sure that you record a script of your program compiling and working correctly. 
* Confirm that the file sizes listed in the submission receipt are not 0Kb!

## Tentative Marking Guide

**Solution Correctness - 70%**

* Does your solution compile and run?
* Does your solution produce the correct/expected output?
* Does your solution use the correct access protocol (e.g. Read/Write Locks)?
* Does your solution achieve parallelism using pthreads?
* Does you solution us a `struct` for the shared resourses?
* Does solution create the correct number of threads?

**Quality of Solution - 15%**

***BEWARE! - 5% Will be deducted for each offence (minimum of 0 for this section)***

* Is your code broken down into functions (e.g. not more than about 60 lines - excluding braces, comments and whitespace)
* Have you generated general-purpose/reusable functions?
* Have you grouped related functions into separate libraries?
* Have you included a complete makefile with `clean` and `run` targets? 
* Does your makefile use -Wall -pedantic ?
* Does the code compile without errors/warnings (e.g. with -Wall -pedantic)?
* Is there error checking on all system calls, user inputs or source file content?
* Does your solution take appropriate action if an error occurs (e.g. make every effort to save the situation)?
* Have you avoided the use of hard-coded literals? (e.g. use #define macros)

**Documentation - 10%**

***BEWARE! - 5% Will be deducted for each offence (minimum of 0 for this section)***

* Does your header block contain the author's name, the purpose of the program and a description of how to compile and run the solution.
* Are identifiers named in a meaningful way?
* Are any obscure constructs fully explained?
* Does each function have a header block that explains the purpose, its arguments and return value?
* Have you recorded a submission script (in the `submit` program) showing your assignment compiling and running?

**Source Formatting - 5%**

***BEWARE! - 5% Will be deducted for each offence (minimum of 0 for this section)***

* Is your indentation consistent? Make sure that you use the `indent` utility on all source files.
* Have blank lines been used so that the code is easy to read?
* Are any lines longer than 80 characters? (`indent` can sort this out)
* Are capitalisation and naming conventions consistent?




