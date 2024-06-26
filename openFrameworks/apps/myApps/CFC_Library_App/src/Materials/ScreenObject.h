//
//  ScreenObject.hpp
//
//  Created by Nicole Yi Messier 
//

#pragma once

#include "CFCStructs.h"
#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ofxInterface.h"
#include "AtlasManager.h"

class ScreenObject : public ofxInterface::Node {
  public:
    ScreenObject();
    ~ScreenObject();

    // Setup and Update
    void setup( ofVec2f maxSize, ofVec2f position);
    void update( float dt );
    void draw();
    void drawDebug();
  

  private:

    // Animatable Floats
    ofxAnimatableFloat animVal;
};
