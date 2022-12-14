CONTENTS OF THIS FILE
---------------------
 * INTRODUCTION 
 * HOW TO RUN

INTRODUCTION 
-------------
Made by: Hriday Kondru (B20CS021)

There are 2 .cpp programs named FAT.cpp and disk_scheduling.cpp provided in the folder.

NOTE: These are c++ files. An appropriate compiler needs to be used to compile these files.

HOW TO RUN 
------------
For the FAT.cpp program, the commands supported and format are:
    my_open <file_name>
    my_close
    my_read
    my_write
    my_mkdir <dir_name>
    my_chdir <dir_name>
    my_rmdir <dir_name>
    my_copy <file_name in linux in the same directory as the FAT.cpp program>

my_close my_read my_write only when a file is opened using my_open.
my_open only works when a directory is selected using my_chdir.
my_chdir my_rmdir only works when a directory is already created using my_mkdir.
my_copy <file_name> only works when a directory is already created using my_mkdir, and the file_name is of a real file in the linux system present in the same real directory as the FAT.cpp program.
my_copy <file_name> is used to copy an actual file named fine_name in the same real directory to the created FAT directory which is open in the FAT.cpp program.
The copied file has the same file name as the real file.

For both the programs, input is necessary from the user as prompted by the program.