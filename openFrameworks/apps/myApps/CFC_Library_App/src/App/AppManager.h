//
//  AppManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once

#include "AppSettings.h"
#include "CFCStructs.h"
#include "ofxNotificationCenter.h"
#include "ofxScreenSetup.h"

#define CONFIGS_DIRECTORY "configs"

class AppManager {
  public:
    static AppManager &get()
    {
        static AppManager sInstance;
        return sInstance;
    }

    AppManager(){};
    ~AppManager(){};

    void setup();
    void setupManagers();
    void update( float dt );
    void draw();

    // --- STATES --- //
    void   setAppState( CFC::AppState appState );
    void   nextAppState();
    string getAppState();


    // --- KEY EVENTS --- //
    void onKeyPressed( ofKeyEventArgs &arg );

  private:
    // --- STATES --- //
    CFC::AppState mAppState;

    // --- LISTENERS --- //
    void onContentStateFinished( ofxNotificationCenter::Notification &n );

    //! animatable object
    ofxAnimatableFloat mAnim;
    void               onAnimFinished( ofxAnimatable::AnimationEvent &args );
    void               startInternalTimer( float dur );

    // logs
    string log_path = "logs";

    // screen
    ofxScreenSetup ss;
    void           setupChanged( ofxScreenSetup::ScreenSetupArg &arg );

        // TextureAtlas ////////////////////////////////////////////
    void onAtlasCreationFinished( bool &arg );
    void onAtlasesLoaded( bool & );
    void setUpTextures();
    void createAtlas();
    void drawAtlas();
    bool loadAtlas();K
};

/*
switch (mAppState)
{
    case AppState::IDLE: { break; }
    case AppState::WIPE_START: { break; }
    case AppState::LOADING_HAIKU: { break; }
    case AppState::ANIMATE_HAIKU: { break; }
    default: break;
}
*/