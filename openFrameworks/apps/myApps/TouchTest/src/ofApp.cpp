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

    // setup a listener for changed window modes
    ofAddListener( ss.setupChanged, this, &ofApp::setupChanged );
    ss.setup( ofGetWidth(), ofGetHeight(), ofxScreenSetup::WINDOWED );

    // set up touch
    tuioClient.start( 3333 );
    ofAddListener( tuioClient.cursorAdded, this, &ofApp::tuioAdded );
    ofAddListener( tuioClient.cursorRemoved, this, &ofApp::tuioRemoved );
    ofAddListener( tuioClient.cursorUpdated, this, &ofApp::tuioUpdated );

    // setup font
    font.setup( "p22.ttf", 1.0, 1024, false, 8, 1.5 );

    // images
    string      path = "images";
    ofDirectory dir( path );
    dir.allowExt( "jpg" );
    dir.listDir();

    // go through and print out all the paths
    float total_width = 0.0f;
    for( int i = 0; i < dir.size(); i++ ) {
        ofImage img;
        img.load( dir.getPath( i ) );
        images.push_back( img );
        ofLogNotice( dir.getPath( i ) );

        float width = img.getWidth() / img.getHeight() * maxHeight;
        total_width += width;

        imgPoses.push_back( 0.0f );
    }

    float spacing = ( ofGetWidth() - total_width ) / ( images.size() + 2 );
    float newWidth = 0.0f;
    for( int i = 0; i < imgPoses.size(); i++ ) {
        imgPoses[i] = spacing * ( i + 1 ) + newWidth;

        float width = images[i].getWidth() / images[i].getHeight() * maxHeight;
        newWidth += width;
    }
}

//--------------------------------------------------------------
void ofApp::update()
{
    tuioClient.getMessage();

    switch( mState ) {
    case AnimationStates::LINES_VERTICAL:
    case AnimationStates::LINES_HORIZONTAL: {
        for( int i = 0; i < linePoses.size(); i++ ) {
            linePoses[i] += mLineSpeed;
        }

        break;
    }
    case AnimationStates::IMAGE_PARTICLES:
        break;
    default:
        break;
    }

    for( int i = 0; i < imgPoses.size(); i++ ) {
        imgPoses[i] += mImgSpeed;

        if( imgPoses[i] > ofGetWidth() )
            imgPoses[i] = 0.0f;
    }

    if( ofGetElapsedTimef() - scanTime > 2.0f )
        title = cfc;
}

//--------------------------------------------------------------
void ofApp::draw()
{
    tuioClient.drawCursors();

    switch( mState ) {
    case AnimationStates::LINES_VERTICAL: {

        for( int i = 0; i < linePoses.size() - 1; i++ ) {
            ofSetColor( lineColor );
            ofDrawLine( linePoses[i], 0, linePoses[i], ofGetHeight() );

            if( linePoses[i] > ofGetWidth() )
                linePoses[i] = 0.0f;
        }

        break;
    }
    case AnimationStates::LINES_HORIZONTAL: {

        for( int i = 0; i < linePoses.size() - 1; i++ ) {
            ofSetColor( lineColor );
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


    ofDrawBitmapString( "Press 'w' to cycle through window modes\n activeQR: " + activeQr + ".", 20, 20 );


    font.draw( title, 100, 100.0f, 100.0f );
    ofRectangle column;
    int         numLines = 0;
    bool        wordsWereCropped;
    column = font.drawMultiLineColumn( paragraph, /*string*/
        36,                                       /*size*/
        100.0f, 200.0f,                           /*where*/
        1500.0f,                                  /*column width*/
        numLines,                                 /*get back the number of lines*/
        false,                                    /* if true, we wont draw (just get bbox back) */
        9,                                        /* max number of lines to draw, crop after that */
        true,                                     /*get the final text formatting (by adding \n's) in the supplied string;
                                                   BE ARWARE that using TRUE in here will modify your supplied string! */
        &wordsWereCropped,                        /* this bool will b set to true if the box was to small to fit all text*/
        false                                     /*centered*/
    );

    for( int i = 0; i < imgPoses.size(); i++ ) {
        ofSetColor( 255 );
        images[i].draw( imgPoses[i], 500.0f, images[i].getWidth() / images[i].getHeight() * maxHeight, maxHeight );
    }

    ofDrawCircle( mX, mY, 50.0f );
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
    ofLogNotice() << "key: " << ( key ); 

    if( key != 'z' && key != 13 ) {
  
        newQr.push_back(char(key));
    }
    else if( key == 'z' )
    {
        activeQr = newQr; 
        newQr = "";

    }

   

    /*
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
    case '9':
        scanTime = ofGetElapsedTimef();
        title = qr_scan;
        break;
    case 'n': {
        // next state
        int count = static_cast<int>( mState );
        int max = static_cast<int>( AnimationStates::NUM_ANIMATION_STATES );
        ( count < ( max - 1 ) ) ? count++ : count = 0;
        setAppState( static_cast<AnimationStates>( count ) );
        break;
    }
    case 'w': {
        ss.cycleToNextScreenMode();
        break;
    }
    default:
        break;
    }
    */
}

void ofApp::setupChanged( ofxScreenSetup::ScreenSetupArg &arg )
{

    setAppState( mState );
    ofLogNotice() << "ofxScreenSetup setup changed from " << ss.stringForMode( arg.oldMode ) << " (" << arg.oldWidth << "x" << arg.oldHeight
                  << ") "
                  << " to " << ss.stringForMode( arg.newMode ) << " (" << arg.newWidth << "x" << arg.newHeight << ")";
}


//--------------------------------------------------------------
void ofApp::mouseMoved( int x, int y )
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged( int x, int y, int button )
{
    mX = x;
    mY = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed( int x, int y, int button )
{
    mX = x;
    mY = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased( int x, int y, int button )
{
    mX = x;
    mY = y;
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
