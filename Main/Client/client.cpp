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

void *startTmpMsg(void *t);
void sendMessage();

TCPClient tcp;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_exit);
	
	pthread_t threads[1];



	tcp.setup("www.zestyguys.com",30666);
	while(1)
	{
		srand(time(NULL));
//		tcp.Send(to_string(rand()%25000));
		string rec = tcp.receive();

		 if (rec == "t1" || rec=="t1\n"){

			cout << "toggle";
//			tcp.Send(to_string("toggled to"));
//			sendMessage();
	 int rc6 = pthread_create(&threads[0], NULL, startTmpMsg, NULL);

if (rc6 ) {
    cout << "Error:unable to create Client thread," << rc6 << endl;
    exit(-1);
  }

		} else if ( rec != "" ){
			cout << "Server Response:" << rec;
		}
		sleep(1);
	}
	return 0;
}

void *startTmpMsg(void *t){
   sendMessage();
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
