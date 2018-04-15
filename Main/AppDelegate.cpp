//
//  AppDelegate.cpp
//  SBBoardSingle
//
//  Created by Harshit on 30/03/18.
//  Copyright © 2018 ZestyGuys. All rights reserved.
//

#include "AppDelegate.hpp"
#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <thread>
#include <fstream>
#include <chrono>
#include "ReadSwitch/ReadSwitch.h"
#include "Client/ClsClient.h"
#include "Server/server.h"
#include "Server/mythread.h"
#include "Common.hpp"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


using namespace std;

static ReadSwitch arrSwitch[5];

ClsClient objClient;
ReadSwitch objRead1;
ReadSwitch objRead2;
ReadSwitch objRead3;
ReadSwitch objRead4;
Server *objServer;


void *startSwitch1Thread(void *t);
void *startSwitch2Thread(void *t);
void *startSwitch3Thread(void *t);
void *startSwitch4Thread(void *t);
void *startClientThread(void *t);
void *startServerThread(void *t);
void *wifiReconnectorThread(void *t);
void saveState();
bool readSwitchState(string id);


AppDelegate::AppDelegate() {
    
    pthread_t threads[8];
    
    int rc1 = pthread_create(&threads[0], NULL, startSwitch1Thread, NULL);
    int rc2 = pthread_create(&threads[1], NULL, startSwitch2Thread, NULL);
    int rc3 = pthread_create(&threads[2], NULL, startSwitch3Thread, NULL);
    int rc4 = pthread_create(&threads[3], NULL, startSwitch4Thread, NULL);
    int rc5 = pthread_create(&threads[4], NULL, startClientThread, NULL);
    int rc6 = pthread_create(&threads[5], NULL, startServerThread, NULL);
    int rc7 = pthread_create(&threads[6], NULL, wifiReconnectorThread, NULL);
    
    if (rc1 ) {
        std::cout << "Error:unable to create thread." << rc1 << std::endl;
        exit(-1);
    }
    if (rc2 ) {
        std::cout << "Error:unable to create thread." << rc2 << std::endl;
        exit(-1);
    }
    if (rc3 ) {
        std::cout << "Error:unable to create thread." << rc3 << std::endl;
        exit(-1);
    }
    if (rc4 ) {
        std::cout << "Error:unable to create thread." << rc4 << std::endl;
        exit(-1);
    }
    if (rc5 ) {
        std::cout << "Error:unable to create thread." << rc5 << std::endl;
        exit(-1);
    }
    if (rc6 ) {
        std::cout << "Error:unable to create thread." << rc6 << std::endl;
        exit(-1);
    }
    if (rc7 ) {
        std::cout << "Error:unable to create thread." << rc6 << std::endl;
        exit(-1);
    }
    
    std::this_thread::sleep_for(5000h);
}

void serverListner(string strMessage){
    
    if (strMessage == "t1" || strMessage == "t1\n"){
        objRead1.ToggleSwitch();
        saveState();
    }
}

void *startServerThread(void *t){

    objServer = new Server(30666);
    objServer->serverListner = serverListner;
    objServer->AcceptAndDispatch();
    
    pthread_exit(0);
}

void clientListner(string strMessage){
    
    if (strMessage == "t1"){
        objRead1.ToggleSwitch();
        saveState();
    }
}

void *startClientThread(void *t){

    //objClient = ClsClient("localhost", 30661);
     objClient = ClsClient("zestyguys.com", 30666);
    //objClient = ClsClient("raspberrypi.local", 30666);
    objClient.clientListner = clientListner;
    
    pthread_exit(0);
}

// ReadSwitch.cpp Call Back
void toggleListner(Switch pobjSwitch){
    cout << "Switch toggled" << pobjSwitch.readPin;
}

void *startSwitch1Thread(void *t)
{
    objRead1 = ReadSwitch(29,0,1);
    objRead1.objCurrentSwitch->isOn = readSwitchState(to_string(objRead1.objCurrentSwitch->id));
    objRead1.toggleListner = toggleListner;
    objRead1.startReading();
    pthread_exit(0);
}
void *startSwitch2Thread(void *t)
{
    objRead2 = ReadSwitch(24,3,2);
    objRead2.objCurrentSwitch->isOn = readSwitchState(to_string(objRead2.objCurrentSwitch->id));
    objRead2.toggleListner = toggleListner;
    objRead2.startReading();
    pthread_exit(0);
}
void *startSwitch3Thread(void *t)
{
    objRead3 = ReadSwitch(13,7,3);
    objRead3.objCurrentSwitch->isOn = readSwitchState(to_string(objRead3.objCurrentSwitch->id));
    objRead3.toggleListner = toggleListner;
    objRead3.startReading();
    pthread_exit(0);
}
void *startSwitch4Thread(void *t)
{
    objRead4 = ReadSwitch(28,2,4);
    objRead4.objCurrentSwitch->isOn = readSwitchState(to_string(objRead4.objCurrentSwitch->id));
    objRead4.toggleListner = toggleListner;
    objRead4.startReading();
    pthread_exit(0);
}

void *wifiReconnectorThread(void *t)
{
    while(1){
        #ifndef DEBUG
        std::system("sh /home/pi/Production/Sboard/WifiReconnect/Wifi_reconnect.sh");
#else
        std::system("sh WifiReconnect/Wifi_reconnect.sh");
#endif
        std::this_thread::sleep_for(5s);
    }
    pthread_exit(0);
}

bool readSwitchState(string id){
    
    ifstream myReadFile;
    myReadFile.open("states.json");
    char output[500];
    char *value;
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
            myReadFile >> output;
            value = output;
        }
        myReadFile.close();
    } else {
        
        return false;
    }
    
    rapidjson::Document d;
    d.Parse(value);
    cout << "Stored json value is \n\n" << value;
    if (d.HasMember(id.c_str()) == true){
        rapidjson::Value& state =  d[id.c_str()];
        int intState = state.GetInt();
        return (intState == 1) ? true : false;
    }
    return false;
}

void saveState(){
    
    ReadSwitch arrSwitches[] = {objRead1,objRead2,objRead3, objRead4};
    
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    writer.StartObject();
    
    for(int i=0;i<4;i++){
        
        ReadSwitch objSwitch = arrSwitches[i];
        string strKey = to_string(objSwitch.objCurrentSwitch->id);
        string strValue = to_string(objSwitch.objCurrentSwitch->isOn);
        
        writer.Key(strKey.c_str());
        //writer.String(strValue.c_str());
        writer.Int(objSwitch.objCurrentSwitch->isOn);
    }
    writer.EndObject();
    
    ofstream myfile;
    myfile.open ("states.json");
    myfile <<  s.GetString();
    myfile.close();
}


