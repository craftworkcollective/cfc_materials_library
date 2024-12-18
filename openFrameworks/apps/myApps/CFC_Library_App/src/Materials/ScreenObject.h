//
//  ScreenObject.hpp
//
//  Created by Nicole Yi Messier
//

#pragma once

#include "AppSettings.h"
#include "AtlasManager.h"
#include "CFCObject.h"
#include "CFCStructs.h"
#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxInterface.h"

class ScreenObject : public ofxInterface::Node {
  public:
    ScreenObject();
    ~ScreenObject();

    // Setup and Update
    void setup( CFCObject *cfcObject, int uid);
    void setStartPosition( ofVec2f pos );
    void update( float dt );
    void updateDrift();
    void draw();
    void drawDebug();
    void calcCrop( float widthPerc );


    // Texture Thumbnail
    void drawInBatch();
    void setupTexture();

    // Attributes /////////////////////////////////////////////////
    void    setColor( ofColor _color ) { mColor = _color; };
    void    setOnScreen( bool onScreen ) { mOnScreen = onScreen; };
    void    setMaxSize( ofVec2f size );
    void    setReplaceData( bool replace ) { mReplaceData = replace; };
    void    setData( CFCObject *cfcObject );
    void    setMDrift( bool drift ) { mDrifting = drift; }

    // getters
    ofColor getColor() { return mColor; };
    bool    getOnScreen() { return mOnScreen; };
    bool    getReplaceData() {return mReplaceData; }; 
    string  getTextureFile() { return textureFile; };
    ofVec2f getTempPos() { return mPos; }


    // touch
    void onTouchDown( ofxInterface::TouchEvent &event );
    void onTouchUp( ofxInterface::TouchEvent &event );
    void onClick( ofxInterface::TouchEvent &event );

    
    // events
    ofEvent<CFC::ScreenObjectData> eventSoClicked;

  private:
    // ATTRIBUTES
    int             mUid{ 0 };
    ofxAnimatableFloat animVal;
    ofVec2f            mPos;
    ofVec2f            startPos;
    ofVec2f            mTargetSize;
    ofVec2f            maxSize;
    ofVec2f            drift;
    ofVec2f            mTouchAnchor;
    ofColor            mColor{ ofColor::white };
    float              mAlpha{ 255.0f };
    bool               mOnScreen{ true };
    bool               mReplaceData {false}; 
    bool               mDrifting{ false };

    // Material Data
    CFCObject *mCfcObject;

    // texture
    string                                textureFile;
    TextureAtlasDrawer::TexQuad           texQuad;
    TextureAtlasDrawer::TexQuad           targetTexQuad;
    TextureAtlasDrawer::TextureDimensions td;
    bool                                  drifting = false;

    // touch
    bool mTouched{ false };

};
