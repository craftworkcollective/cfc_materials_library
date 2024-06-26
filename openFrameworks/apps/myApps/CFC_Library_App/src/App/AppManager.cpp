//
//  AppManager.cpp
//
//  Created by Nicole Yi Messier
//

#include "AppManager.h"

using namespace CFC;

void AppManager::setup()
{
    //! general setup
    ofSetFrameRate( 60 );
    ofSetVerticalSync( true );
    ofEnableAlphaBlending();
    ofBackground( 0 );

    // set up configs
    configs().setup();

    // log to file
    if( configs().getLogToFile() ) {
        string path = log_path + "/" + ofGetTimestampString() + ".log";
        ofLogToFile( path, true );
    }
    else {
        ofLogToConsole();
    }

    // screen
    // start the app in windowed mode
    int num = configs().getWindowMode();
    ss.setup( ofGetWidth(), ofGetHeight(), static_cast<ofxScreenSetup::ScreenMode>( num ) );


    // setup a listener for changed window modes
    ofAddListener( ss.setupChanged, this, &AppManager::setupChanged );

    // turn mouse on/off
    if( !configs().getMouseOn() )
        ofHideCursor();

    // setup transition
    int   numThreads = 4;
    int   buffer = 60; // MAX(1.5 * numThreads, 8);
    float framerate = 60;
    bool  loops = false;
    bool  useTexture = true;

    // setup fonts
    // FontManager::one().setupFonts();

    //! Add listeners
    ofAddListener( ofEvents().keyReleased, this, &AppManager::onKeyPressed );
    ofAddListener( mAnim.animFinished, this, &AppManager::onAnimFinished );
    // ofxNotificationCenter::one().addObserver( this, &AppManager::onContentStateFinished, "FINISHED_CONTENT_ID" );

    // Reset window size
    ofSetWindowShape( configs().getAppSize().x, configs().getAppSize().y );

    // Setup Managers
    setupManagers();
}


void AppManager::setupChanged( ofxScreenSetup::ScreenSetupArg &arg )
{

    ofLogNotice() << "ofxScreenSetup setup changed from " << ss.stringForMode( arg.oldMode ) << " (" << arg.oldWidth << "x" << arg.oldHeight
                  << ") "
                  << " to " << ss.stringForMode( arg.newMode ) << " (" << arg.newWidth << "x" << arg.newHeight << ")";
}

void AppManager::setupManagers()
{
    setAppState( AppState::LOADING );
}


void AppManager::update( float dt )
{
    mAnim.update( dt );

    switch( mAppState ) {
    case AppState::LOADING:
        break;
    case AppState::ATTRACT:
        break;
    case AppState::DRAWER:
        break;
    case AppState::MATERIAL:
        break;
    default:
        break;
    }
}


void AppManager::draw()
{
    switch( mAppState ) {
    case AppState::LOADING:
        break;
    case AppState::ATTRACT:
        break;
    case AppState::DRAWER:
        break;
    case AppState::MATERIAL:
        break;
    default:
        break;
    }
}

#pragma mark STATE MGMT

void AppManager::setAppState( AppState appState )
{
    mAppState = appState;

     switch( mAppState ) {
    case AppState::LOADING:
        break;
    case AppState::ATTRACT:
        break;
    case AppState::DRAWER:
        break;
    case AppState::MATERIAL:
        break;
    default:
        break;
    }
    ofLogNotice( "AppManager" ) << "Set app state to " << getAppState();
}


void AppManager::startInternalTimer( float dur )
{
    mAnim.setCurve( AnimCurve::SWIFT_GOOGLE );
    mAnim.setDuration( dur );
    mAnim.animateFromTo( 0., 1. );
}

string AppManager::getAppState()
{

   switch( mAppState ) {
    case AppState::LOADING:
        return "AppState::LOADING";
    case AppState::ATTRACT:
        return "AppState::ATTRACT";
    case AppState::DRAWER:
        return "AppState::DRAWER";
    case AppState::MATERIAL:
        return "AppState::MATERIAL";
    default:
        return "";
    }
}

void AppManager::nextAppState()
{
    int count = static_cast<int>( mAppState );
    int max = static_cast<int>( AppState::NUM_STATES );
    ( count < ( max - 1 ) ) ? count++ : count = 0;
    setAppState( static_cast<AppState>( count ) );
}

#pragma mark UTILS

void AppManager::onKeyPressed( ofKeyEventArgs &arg )
{
    switch( arg.key ) {
    case 'n': {
        nextAppState();
        break;
    }
    case 'm': {
        ofShowCursor();
        break;
    }
    case 'h': {
        ofHideCursor();
        break;
    }
    case 's': {
        ss.cycleToNextScreenMode();
    }
    default:
        break;
    }
}

void AppManager::onAnimFinished( ofxAnimatable::AnimationEvent &args )
{
    switch( mAppState ) {
    case AppState::LOADING:
        break;
    case AppState::ATTRACT:
        break;
    case AppState::DRAWER:
        break;
    case AppState::MATERIAL:
        break;
    default:
        break;
    }
}


void AppManager::onContentStateFinished( ofxNotificationCenter::Notification &n )
{
   
}
