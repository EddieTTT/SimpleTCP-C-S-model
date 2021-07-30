#include <iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string.h>
#include<string>
using namespace std;

#define PORT_NUM 6666

int main()
{
    //Create a socket
    int cfd = socket(AF_INET,SOCK_STREAM,0);
    if(cfd== -1)
    {
        cerr<<"socket error";
        return 1;
    }

    //create a server which i'm going to connect
    string ipAddress = "127.0.0.1";

    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUM);
    inet_pton(AF_INET,ipAddress.c_str(),&servaddr.sin_addr);

    //connect to the server on the socket
    int connectRes = connect(cfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(connectRes == -1)
    {
        return 1;
    }

    //while loop
    char buf[4096];
    string userInput;

    while(1){
            // Enter lines of text
        cout<<">";
        getline(cin,userInput);
        // Send message to server
        int sendRes = send(cfd,userInput.c_str(),userInput.size()+1,0);
        if(sendRes == -1)
        {
            cout<<"Cloud not send to sever!\r\n";
            continue;
        }
        //wait for response
        memset(buf,0,4096);
        int byteReceived = recv(cfd,buf,4096,0);

        //Display message
        cout<<"Server> "<<string(buf,byteReceived)<<"\r\n";

    }
    close(cfd);
    return 0;
}
