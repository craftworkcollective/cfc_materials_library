#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    //! general setup
    ofSetFrameRate( 60 );
    ofSetVerticalSync( true );
    ofEnableAlphaBlending();
    ofBackground( 0 );
    lineColor = ofColor( 255 );

    // FPS
    TIME_SAMPLE_SET_FRAMERATE( 60.0f ); // specify a target framerate

    // ANIMATION STATE
    mState = AnimationStates::LINES_VERTICAL;

    // set up line animations
    for( int i = 0; i < numLines + 1; i++ ) {
        linePoses.push_back( ofGetWidth() / numLines * ( i ) );
    }

    // set up touch
    tuioClient.start( 3333 );

    ofAddListener( tuioClient.cursorAdded, this, &ofApp::tuioAdded );
    ofAddListener( tuioClient.cursorRemoved, this, &ofApp::tuioRemoved );
    ofAddListener( tuioClient.cursorUpdated, this, &ofApp::tuioUpdated );
}

//--------------------------------------------------------------
void ofApp::update()
{
    tuioClient.getMessage();

    switch( mState ) {
    case AnimationStates::LINES_VERTICAL:
        break;
    case AnimationStates::LINES_HORIZONTAL:
        break;
    case AnimationStates::IMAGE_PARTICLES:
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    tuioClient.drawCursors();

    switch( mState ) {
    case AnimationStates::LINES_VERTICAL: {

        for( int i = 0; i < linePoses.size() - 1; i++ ) {
            ofSetColor( lineColor );
            linePoses[i] += mLineSpeed;
            ofDrawLine( linePoses[i], 0, linePoses[i], ofGetHeight() );

            if( linePoses[i] > ofGetWidth() )
                linePoses[i] = 0.0f;
        }

        break;
    }
    case AnimationStates::LINES_HORIZONTAL: {

        for( int i = 0; i < linePoses.size() - 1; i++ ) {
            ofSetColor( lineColor );
            linePoses[i] += mLineSpeed;
            ofDrawLine( 0, linePoses[i], ofGetWidth(), linePoses[i] );

            if( linePoses[i] > ofGetHeight() )
                linePoses[i] = 0.0f;
        }

        break;
    }
    case AnimationStates::IMAGE_PARTICLES:
        break;
    default:
        break;
    }
}

void ofApp::setAppState( AnimationStates state )
{
    mState = state;
    switch( mState ) {
    case AnimationStates::LINES_VERTICAL: {
        for( int i = 0; i < numLines + 1; i++ ) {
            linePoses[i] = ofGetWidth() / numLines * ( i );
        }
        break;
    }
    case AnimationStates::LINES_HORIZONTAL: {
        for( int i = 0; i < numLines + 1; i++ ) {
            linePoses[i] = ofGetHeight() / numLines * ( i );
        }
        break;
    }
    case AnimationStates::IMAGE_PARTICLES:
        break;
    default:
        break;
    }
}

void ofApp::tuioAdded( ofxTuioCursor &tuioCursor )
{
    ofPoint loc = ofPoint( tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight() );
    cout << "Point n" << tuioCursor.getSessionId() << " add at " << loc << endl;
}

void ofApp::tuioRemoved( ofxTuioCursor &tuioCursor )
{
    ofPoint loc = ofPoint( tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight() );
    cout << "Point n" << tuioCursor.getSessionId() << " updated at " << loc << endl;
}

void ofApp::tuioUpdated( ofxTuioCursor &tuioCursor )
{
    ofPoint loc = ofPoint( tuioCursor.getX() * ofGetWidth(), tuioCursor.getY() * ofGetHeight() );
    cout << "Point n" << tuioCursor.getSessionId() << " remove at " << loc << endl;
}

//--------------------------------------------------------------
void ofApp::keyPressed( int key )
{
}

//--------------------------------------------------------------
void ofApp::keyReleased( int key )
{

    switch( key ) {
    case '1':
        ofBackground( 0 );
        lineColor = ofColor( 255 );
        break;
    case '2':
        ofBackground( 255 );
        lineColor = ofColor( 0 );
        break;
    case '3':
        ofBackground( 236, 52, 46 );
        lineColor = ofColor( 0 );
        break;
    case '4':
        ofBackground( 253, 234, 227 );
        lineColor = ofColor( 0 );
        break;
    case 'n': {
        // next state
        int count = static_cast<int>( mState );
        int max = static_cast<int>( AnimationStates::NUM_ANIMATION_STATES );
        ( count < ( max - 1 ) ) ? count++ : count = 0;
        setAppState( static_cast<AnimationStates>( count ) );
        break;
    }
    default:
        break;
    }
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
