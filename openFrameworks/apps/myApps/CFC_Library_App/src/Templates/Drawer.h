//
//  AtlasManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once
#include "AppSettings.h"
#include "AtlasManager.h"
#include "CFCColors.h"
#include "FontManager.h"
#include "ofxInterface.h"
#include "ofxTimeMeasurements.h"

class Drawer : public ofxInterface::Node {
  public:
    Drawer();
    ~Drawer();


    void setup();
    void draw();
    void drawInBatch();
    void update( float dt );

    void setTitle( string txt ) { mTitle = txt; };
    void setDescription( string txt ) { mDescription = txt; };
    void setCompositeMaterials( string txt ) { mCompositeMaterials = txt; };
    void setPrimaryUses( string txt ) { mPrimaryUses = txt; };
    void setAdditionalMaterials( string txt ) { mAdditionalMaterials = txt; };
    void setCategory( string txt ) { mCategory = txt; };
    void setDrawer( string txt ) { mDrawer = txt; };
    void setLogoPath( string path ) { mLogoImgPath = path; };
    void setMaterialImgPath( string path ) { mMaterialImgPath = path; }

    TextureAtlasDrawer::TexQuad getParalelogramForRect( const ofRectangle &r );

  private:
    string mTitle{ "Mock Rib Pocket Stretch Sensor" };
    string mDescription{
        "A knitted stretch sensor can be connected to an output such as a light to decrease or increase the brightness. The more a "
        "resistive material is stretched, the more its resistance will increase because it has more surface area to cover. This sensor was "
        "made with conductive and non-conductive yarns using a Brother KH-910 knitting machine."
    };
    string mCompositeMaterials{ "Conductive Yarn, Non-Conductive Yarn" };
    string mPrimaryUses{ "" };
    string mAdditionalMaterials{ "" };
    string mCategory{ "" };
    string mDrawer{ "" };
    string mMaterialImgPath{ "" };
    string mLogoImgPath{ "" };

    ofVec2f mSize{ ofVec2f( 3072.0f, 1388.0f ) };
    ofVec2f imgSize{ ofVec2f( 3072.0f, 1388.0f ) };
    float   padding{ 100.0f };
};