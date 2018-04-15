//
//  readSwitch.h
//  SBBoardSingle
//
//  Created by Harshit on 31/03/18.
//  Copyright © 2018 ZestyGuys. All rights reserved.
//

#ifndef readSwitch_h
#define readSwitch_h

#include "../Switch/switch.h"

class ReadSwitch {
  
public:
    Switch *objCurrentSwitch;
    void(*toggleListner)(Switch objSwitch);
    void ToggleSwitch();
    void startReading();
    
    ReadSwitch();
    ReadSwitch(int readPin,int writePin,int intId);
};

#endif /* readSwitch_h */
