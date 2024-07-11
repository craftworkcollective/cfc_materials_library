//
//  AtlasManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once
#include "AppSettings.h"
#include "AtlasManager.h"
#include "CFCColors.h"
#include "CloseButton.h"
#include "FontManager.h"
#include "ofxAnimatableFloat.h"
#include "ofxInterface.h"
#include "ofxTimeMeasurements.h"

class DrawerWindow : public ofxInterface::Node {
  public:
    DrawerWindow();
    ~DrawerWindow();


    void setup();
    void draw();
    void drawInBatch( float alpha );
    void update( float dt );
    void passData( CFC::DrawerData data );


    // setters
    void setCategory( string txt ) { mCategory = txt; };
    void setDrawer( string txt ) { mDrawer = txt; };
    void setState( CFC::DrawerState state );

    // texture
    void calcCrop( float widthPerc );

  private:
    string mCategory{ "" };
    string mDrawer{ "" };

    ofVec2f imgSize{ ofVec2f( 1188, 1188 ) }; // image
    ofVec2f mSize{ ofVec2f( 3072.0f, 1388.0f ) };
    float   padding{ 100.0f };

    // UI
    CloseButton *closeBtn;
    void         onCloseBtnClicked( CFC::ScreenObjectData &data );

    // state
    CFC::DrawerState mState{ CFC::DrawerState::NOT_ACTIVE };


    // texture
    string                                textureFile;
    TextureAtlasDrawer::TexQuad                   texQuad;
    TextureAtlasDrawer::TexQuad                   targetTexQuad;
    TextureAtlasDrawer::TextureDimensions         td;
    bool                                  drifting = false;

    // animations
    ofxAnimatableFloat alpha;
    float              duration = 0.5;
    void               onAnimValFinished( ofxAnimatable::AnimationEvent &event );
};