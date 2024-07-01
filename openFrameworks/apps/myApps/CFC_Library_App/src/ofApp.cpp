#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    //! general setup
    ofSetFrameRate( 60 );
    ofSetVerticalSync( true );
    ofEnableAlphaBlending();
    ofBackground( 0 );

    // set up configs
    configs().setup();

    // set up the scene with width + height
    scene = new ofxInterface::Node();
    scene->setSize( configs().getAppSize().x, configs().getAppSize().y );
    scene->setName( "Scene" );

    // pass the scene to the touch manager
    ofxInterface::TouchManager::one().setup( scene );

    app = new AppManager();
    app->setup();
    app->setName( "APP" ); 
    app->setPosition( ofVec2f( 0.0f, 0.0f ) );
    app->setSize( ofVec2f( configs().getAppSize().x, configs().getAppSize().y ) );
    scene->addChild( app );
}

//--------------------------------------------------------------
void ofApp::update()
{

    float dt;

    dt = 1.0f / mFrameRate;

    if( ofGetFrameRate() < 45 ) {
        dt = ofClamp( ofGetLastFrameTime(), 1.0f / 60.0f, 1.0f / 30.0f );
    }

    ofxInterface::TouchManager::one().update();
    scene->updateSubtree( 1 / 60.0f, true );
}

//--------------------------------------------------------------
void ofApp::draw()
{
    scene->render();

    if( bShowDebug ) {
        scene->renderDebug();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
    if( key == 'd' )
        bShowDebug != bShowDebug;
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
    ofxInterface::TouchManager::one().touchMove( button, ofVec2f( x, y ) );
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    ofxInterface::TouchManager::one().touchDown( button, ofVec2f( x, y ) );
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    ofxInterface::TouchManager::one().touchUp( button, ofVec2f( x, y ) );
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
