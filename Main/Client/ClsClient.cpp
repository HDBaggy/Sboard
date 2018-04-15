#include <iostream>
#include <signal.h>
#include "TCPClient.h"
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <pthread.h>
#include <thread>
#include <chrono>
#include "ClsClient.h"
#include "../Common.hpp"

using namespace std;

void *startTmpMsg(void *t);
void sendMessage();

TCPClient tcp;

void sig_exit(int s)
{
    tcp.exit();
    exit(0);
}

ClsClient::ClsClient(){

}

ClsClient::ClsClient(string strAddress, int port){
    
    pthread_t threads[1];

    int i = 0;
    
    while (tcp.setup(strAddress, port) == false){
        
        i++;
        cout << "Retrying...";
        
        sleep(5);
        if(i>300 && i<600){
            sleep(15);
        } else if(i>600 && i<900){
            sleep(30);
        } else if(i>900 && i< 3600){
            sleep(45);
        } else if (i>3600 && i<7200){
            sleep(120);
        } else if (i>7200 && i<9600) {
            sleep(360);
        } else if (i==9600) {
            sleep(960);
        } else {
            sleep(2);
            i = 0;
        }
    }

    i = 0;
    
    while(1)
    {
        srand(time(NULL));
        string rec = tcp.receive();
        
        
        if (rec == "t1" || rec=="t1\n"){
            
            cout << "toggle";
            //            tcp.Send(to_string("toggled to"));
            //            sendMessage();
            int rc6 = pthread_create(&threads[0], NULL, startTmpMsg, NULL);
            
            if (rc6 ) {
                cout << "Error:unable to create Client thread," << rc6 << endl;
                exit(-1);
            }
            
            if(clientListner != NULL)
                clientListner(rec);
            
        } else if ( rec != "" ){
            cout << "Server Response:" << rec;
        }
    }
    sleep(1);
}

void *startTmpMsg(void *t){
    sendMessage();
    pthread_exit(0);
}

void sendMessage(){
    
    ofstream myfile;
    myfile.open ("server_log.txt");
    myfile <<  "t1";
    myfile.close();
    
    ifstream myReadFile;
    myReadFile.open("s1_pid.txt");
    char output[10];
    
    if (myReadFile.is_open()) {
        
        while (!myReadFile.eof()) {
            myReadFile >> output;
            //cout<<output;
        }
    }
    
    char *value = output;
    const char* strPID  = value;
    
    std::string str1(strPID);
    int intId = atoi(str1.c_str());
    
    if (intId != 0){
        cout << "Sending notification to PID: " << intId;
        int ret;
        ret = kill(intId,SIGUSR1);
    }else {
        cout << "PID wad nil";
    }
}
