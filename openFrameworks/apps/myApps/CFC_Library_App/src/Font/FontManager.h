//
//  FontManager.h
//  ApartmentViewer
//
//  Created by Nicole Yi Messier on 3/4/20.
//

#pragma once

#include "ofMain.h"
#include "ofxFontStash.h"

class FontManager {
  public:
    static FontManager &one()
    {
        static FontManager instance;
        return instance;
    };

    FontManager(){};
    ~FontManager(){};

    // --- FONT OBJECTS --- //
    ofxFontStash reg;
    ofxFontStash medium;
    ofxFontStash bold;
    ofxFontStash semiBold;

    // --- FONT ATTRIBUTES --- //
    float lineHeightPercent = 1.0f;
    int   texDimension = 1024;
    bool  createMipMaps = false;
    int   intraCharPadding = 10;
    float dpiScale = 1.5f;


    void setupFonts();
    void update();

    void   drawTitle( string title );
    void   drawBody( string description, string composite, string uses, string details );
    void   drawDrawer( string drawer );
    void   drawMaterialCategory( string txt );
    string capitalizeFirstLetter( const std::string &input );

  private:
    float titleSize{ 100.0f };
    float descriptionSize{ 60.0f };
    float restOfInfo{ 50.0f };

    float padding{ 100.0f };
    float yDistPadding{ 50.0f };
    float bounding_box_width{ 1618.0f };

    // positions
    float   spacing{ 50.0f };
    float   xPos{ 1354.0f };
    float   yPos{ 100 + padding };
    float   xPosImg{ 150.0f };
    ofVec2f titlePos{ ofVec2f( xPos, yPos ) };
    ofVec2f drawerPos{ ofVec2f( xPosImg, yPos ) };
    ofVec2f materialCatPos{ ofVec2f( xPosImg, 1188 ) };
    ofVec2f desPos{ ofVec2f( xPos, titlePos.y + yDistPadding + spacing ) };
};
