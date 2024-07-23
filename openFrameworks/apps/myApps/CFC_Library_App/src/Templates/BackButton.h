//
//  BackButton.hpp
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

class BackButton : public ofxInterface::Node {
  public:
    BackButton();
    ~BackButton();

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
    ofEvent<CFC::ScreenObjectData> eventBackBtn;

  private:
    ofImage img;
    float   mAlpha{ 0 };

    ofColor color;
    ofColor highlightColor;

    bool    CB_draw;
    bool    CB_touched;
    ofVec2f touchAnchor;
};