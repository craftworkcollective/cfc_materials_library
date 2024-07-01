#include "FontManager.h"

void FontManager::setupFonts()
{
    reg.setup( "fonts/p22.ttf", lineHeightPercent, texDimension, createMipMaps, intraCharPadding, dpiScale );
}

void FontManager::update()
{
}

void FontManager::drawTitle( string title ){
    
    reg.draw( title, titleSize, titlePos.x, titlePos.y );
};


void FontManager::drawBody( string text )
{
    
    int  numLines;
    bool wordsWereCropped;

    reg.drawMultiLineColumn( text,      // string
        descriptionSize,                 // size
        desPos.x, desPos.y,          // position
        bounding_box_width,              // column width
        numLines,                        // get back the number of lines
        false,                           // true would not draw, just get back the rectangle
        5,                               // max number of lines
        true,                            // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,               // this is set to true if the box was too small to fit all of the text
        false );
        
};
