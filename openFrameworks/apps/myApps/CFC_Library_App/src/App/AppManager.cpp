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
    FontManager::one().setupFonts();

    //! Add listeners
    ofAddListener( ofEvents().keyReleased, this, &AppManager::onKeyPressed );
    ofAddListener( mAnim.animFinished, this, &AppManager::onAnimFinished );
    ofxNotificationCenter::one().addObserver( this, &AppManager::onContentStateFinished, "FINISHED_CONTENT_ID" );

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
    setAppState( AppState::IDLE );
}


void AppManager::update( float dt )
{
    mAnim.update( dt );

    switch( mAppState ) {
    case AppState::IDLE:
    case AppState::WIPE_START: {
        mVideo.update();
        break;
    }
    case AppState::WIPE: {
        break;
    }
    case AppState::LOADING_HAIKU: {
        mWipe.update( dt );
        numbers.update( dt );
        fact.update( dt );
        textB.update( dt );
        break;
    }
    case AppState::ANIMATE_HAIKU: {
        haiku.update( dt );
        break;
    }
    case AppState::WIPE_HAIKU: {
        haiku.update( dt );
        break;
    }
    default:
        break;
    }
}


void AppManager::draw()
{


    switch( mAppState ) {
    case AppState::IDLE:
    case AppState::WIPE_START: {
        ofSetColor( 255 );
        mVideo.draw( 0.0f, 0.0f );

        if( mWipe.getTexture().isAllocated() ) {
            mWipe.getTexture().draw( 0, 0 );
        }
        break;
    }
    case AppState::WIPE: {


        ofSetColor( LinkedColors::lightBlue );
        ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );

        ofSetColor( 255 );
        if( mWipe.getTexture().isAllocated() ) {
            mWipe.getTexture().draw( 0, 0 );
        }

        /* if( mWipeBackwards.getTexture().isAllocated() && mHaikuState == HAIKU::HAIKU_OUT ) {
             mWipeBackwards.getTexture().draw( 0, 0 );
         }*/
        break;
    }
    case AppState::LOADING_HAIKU: {

        numbers.draw();
        fact.draw();
        textB.draw();

        gptAnims.drawEngagingChatGpt();
        break;
    }
    case AppState::ANIMATE_HAIKU: {
        drawHaiku();
        break;
    }
    case AppState::WIPE_HAIKU: {

        ofSetColor( 255 );
        mVideo.draw( 0.0f, 0.0f );

        if( mWipeBackwards.getTexture().isAllocated() && mHaikuState == HAIKU::HAIKU_OUT ) {
            mWipeBackwards.getTexture().draw( 0, 0 );
        }
        break;
    }
    default:
        break;
    }
}

void AppManager::drawHaiku()
{
    ofSetColor( LinkedColors::darkPurple );

    if( mHaikuState == HAIKU::HAIKU_TYPING )
        ofDrawRectangle( 0, ofGetHeight() - ofGetHeight() * mAnim.getCurrentValue(), ofGetWidth(), ofGetHeight() );
    else
        ofDrawRectangle( 0, 0, ofGetWidth(), ofGetHeight() );

    haiku.draw( LinkedColors::lightBlue );


    if( cup.getTexture().isAllocated() ) {
        ofSetColor( 255 );
        cup.getTexture().draw( 0, 0 );
    }


    if( mWipeBackwards.getTexture().isAllocated() && mHaikuState == HAIKU::HAIKU_OUT ) {
        mWipeBackwards.getTexture().draw( 0, 0 );
    }

    gptAnims.drawHaikuBy();
}

#pragma mark STATE MGMT

void AppManager::setAppState( AppState appState )
{
    mAppState = appState;

    switch( mAppState ) {
    case AppState::IDLE: {
        current_id = -1;
        break;
    }
    case AppState::WIPE_START: {
        mWipe.seekToFrame( 0 );
        mWipe.play();

        startInternalTimer( .6f );
        break;
    }
    case AppState::WIPE: {
        startInternalTimer( 0.5f );
        break;
    }
    case AppState::LOADING_HAIKU: {
        mTimeStamp = mVideo.getPosition();
        mVideo.stop();

        setLoadingHaikuState( LOADING_HAIKU::SCREEN_A_IN );
        gptAnims.animateEng( 5.0f );
        break;
    }
    case AppState::ANIMATE_HAIKU: {
        haiku.startAnimation( 1.0f );
        seHaikuState( HAIKU::HAIKU_TYPING );
        startInternalTimer( 1.0f );
        gptAnims.animateHaikuIn();
        break;
    }
    case AppState::WIPE_HAIKU: {
        mVideo.play();
        mVideo.setPosition( mTimeStamp );
        startInternalTimer( .5f );
        break;
    }
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

void AppManager::setLoadingHaikuState( LOADING_HAIKU HaikuState )
{
    mLoadingState = HaikuState;
    switch( mLoadingState ) {
    case LOADING_HAIKU::SCREEN_A_IN: {
        ofSetBackgroundColor( LinkedColors::lightBlue );
        fact.startAnimation( true );
        numbers.startAnimation( true );
        startInternalTimer( 2.0f );
        break;
    }
    case LOADING_HAIKU::SCREEN_A_OUT: {
        fact.animateOut();
        numbers.animateOut();
        startInternalTimer( 2.0f );
        break;
    }
    case LOADING_HAIKU::SCREEN_B_IN: {
        textB.startAnimation( false );
        startInternalTimer( 4.0f );
        break;
    }
    case LOADING_HAIKU::SCREEN_B_OUT: {
        textB.animateOut();
        startInternalTimer( 2.0f );
        gptAnims.animateEng( 0.0f );
        break;
    }
    default:
        break;
    }
}

void AppManager::seHaikuState( HAIKU HaikuState )
{
    mHaikuState = HaikuState;
    switch( mHaikuState ) {
    case HAIKU::HAIKU_TYPING: {
        break;
    }
    case HAIKU::HAIKU_STEADY: {
        // startInternalTimer( 1.0f );
        break;
    }
    case HAIKU::CALL_TO_ACTION: {
        cup.seekToFrame( 0 );
        cup.play();
        startInternalTimer( 10.0f );
        break;
    }
    case HAIKU::HAIKU_OUT: {
        mWipeBackwards.seekToFrame( 0 );
        mWipeBackwards.play();
        startInternalTimer( .5f );
        break;
    }
    default:
        break;
    }
}

string AppManager::getAppState()
{

    switch( mAppState ) {
    case AppState::IDLE:
        return "AppState::IDLE";
    case AppState::WIPE_START:
        return "AppState::WIPE_START";
    case AppState::LOADING_HAIKU:
        return "AppState::LOADING_HAIKU";
    case AppState::ANIMATE_HAIKU:
        return "AppState::ANIMATE_HAIKU";
    default:
        return "";
    }
}

void AppManager::nextAppState()
{
    int count = static_cast<int>( mAppState );
    int max = static_cast<int>( AppState::NUM_RENDER_STATES );
    ( count < ( max - 1 ) ) ? count++ : count = 0;
    setAppState( static_cast<AppState>( count ) );
}

void AppManager::nextLoadingState()
{
    int count = static_cast<int>( mLoadingState );
    int max = static_cast<int>( LOADING_HAIKU::NUM_LOADING_HAIKU_STATES );
    ( count < ( max - 1 ) ) ? count++ : count = 0;
    setLoadingHaikuState( static_cast<LOADING_HAIKU>( count ) );
}

void AppManager::nextHaikuState()
{
    int count = static_cast<int>( mHaikuState );
    int max = static_cast<int>( HAIKU::NUM_HAIKU_STATES );
    ( count < ( max - 1 ) ) ? count++ : count = 0;
    seHaikuState( static_cast<HAIKU>( count ) );
}


#pragma mark UTILS

void AppManager::onKeyPressed( ofKeyEventArgs &arg )
{
    switch( arg.key ) {
    case 'n': {
        haiku.setText( "This is a test." );
        nextAppState();
        break;
    }
    case 'b': {

        mWipeBackwards.seekToFrame( 0 );
        mWipeBackwards.play();
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
    case AppState::IDLE: {
        break;
    }
    case AppState::WIPE_START: {
        setAppState( AppState::WIPE );
        break;
    }
    case AppState::WIPE: {
        setAppState( AppState::LOADING_HAIKU );
        break;
    }
    case AppState::LOADING_HAIKU: {
        if( mLoadingState != LOADING_HAIKU::SCREEN_B_OUT ) {
            nextLoadingState();
        }
        else
            setAppState( AppState::ANIMATE_HAIKU );
        break;
    }
    case AppState::ANIMATE_HAIKU: {
        if( mHaikuState == HAIKU::HAIKU_TYPING ) {
            return;
        }
        else if( mHaikuState != HAIKU::HAIKU_OUT ) {
            nextHaikuState();
        }
        else
            setAppState( AppState::WIPE_HAIKU );
        break;
    }
    case AppState::WIPE_HAIKU: {
        setAppState( AppState::IDLE );
        break;
    }
    default:
        break;
    }
}

void AppManager::onOSCHaikuIncoming( ofxNotificationCenter::Notification &n )
{

    int    id = n.data["id"];
    string haikuText = n.data["haiku"];
    string factText = n.data["fact"];

    if( id != current_id && mAppState == AppState::IDLE ) {
        setAppState( AppState::WIPE_START );
        haiku.setText( haikuText );


        if( ofIsStringInString( factText, ":" ) ) {
            vector<string> result = ofSplitString( factText, ":" );

            if( result.size() == 2 ) {
                fact.setText( result[0] );
                fact.setText( result[1] );
            }
        }
        else if( !factText.size() ) {
            fact.setText( "This means I am not getting a fact from the backend." );
        }
        else {
            fact.setText( factText );
            numbers.setText( "" );
        }
        current_id = id;
    }
    ofLogNotice( "AppManager" ) << "incoming haiku " + id << ": " << haikuText;
}

void AppManager::onContentStateFinished( ofxNotificationCenter::Notification &n )
{
    nextHaikuState();
}
