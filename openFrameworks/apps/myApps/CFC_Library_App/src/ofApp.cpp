#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    app.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{

    float dt;

    dt = 1.0f / mFrameRate;

    if( ofGetFrameRate() < 45 ) {
        dt = ofClamp( ofGetLastFrameTime(), 1.0f / 60.0f, 1.0f / 30.0f );
    }

    app.update( dt );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    app.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseExited( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::windowResized( int w, int h )
{
}

//--------------------------------------------------------------
void ofApp::gotMessage( ofMessage msg )
{
}

//--------------------------------------------------------------
void ofApp::dragEvent( ofDragInfo dragInfo )
{
}
