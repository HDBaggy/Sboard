/** --------------------------------------------------------------------------
*  echoServer.cpp
*
*  A basic example of how to implement a WebSocketServer.  Creats a WS server
*  bound to port 8080.  It listens for connections, and when a message is
*  received, it echos the same message back to the client.
*
*  Author    : Jason Kruse <jason@jasonkruse.com> or @mnisjk
*  Copyright : 2014
*  License   : BSD (see LICENSE)
*  --------------------------------------------------------------------------
**/

// Log to stdout for easy debugging.
#define LOG_TO_STDOUT 1

#include "Util.h"
#include "WebSocketServer.h"
#include <fstream>
#include <stdio.h>
#include <wiringPi.h>
#include <cstdlib>
#include <iostream>
#include <pthread.h>


#define WAIT_TIME 500

using namespace std;

class Switch {

public:
  //	static void startGlobalSensor(Switch pobjSwitch);
  bool isOn = true;
  int intWritePin = 0;
  int intReadPin = 0;
  pthread_t threads[1];


  // Constructor with no arguments
  Switch(): intWritePin(0), intReadPin(0) { }

  // Constructor with two arguments
  Switch(int l, int b): intWritePin(l), intReadPin(b){ }

  void initializeOnce (){

    cout << "\n 1. Starting sensor" << intWritePin << "Read Value" << intReadPin;

    wiringPiSetup();
    pinMode(intWritePin, OUTPUT);
    pinMode(intReadPin, INPUT);

  //  Switch *objTmpSwitch = this;
  //  int rc2 = pthread_create(&threads[0], NULL, &Switch::startSensor, &objTmpSwitch);
  //  pthread_exit(NULL);

  }


  void startSensor(){

    // cout << "\n 2. Read Pin " << intReadPin << " Started \n";
for(;;){
    // Read Switch state for manually press
    int intPinValue = digitalRead(intReadPin);

    if (intPinValue == 1)
    {
      delay(WAIT_TIME);
      intPinValue = digitalRead(intReadPin);

      if (intPinValue == 0)
      this ->  toggle();

      this -> toggledState(isOn, this);
    }
  }
}
  void toggle(){

    isOn = !isOn;
    if (isOn == true)
    digitalWrite (intWritePin, HIGH) ;  // On
    else
    digitalWrite (intWritePin, LOW) ;   // Off

    Util::log( "\n Toggle from Input: " + intReadPin );
  }

  virtual void toggledState(bool state, Switch *objTmpSwitch);

};

class SBoard {

public:
  Switch arrSwitches[1];


  void initializeOnce(){

    cout << "INitialize Switchboard called";

    Switch objSwitch1 = Switch(0,29);
    arrSwitches[0] =  objSwitch1;

    Switch objSwitch2 = Switch(2,28);
    arrSwitches[1] = objSwitch2;

    //  arrSwitches[1] =  Switch(2,28);
    //  arrSwitches[2] =  Switch(3,24);
    //  arrSwitches[3] =  Switch(7,11);
  }

  void startSensors(){

    Util::log( "SBoard Starting sensors \n" );

    int i=0;
    int j = sizeof(arrSwitches)/sizeof(arrSwitches[0]);

    cout << "Total Switches are " << j;

    for (i=0;i<j;i++){
      Switch objSwitch =  arrSwitches[i];
      objSwitch.initializeOnce();
    }
  }
};

// For any real project this should be defined separately in a header file
class EchoServer : public WebSocketServer
{
public:
  void (*msg)();

  EchoServer( int port );
  ~EchoServer( );
  virtual void onConnect(    int socketID                        );
  virtual void onMessage(    int socketID, const string& data    );
  virtual void onDisconnect( int socketID                        );
  virtual void   onError(    int socketID, const string& message );
};

void serverGotMessage(void);
void *startGpioServer1(void *t);
void *startGpioServer2(void *t);
void *startGpioServer3(void *t);

EchoServer es = EchoServer(8080);
SBoard objBoard;

int main( int argc, char **argv )
{

  ::objBoard.initializeOnce();
  ::objBoard.startSensors();

  pthread_t threads[1];
  int rc1 = pthread_create(&threads[0], NULL, startGpioServer1, NULL);
int rc2 = pthread_create(&threads[1], NULL, startGpioServer2, NULL);
// int rc1 = pthread_create(&threads[0], NULL, startGpioServer, NULL);

  if (rc1 ) {
    cout << "Error:unable to create GPIO thread," << rc1 << endl;
    exit(-1);
  }

  if (rc2 ) {
    cout << "Error:unable to create GPIO thread," << rc1 << endl;
    exit(-1);
  }
  //   pthread_exit(NULL);

  ::es.msg = serverGotMessage;
  ::es.run();
}

void *startGpioServer1(void *t){

    Switch objSwitch =  ::objBoard.arrSwitches[0];
    objSwitch.startSensor();
}

void *startGpioServer2(void *t){

    Switch objSwitch =  ::objBoard.arrSwitches[1];
    objSwitch.startSensor();
}

void *startGpioServer3(void *t){

    Switch objSwitch =  ::objBoard.arrSwitches[2];
    objSwitch.startSensor();
}

void serverGotMessage(void){

  // ::objBoard.toggle();
}

void Switch::toggledState(bool state, Switch *objTmpSwitch)
{
  cout << "The output -> " << objTmpSwitch->intReadPin << " State is ->" << state;
  if (state == true){
    ::es.broadcast("Switch On");
  } else {
    ::es.broadcast("Switch Off");
  }
}

EchoServer::EchoServer( int port ) : WebSocketServer( port )
{

}

EchoServer::~EchoServer( )
{

}

void EchoServer::onConnect( int socketID )
{
  Util::log( "New connection" );
}

void EchoServer::onMessage( int socketID, const string& data )
{
  // Reply back with the same message
  Util::log( "Received: " + data );

  this->send( socketID, data );
  this->broadcast(data);
  this->msg();
}

void EchoServer::onDisconnect( int socketID )
{
  Util::log( "Disconnect" );
}

void EchoServer::onError( int socketID, const string& message )
{
  Util::log( "Error: " + message );
}
