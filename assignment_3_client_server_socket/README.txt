CONTENTS OF THIS FILE
---------------------

 * INTRODUCTION
 * HOW TO USE
 * SUPPORTED MATH EXPRESSION FORMAT
 * SERVER COMPARISION


INTRODUCTION
------------

Made by: Hriday Kondru (B20CS021)

There are four programs provided in the folder with file names: client.c, server1.c, server2.c,  server3.c. 

server1.c is a single process server that can handle only one client at a time. The listening socket is closed until the current client disconnects from the server, i.e. only one client can connect with the server at a time.

server2.c is a multi-process server that forks a process whenever it receives a new client request. The child process uses the new socket created for communication with the client while the parent continues listening for new connection requests on the listening socket, i.e. for each client a child server process is created which handles the requests. Here more than one client can work simultaneously.

server3.c is a single process (multi-threaded) server that uses select system call to handle multiple clients concurrently. All the sockets are checked whether there is any communication from the client while also continuing to listen for new connection requests on the listening socket. Here more than one client can work simultaneously. This server supports a maximum of 20 clients at once. This limit can easily be changed by changinging a single value in the code.

HOW TO USE
------------

Compile the .c files using the gcc compiler.
First run the server program in one terminal with port number as the arguement.
Then run each of the client programs in separate terminals with server ip-address and server port number as the arguements.
Type the mathematical expression in the client program, and the output will be shown in both the client prrogram and server program.
The output is a float value.
To terminate the client, use CTRL+C. The server closes the current socket used for communicating with the client, and continues listening for any new clients.
To terminate the server, use CTRL+C.

SUPPORTED MATH EXPRESSION FORMAT
----------------------------------
<int> + <int> 
<int> - <int>
<int> * <int>
<int> / <int>

Note:->The spaces between the operands and operations are not compulsory.
     ->Only two operands are supported. 

SERVER COMPARISION
--------------------
server1 can support only one client at a time. Once the current client disconnects, then the server will start listening for requests to connect from new clients.

server2 supports multiple clients at a time and assignes a separate process for each client. This ensures concurrency of operation of clients but creating a new process has more overhead than creating a thread as all the contents of the parent process are copied into the child process which is now an independent process. Also context switching between processes is slower than compared to context switching between threads.

server3 supports multiple clients at a time and does not create a new process foor each client. This ensures concurrency and is faster than server2 as creating processes has an overhead. Thus, server3 is faster than server2.
