#pragma once

#include "AppStructs.h"
#include "ofMain.h"
#include "ofxTimeMeasurements.h"
#include "ofxTuio.h"
#include "ofxScreenSetup.h"
#include "ofxFontStash.h"

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
    float mImgSpeed = 1.0f;

    //qr code
    string cfc = "Center for Craft"; 
    string qr_scan = "QR Code Scanned"; 
    string title = cfc; 
    float scanTime = 0.0f; 

    // mouse 
    int mX =0.0f; 
    int mY = 0.0f;

    //images
    vector<ofImage> images; 
    vector<float>   imgPoses;
    float maxHeight = 400.0f;

    //screen
    void setupChanged( ofxScreenSetup::ScreenSetupArg &arg );
    ofxScreenSetup ss;

    //font
    ofxFontStash font;
    string       paragraph = "At vero eos et accusamus et iusto odio dignissimos ducimus qui blanditiis praesentium voluptatum deleniti atque corrupti quos dolores et quas molestias excepturi sint occaecati cupiditate non provident, similique sunt in culpa qui officia deserunt mollitia animi, id est laborum et dolorum fuga. Et harum quidem rerum facilis est et expedita distinctio. Nam libero tempore, cum soluta nobis est eligendi optio cumque nihil impedit quo minus id quod maxime placeat facere possimus."; 
    
    string activeQr = "";
    string newQr = "";
};
