//
//  CloseButton.hpp
//  interfaceExample
//
//  Created by Nicole Yi Messier
//
//

#pragma once

#include "CFCStructs.h"
#include "ofEvent.h"
#include "ofMain.h"
#include "ofxInterface.h"

class InfoWindow;
class ScreenObject;

class CloseButton : public ofxInterface::Node {
  public:
    CloseButton();
    ~CloseButton();

    void setup( ofVec2f size, ofVec2f position );
    void update( float dt );
    void draw();

    // setters
    void setAlpha( float alpha ){ mAlpha = alpha; };

    // listeners
    void onTouchDown( ofxInterface::TouchEvent &event );
    void onTouchUp( ofxInterface::TouchEvent &event );
    void onClick( ofxInterface::TouchEvent &event );

    // events
    ofEvent<CFC::ScreenObjectData> eventCloseClicked;

  private:
    ofImage img;
    float   mAlpha{ 0 };

    ofColor color;
    ofColor highlightColor;

    bool    CB_draw;
    bool    CB_touched;
    ofVec2f touchAnchor;
};