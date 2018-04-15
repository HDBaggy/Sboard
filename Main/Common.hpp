//
//  Common.hpp
//  SBBoardSingle
//
//  Created by Harshit on 31/03/18.
//  Copyright © 2018 ZestyGuys. All rights reserved.
//

#ifndef Common_hpp
#define Common_hpp

#include <stdio.h>
#include "ReadSwitch/ReadSwitch.h"

//#define OS 1

typedef struct thread_data {
    int thread_id;
    int read_pin;
    int write_pin;
} tdata_t;

#endif /* Common_hpp */
