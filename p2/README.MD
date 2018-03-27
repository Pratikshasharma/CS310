//We consulted Head TA (Waqar Aqeel)
//We used the recommended shellcode. We obviously changed the return address.
//We made 500 no-ops based on our calculations
//We ran a java file to print out those hex values of the bytecode instructions and then copy pasted into 
a terminal. Details are in shellcode.c . The actually shellcode is at the very bottom of that file.

Sequence of commands to run:

1) echo -e "GET <attack string> HTTP" | nc 9218

2) connect to port 5050 after opening it (open another tab in termial)
and to also send in the string that replaces the content of index.html in the remote server::
        nc 310test.cs.duke.edu 5050

        
        

3)Doing this took us to a terminal on the remote server.
    We cd'd into the www folder where the index.html file was and used vim to replaced the contents of the file with "we hacked this".
    
    
    

    



