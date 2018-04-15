//
//  client.h
//  SBBoardSingle
//
//  Created by Harshit on 31/03/18.
//  Copyright © 2018 ZestyGuys. All rights reserved.
//

#ifndef client_h
#define client_h

class ClsClient {
  
public:
    void(*clientListner)(string strMessage);
    ClsClient();
    ClsClient(string strAddress, int port);
};

#endif /* client_h */
