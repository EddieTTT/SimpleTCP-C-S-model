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
    //Create a listen socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        cerr<<"Can't create a socker!";
        return -1;
    }
    //Bind listenfd to a IP/PORT
    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT_NUM);
    inet_pton(AF_INET,"0.0.0.0",&servaddr.sin_addr);

    if(bind(listenfd,(sockaddr*)&servaddr,sizeof(servaddr))== -1)
    {
        cerr<<"Can't bind to IP/port";
        return -2;
    }
    //Mark the socket for listenfd
    if(listen(listenfd,SOMAXCONN)==-1)//SOMAXCONN == 128
    {
        cerr<<"Can't listen!";
        return -3;
    }
    //Accept a client call
    sockaddr_in client;
    socklen_t clilen = sizeof(client);
    char host[NI_MAXHOST];//1025
    char svc[NI_MAXSERV];//32

    int cfd = accept(listenfd,(sockaddr*)&client,&clilen);
    if(cfd == -1)
    {
        cerr<<"Problem with client connecting!";
        return -4;
    }

     close(listenfd);

    memset(host,0,NI_MAXHOST);
    memset(svc,0,NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client,sizeof(client),host,NI_MAXHOST,svc,NI_MAXSERV,0);

    if(result)
    {
        cout<<host<<" connected on "<<svc<<endl;
    }
    else
    {
        inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
        cout<<host<<" connected on "<<ntohs(client.sin_port)<<endl;

    }
    //display message while loop
    char buf[4096];
    while(1)
    {
        //clear the buffer
        memset(buf,0,4096);
        //wait for a message
        int bytesRecv = recv(cfd,buf,4096,0);
        if(bytesRecv == -1)
        {
            cerr<<"There was a connection issue"<<endl;
            break;
        }
        if(bytesRecv == 0)
        {
            cout<<"The client disconnected"<<endl;
            break;
        }
        cout<<"Received:"<<string(buf,0,bytesRecv)<<endl;
        send(cfd,buf,bytesRecv+1,0);
        //display message
        //resend message
    }
    close(cfd);

    return 0;
}
