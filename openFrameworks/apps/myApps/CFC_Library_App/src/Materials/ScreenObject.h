//
//  ScreenObject.hpp
//
//  Created by Nicole Yi Messier
//

#pragma once

#include "AtlasManager.h"
#include "CFCStructs.h"
#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxInterface.h"

class ScreenObject : public ofxInterface::Node {
  public:
    ScreenObject();
    ~ScreenObject();

    // Setup and Update
    void setup( ofVec2f maxSize, ofVec2f position );
    void update( float dt );
    void draw();
    void drawDebug();

    // Texture Thumbnail 
    void drawInBatch();
    void setupTexture();

  private:
    // Animatable Floats
    ofxAnimatableFloat animVal;

    // texture
    string                      textureFile;
    TextureAtlasDrawer::TexQuad texQuad;
    TextureAtlasDrawer::TexQuad targetTexQuad;
    bool                        drifting = false;
};
