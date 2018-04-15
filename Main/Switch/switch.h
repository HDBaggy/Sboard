#ifndef _switch_h_
#define _switch_h_

#include <iostream>

#include <cstdio>
#include <cstdlib>

#define MAX_NAME_LENGHT 20

using namespace std;

class Switch {
  public:
    char *name;
    int id;
    bool isOn;
    bool isInitialised;
    int readPin;
    int writePin;

  public:
    Switch();
    Switch(int readPin, int writePin);
    void SetName(const char *name);
    void SetId(int id);
    void SetPins(int readPin,int writePin);
};

#endif
