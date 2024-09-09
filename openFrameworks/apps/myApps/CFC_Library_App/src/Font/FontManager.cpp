#include "FontManager.h"

void FontManager::setupFonts()
{
    reg.setup( "fonts/NunitoSans_10pt-Regular.ttf", lineHeightPercent, texDimension, createMipMaps, intraCharPadding, dpiScale );
    medium.setup( "fonts/NunitoSans_10pt-Medium.ttf", lineHeightPercent, texDimension, createMipMaps, intraCharPadding, dpiScale );
    bold.setup( "fonts/NunitoSans_10pt-Bold.ttf", .9, texDimension, createMipMaps, intraCharPadding, dpiScale );
    semiBold.setup( "fonts/NunitoSans_10pt-SemiBold.ttf", lineHeightPercent, texDimension, createMipMaps, intraCharPadding, dpiScale );
}

void FontManager::update()
{
}

void FontManager::drawTitle( string title )
{
    bold.draw( title, titleSize, titlePos.x, titlePos.y );
};


void FontManager::drawDrawer( string drawer )
{
    int  numLines;
    bool wordsWereCropped;

    ofRectangle rec = reg.drawMultiLineColumn( ofToUpper( drawer ), // string
        titleSize,                                                  // size
        drawerPos.x, drawerPos.y,                                   // position
        bounding_box_width,                                         // column width
        numLines,                                                   // get back the number of lines
        false,                                                      // true would not draw, just get back the rectangle
        1,                                                          // max number of lines
        true,                                                       // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                                          // this is set to true if the box was too small to fit all of the text
        false );


    float paddingX = 40.0f;
    float paddingY = 40.0f;
    ofNoFill();
    ofDrawRectangle( drawerPos.x - paddingX/2, drawerPos.y - rec.getHeight() - paddingY/2, rec.getWidth() + paddingX, rec.getHeight() + paddingY);
    ofFill();
    // bold.draw( ofToUpper( drawer ), titleSize, drawerPos.x, drawerPos.y );
}

void FontManager::drawMaterialCategory( string txt )
{
    bold.draw( capitalizeFirstLetter( txt ), titleSize, materialCatPos.x, materialCatPos.y );
}

string FontManager::capitalizeFirstLetter( const std::string &input )
{
    if( input.empty() ) {
        return input; // Return empty string if input is empty
    }

    std::string result = input;
    result[0] = std::toupper( result[0] ); // Capitalize the first letter

    for( size_t i = 1; i < result.size(); ++i ) {
        result[i] = std::tolower( result[i] ); // Convert the rest to lowercase
    }

    return result;
}


void FontManager::drawBody( string description, string composite, string uses, string details )
{

    int  numLines;
    bool wordsWereCropped;

    ofRectangle rec = reg.drawMultiLineColumn( description, // string
        descriptionSize,                                    // size
        desPos.x, desPos.y,                                 // position
        bounding_box_width,                                 // column width
        numLines,                                           // get back the number of lines
        false,                                              // true would not draw, just get back the rectangle
        5,                                                  // max number of lines
        true,                                               // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                                  // this is set to true if the box was too small to fit all of the text
        false );

    ofVec2f compPos = ofVec2f( desPos.x, desPos.y + rec.getHeight() + spacing );
    ofVec2f usesPos = ofVec2f( desPos.x, 0 );
    ofVec2f detailsPos = ofVec2f( desPos.x, 0 );

    ofRectangle compRec, usesRec, detailsRec;
    float       titleSpacing = 40.0f;

    if( composite.size() > 0 ) {
        bold.draw( "Composite Materials", restOfInfo, compPos.x, compPos.y );
        compPos.y += titleSpacing;
        compRec = reg.drawMultiLineColumn(
            composite, restOfInfo, compPos.x, compPos.y, bounding_box_width, numLines, false, 5, true, &wordsWereCropped, false );
        usesPos.y = compPos.y + compRec.getHeight() + spacing;
    }
    else 
    {
        usesPos.y = compPos.y;
    }


    if( uses.size() > 0 ) {
        bold.draw( "Primary Uses", restOfInfo, usesPos.x, usesPos.y );
        usesPos.y += titleSpacing;

        usesRec = reg.drawMultiLineColumn(
            uses, restOfInfo, usesPos.x, usesPos.y, bounding_box_width, numLines, false, 5, true, &wordsWereCropped, false );
    }

    detailsPos.y = usesPos.y + usesRec.getHeight() + spacing;
    if( details.size() > 0 ) {
        bold.draw( "Additional Details", restOfInfo, detailsPos.x, detailsPos.y );
        detailsPos.y += titleSpacing;

        detailsRec = reg.drawMultiLineColumn(
            details, restOfInfo, detailsPos.x, detailsPos.y, bounding_box_width, numLines, false, 5, true, &wordsWereCropped, false );
    }
};

void FontManager::drawDrawerCategory( string text )
{
    bold.draw( text, titleSize, titlePosDrawer.x, titlePosDrawer.y );
};

void FontManager::drawDrawerMaterial( string text )
{
    reg.draw( text, titleSize, titlePosDrawer.x, titlePosDrawer.y );
};

void FontManager::drawDrawerCompany( string text )
{
    reg.draw( text, titleSize, titlePosDrawer.x, titlePosDrawer.y );
};


void FontManager::drawDrawerLabel( string text, float width )
{
    reg.draw( text, titleSize, drawerLabelDrawer.x - width, drawerLabelDrawer.y );
};

float FontManager::getDrawerLabelWidth( string text )
{
    return FontManager::one().reg.draw( text, titleSize, 0.0f, 0.0f );
}

void FontManager::drawDOMaterialCompany( string material, string company, float xpos )
{
    int  numLines;
    bool wordsWereCropped;

    ofVec2f pos = doPos;

    ofRectangle rec = bold.drawMultiLineColumn( material, // string
        restOfInfo,                                       // size
        pos.x, pos.y,                                     // position
        DOsize,                                           // column width
        numLines,                                         // get back the number of lines
        false,                                            // true would not draw, just get back the rectangle
        5,                                                // max number of lines
        true,                                             // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                                // this is set to true if the box was too small to fit all of the text
        false );

    ofRectangle rec1 = reg.drawMultiLineColumn( company, // string
        restOfInfo,                                      // size
        pos.x, pos.y + rec.getHeight() + 10.0f,          // position
        DOsize,                                          // column width
        numLines,                                        // get back the number of lines
        false,                                           // true would not draw, just get back the rectangle
        5,                                               // max number of lines
        true,                                            // get the final text formatting (by adding \n's) in the supplied string
        &wordsWereCropped,                               // this is set to true if the box was too small to fit all of the text
        false );
};
