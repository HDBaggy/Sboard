#include <wiringPi.h>
#include "switch.h"
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>

#define WAIT_TIME 500

int send_signal (int pid);
void ToggleSwitch(Switch *s);
void readSwitch(Switch *s);
void saveState();
void savePID();
char* getPID();

Switch *s;
string filename;
string pidfilename;

void my_signal_interrupt(int sig)
{
    printf("I got signal %d\n", sig);
    //  (void) signal(SIGUSR1, SIG_DFL);
	ToggleSwitch(s);

}

int main(int argc, const char* argv[]){

        (void) signal(SIGUSR1,my_signal_interrupt);


	wiringPiSetup();

	if (argc < 3){
        	cout << "\n\nERROR: Pass read,write pins\n\n";
	return 0;	
}


	s = new Switch();
        s->readPin = atoi(argv[1]);
        s->writePin = atoi(argv[2]);

	s->name = strdup(argv[3]);
	filename = s->name;
	pidfilename = strdup(argv[4]);

savePID();
	readSwitch(s);


cout << "\n\n";
  return 0;

}

void readSwitch(Switch *s){

	pinMode(s->writePin, OUTPUT);
        pinMode(s->readPin, INPUT);


while(1){
	delay(100);
   int intPinValue = digitalRead(s->readPin);
      if (intPinValue == 1)
      {
//       cout << "Switch touch down";

        delay(WAIT_TIME);
        intPinValue = digitalRead(s->readPin);

        if (intPinValue == 0)
             ToggleSwitch(s);

//        this -> toggledState(isOn, this);      
        }
  }
}

void ToggleSwitch(Switch *s){

  s->isOn = !s->isOn;
cout << "\n Conditiona is" << s->isOn << "\n";

    if (s->isOn == true)
    digitalWrite (s->writePin, HIGH) ;  // On
    else
    digitalWrite (s->writePin, LOW) ;   // Off

    saveState();

    const char* strPID = getPID();
    std::string str1(strPID);
    int intId = atoi(str1.c_str());
    
    if (intId != 0){
	cout<< "signal sent";     
   send_signal(intId);
    }
}

void savePID(){
    
    ofstream myfile;
    myfile.open (pidfilename);
    myfile << ::getpid();
    myfile.close();

}

void saveState(){

    ofstream myfile;
    myfile.open (filename);
    if(s->isOn == true)
	myfile << "1";
   else
       myfile << "0";

	myfile.close();
}

char* getPID(){

    ifstream myReadFile;
    myReadFile.open("server.txt");
    char output[10];
	char *value;
    if (myReadFile.is_open()) {

        while (!myReadFile.eof()) {
            myReadFile >> output;
	    value = output;
        }
    } else {

       value = strdup("0");
	}
    myReadFile.close();

//    cout << "PID Is " << value;
    return value;

}

int send_signal (int pid)
{
        int ret;
        ret = kill(pid,SIGUSR1);
        printf("ret : %d",ret);
	return 1;
}
