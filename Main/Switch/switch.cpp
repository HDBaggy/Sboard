#include "switch.h"

using namespace std;

Switch::Switch() {
  this->name = (char *) malloc(MAX_NAME_LENGHT+1);
}

Switch::Switch(int readPin, int writePin){
    
    this->readPin = readPin;
    this->writePin = writePin;
}

void Switch::SetName(const char *name) {
  //Copies at most MAX_NAME_LENGHT + 1 (including '\0')
  snprintf(this->name, MAX_NAME_LENGHT+1, name);
}

void Switch::SetId(int id) {
  this->id = id;
}

void Switch::SetPins(int readPin,int writePin){

	this->readPin = readPin;
	this->writePin = writePin;
}
