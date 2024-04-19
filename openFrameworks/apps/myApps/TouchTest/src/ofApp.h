#pragma once

#include "AppStructs.h"
#include "ofMain.h"
#include "ofxTimeMeasurements.h"
#include "ofxTuio.h"

using namespace App;

class ofApp : public ofBaseApp {

  public:
    void setup();
    void update();
    void draw();

    void keyPressed( int key );
    void keyReleased( int key );
    void mouseMoved( int x, int y );
    void mouseDragged( int x, int y, int button );
    void mousePressed( int x, int y, int button );
    void mouseReleased( int x, int y, int button );
    void mouseEntered( int x, int y );
    void mouseExited( int x, int y );
    void windowResized( int w, int h );
    void dragEvent( ofDragInfo dragInfo );
    void gotMessage( ofMessage msg );
    void setAppState( AnimationStates state );

    AnimationStates mState;

    // line animations
    float         mLineSpeed = 2.5f;
    int           numLines = 25;
    ofColor       lineColor;
    vector<float> linePoses;

    // touch
    void tuioAdded( ofxTuioCursor &tuioCursor );
    void tuioRemoved( ofxTuioCursor &tuioCursor );
    void tuioUpdated( ofxTuioCursor &tuioCursor );
    ofxTuioClient tuioClient;
};
