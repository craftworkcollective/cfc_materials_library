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

    // --- FONT ATTRIBUTES --- //
    float lineHeightPercent = 1.0f;
    int   texDimension = 1024;
    bool  createMipMaps = false;
    int   intraCharPadding = 10;
    float dpiScale = 1.5f;


    void setupFonts();
    void update();

    void drawTitle( string title );
    void drawBody( string text );

  private:
    float   titleSize{ 100.0f };
    float   descriptionSize{ 60.0f };
    float   padding{ 100.0f };
    float   yDistPadding{ 50.0f };
    float   xPos{ 1354.0f };
    float   bounding_box_width{ 1618.0f};
    ofVec2f titlePos{ ofVec2f( xPos, 100 + +padding ) };
    ofVec2f desPos{ ofVec2f( xPos, titlePos.y + yDistPadding ) };
};
