//
// CFCObject.h
//
//  Created by Nicole Yi Messier
//
//

#pragma once

#include "CFCStructs.h"

class CFCObject {

  public:
    CFCObject(){};
    ~CFCObject(){};

    string uid{ "" };
    string title{ "" };
    string category{ "" };
    string description{ "" };
    string imagePath{ "" };
    string drawerLabel{ "" };
    string color{ "" };
    string uses{ "" };
    string unexpectedUses{ "" };
    string logoFilePath{ "" };
};