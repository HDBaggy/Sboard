#include "server.h"
#include <fstream>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>


using namespace std;

//Actually allocate clients
vector<Client> Server::clients;

Server::Server(int port) {
    
    //Initialize static mutex from MyThread
    MyThread::InitMutex();
    
    //For setsock opt (REUSEADDR)
    int yes = 1;
    
    //Init serverSock and start listen()'ing
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
    
    //Avoid bind error if the socket was not close()'d last time;
    setsockopt(serverSock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));

    #ifndef DEBUG
    if(bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(sockaddr_in)) < 0)
        cerr << "Failed to bind";
    #endif

    listen(serverSock, 5);
}

/*
 AcceptAndDispatch();
 
 Main loop:
 Blocks at accept(), until a new connection arrives.
 When it happens, create a new thread to handle the new client.
 */
void Server::AcceptAndDispatch() {
    
    Client *c;
    MyThread *t;
    
    socklen_t cliSize = sizeof(sockaddr_in);
    
    while(1) {
        
        c = new Client();
        t = new MyThread();
        
        //Blocks here;
        c->sock = accept(serverSock, (struct sockaddr *) &clientAddr, &cliSize);
        
        if(c->sock < 0) {
            cerr << "Error on accept";
        }
        else {
            
            server_t *objMsg = new server_t();
            objMsg->objClient = c;
            objMsg->serverListner = this->serverListner;
            
            t->Create((void *) Server::HandleClient, objMsg);
        }
    }
}

//Static
void *Server::HandleClient(void *args) {
    
    //Pointer to accept()'ed Client
    
    server_t *objMsg = (server_t*) args;
    
    if (objMsg->objClient == NULL) {
        cerr << "Structure is empty";
    }
    
    Client *c = objMsg->objClient;
    
    if (c == NULL) {
        cerr << "Client object is empty";
    }
    
    void(*tmpServerListner)(string strMessage) = objMsg->serverListner;
    
    //Client *c = (Client *) args;
    char buffer[256-25], message[256];
    int index;
    int n;
    
    //Add client in Static clients <vector> (Critical section!)
    MyThread::LockMutex((const char *) c->name);
    
    //Before adding the new client, calculate its id. (Now we have the lock)
    c->SetId(Server::clients.size());
    sprintf(buffer, "Client n.%d", c->id);
    c->SetName(buffer);
    cout << "Adding client with id: " << c->id << endl;
    Server::clients.push_back(*c);
    
    MyThread::UnlockMutex((const char *) c->name);
    
    while(1) {
        memset(buffer, 0, sizeof buffer);
        n = recv(c->sock, buffer, sizeof buffer, 0);
        
        //Client disconnected?
        if(n == 0) {
            cout << "Client " << c->name << " diconnected" << endl;
            close(c->sock);
            
            //Remove client in Static clients <vector> (Critical section!)
            MyThread::LockMutex((const char *) c->name);
            
            index = Server::FindClientIndex(c);
            cout << "Erasing user in position " << index << " whose name id is: "
            << Server::clients[index].id << endl;
            Server::clients.erase(Server::clients.begin() + index);
            
            MyThread::UnlockMutex((const char *) c->name);
            
            break;
        }
        else if(n < 0) {
            cerr << "Error while receiving message from client: " << c->name << endl;
        }
        else {
            
            string strTest("test\n");
            string strStatus("s\n");
            string strToggle("t1\n");
            string strReceived(buffer);
            
            //Message received. Send to all clients.
            snprintf(message, sizeof message, "{\"%s\": \"%s\"}", c->name, buffer);
            
            if (buffer ==  strTest){
                
                strcpy(buffer,"owwwww");
                
                ofstream myfile;
                myfile.open ("server.txt");
                myfile <<  ::getpid();
                myfile.close();
                
            } else if(buffer == strToggle) {
  
                if(tmpServerListner != NULL){
                    tmpServerListner(buffer);
                }
                
            } else if (buffer == strStatus){
                
                ifstream myReadFile;
                myReadFile.open("s1_status.txt");
                char output[10];
                if (myReadFile.is_open()) {
                    
                    while (!myReadFile.eof()) {
                        myReadFile >> output;
                    }
                }
                myReadFile.close();
                
                char *value = output;
                
                const char* strPID  = value;
                
                std::string str1(strPID);
                int intId = atoi(str1.c_str());
                
                if (intId==0){
                    strcpy(buffer,"0");
                    strcpy(message,"0");
                } else {
                    strcpy(message,"1");
                    strcpy(message,"1");
                }
            }
            
            cout << "Will send to all: " << message << endl;
            Server::SendToAll(message);
        }
    }
    
    //End thread
    return NULL;
}

void Server::SendToAll(char *message) {
    int n;
    
    //Acquire the lock
    MyThread::LockMutex("'SendToAll()'");
    
    for(size_t i=0; i<clients.size(); i++) {
        n = send(Server::clients[i].sock, message, strlen(message), 0);
        cout << n << " bytes sent." << endl;
    }
    
    //Release the lock
    MyThread::UnlockMutex("'SendToAll()'");
}

void Server::ListClients() {
    for(size_t i=0; i<clients.size(); i++) {
        cout << clients.at(i).name << endl;
    }
}

/*
 Should be called when vector<Client> clients is locked!
 */
int Server::FindClientIndex(Client *c) {
    for(size_t i=0; i<clients.size(); i++) {
        if((Server::clients[i].id) == c->id) return (int) i;
    }
    cerr << "Client id not found." << endl;
    return -1;
}
