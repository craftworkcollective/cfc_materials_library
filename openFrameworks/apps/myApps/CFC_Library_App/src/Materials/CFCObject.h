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

    int    indexUID{ 0 };
    string title{ "" };
    string categoryString{ "" };
    string description{ "" };
    string imagePathGrid{ "" };
    string imagePathTop{ "" };
    string drawerLabel{ "" };
    string color{ "" };
    string uses{ "" };
    string logoFilePath{ "" };
    string details{ "" };
    string company{ "" };

    CFC::MaterialTypes materialType;

    void assignCategory()
    {

        if( !categoryString.empty() && categoryString.back() == ' ' ) {
            categoryString.pop_back();
        }

        if( ofToUpper( categoryString ) == ofToUpper( "Common" ) ) {
            materialType = CFC::MaterialTypes::COMMON;
        }
        else if( ofToUpper( categoryString ) == ofToUpper( "Novel Sustainable" ) || ofToUpper( categoryString ) == ofToUpper( "Novel" ) ) {
            materialType = CFC::MaterialTypes::NOVEL;
        }
        else if( ofToUpper( categoryString ) == ofToUpper( "Manufactured" ) ) {
            materialType = CFC::MaterialTypes::MANUFACTURED;
        }
        else if( ofToUpper( categoryString ) == ofToUpper( "Meta" ) ) {
            materialType = CFC::MaterialTypes::META;
        }
        else {
            ofLogError() << categoryString << " is not a type! Defaulting to common";
            materialType = CFC::MaterialTypes::COMMON;
        }
    }
};