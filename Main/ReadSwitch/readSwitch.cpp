#include "../Common.hpp"

#ifndef DEBUG
#include <wiringPi.h>
#endif

#include "../Switch/switch.h"
#include "ReadSwitch.h"
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>

#define WAIT_TIME 500

ReadSwitch::ReadSwitch(){
    
}

ReadSwitch::ReadSwitch(int readPin,int writePin,int intId){
    
#ifndef DEBUG
    wiringPiSetup();
#endif
    objCurrentSwitch = new Switch(readPin,writePin);
    objCurrentSwitch->id = intId;
}

void ReadSwitch::startReading(){
    
#ifndef DEBUG
    
    if(objCurrentSwitch->isInitialised == false && objCurrentSwitch->isOn == true){
        cout<<"Previously it was ON";
        pinMode(objCurrentSwitch->writePin, OUTPUT);
        objCurrentSwitch->isInitialised = true;
    }
    
    pinMode(objCurrentSwitch->readPin, INPUT);
    
    while(1){
        delay(100);
        
        int intPinValue = digitalRead(objCurrentSwitch->readPin);
        if (intPinValue == 1)
        {
            delay(WAIT_TIME);
            intPinValue = digitalRead(objCurrentSwitch->readPin);
            
            if (intPinValue == 0)
                this->ToggleSwitch();
        }
    }
    
#endif
}

void ReadSwitch::ToggleSwitch(){
    
    cout << "S is reverted";
    objCurrentSwitch->isOn = !objCurrentSwitch->isOn;
    cout << "\n Conditiona is" << objCurrentSwitch->isOn << "\n";
    
#ifndef DEBUG
    
    if(objCurrentSwitch->isInitialised == false && objCurrentSwitch->isOn == true){
        
        pinMode(objCurrentSwitch->writePin, OUTPUT);
        objCurrentSwitch->isInitialised = true;
    }
    
    if (objCurrentSwitch->isOn == true)
        digitalWrite (objCurrentSwitch->writePin, HIGH) ;  // On
    else
        digitalWrite (objCurrentSwitch->writePin, LOW) ;   // Off
#endif
    
    if(toggleListner != NULL)
        this->toggleListner(*objCurrentSwitch);
}
