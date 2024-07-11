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
    string categoryString{ "" };
    string description{ "" };
    string imagePath{ "" };
    string drawerLabel{ "" };
    string color{ "" };
    string uses{ "" };
    string unexpectedUses{ "" };
    string logoFilePath{ "" };
    string details { ""}; 

    CFC::MaterialTypes materialType;

    void assignCategory()
    {
        if( categoryString == "manufactured" )
            materialType = CFC::MaterialTypes::MANUFACTURED;
        else if( categoryString == "meta" )
            materialType = CFC::MaterialTypes::META;
        else if( categoryString == "common" )
            materialType = CFC::MaterialTypes::COMMON;
        else if( categoryString == "novel" )
            materialType = CFC::MaterialTypes::NOVEL;
    }
};