#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <thread>
#include <chrono>

// wow
using namespace std;

void *startGpioServer1(void *t);
void *startGpioServer2(void *t);
void *startGpioServer3(void *t);
void *startGpioServer4(void *t);
void *startSocketServer1(void *t);
void *startWifiMonitor(void *t);
void *startClient(void *t);

int main() {
 
pthread_t threads[8];


       int rc1 = pthread_create(&threads[0], NULL, startGpioServer1, NULL);
       int rc2 = pthread_create(&threads[1], NULL, startGpioServer2, NULL);
       int rc3 = pthread_create(&threads[2], NULL, startGpioServer3, NULL);
       int rc4 = pthread_create(&threads[3], NULL, startGpioServer4, NULL);
       int rc5 = pthread_create(&threads[4], NULL, startSocketServer1, NULL);
       int rc6 = pthread_create(&threads[5], NULL, startWifiMonitor, NULL);
	int rc7 = pthread_create(&threads[6], NULL, startClient, NULL);

  if (rc1 ) {
    cout << "Error:unable to create GPIO thread." << rc1 << endl;
    exit(-1);
  }

  if (rc2 ) {
    cout << "Error:unable to create GPIO thread." << rc1 << endl;
    exit(-1);
  }


  if (rc3 ) {
    cout << "Error:unable to create GPIO thread," << rc3 << endl;
    exit(-1);
  }

  if (rc4 ) {
    cout << "Error:unable to create GPIO thread," << rc4 << endl;
    exit(-1);
  }

if (rc5 ) {
    cout << "Error:unable to create Socket server thread," << rc5 << endl;
    exit(-1);
  }


if (rc6 ) {
    cout << "Error:unable to create Wifi monitor thread." << rc6 << endl;
    exit(-1);
  }

if (rc7 ) {
    cout << "Error:unable to create Client thread." << rc7 << endl;
    exit(-1);
  }

std::this_thread::sleep_for(5000h);

   return 0;
}

void *startGpioServer1(void *t){
  std::system("/home/pi/Sboardv2/switchboard/readSwitch 28 2 s4_state.txt s4_pid.txt");
}

void *startGpioServer2(void *t){
   std::system("/home/pi/Sboardv2/switchboard/readSwitch 29 0 s1_state.txt s1_pid.txt");
}

void *startGpioServer3(void *t){
   std::system("/home/pi/Sboardv2/switchboard/readSwitch 24 3 s2_state.txt s2_pid.txt");
}

void *startGpioServer4(void *t){
   std::system("/home/pi/Sboardv2/switchboard/readSwitch 13 7 s3_state.txt s3_pid.txt");
}

void *startSocketServer1(void *t){
   std::system("/home/pi/Sboardv2/switchboard/Server");
}

void *startWifiMonitor(void *t){
///   std::system("/home/pi/Sboardv2/switchboard/network-monitor.sh &");
}

void *startClient(void *t){
   std::system("/home/pi/Sboardv2/switchboard/Client");
}

