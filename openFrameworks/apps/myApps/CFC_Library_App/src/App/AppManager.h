//
//  AppManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once

#include "AppSettings.h"
#include "AtlasManager.h"
#include "CFCObject.h"
#include "CFCStructs.h"
#include "MaterialWindow.h"
#include "FontManager.h"
#include "ScreenObject.h"
#include "ofxNotificationCenter.h"
#include "ofxScreenSetup.h"
#define CONFIGS_DIRECTORY "configs"

class AppManager : public ofxInterface::Node {
  public:
    static AppManager &get()
    {
        static AppManager sInstance;
        return sInstance;
    }

    AppManager(){};
    ~AppManager(){};

    void setup();
    void setupObjects();
    void setupManagers();
    void update( float dt );
    void draw();
    void drawAtlas();

    // --- STATES --- //
    void   setAppState( CFC::AppState appState );
    void   nextAppState();
    string getAppState();
    bool   getDebug() { return bShowDebug; };


    // --- KEY EVENTS --- //
    void onKeyPressed( ofKeyEventArgs &arg );

  private:
    // --- STATES --- //
    CFC::AppState mAppState;
    bool          bShowDebug{ false };

    // screen objects
    vector<CFCObject *>    objects;
    vector<ScreenObject *> screenObjects;
    int                    numScreenObjects;
    void                   layoutScreenObjects();

    // --- LISTENERS --- //
    void onContentStateFinished( ofxNotificationCenter::Notification &n );
    void onAtlasCreationFinished( bool &arg );
    void onAtlasesLoaded( bool & );
    void onScreenObjectClicked( CFC::ScreenObjectData &data );

    //! animatable object
    ofxAnimatableFloat mAnim;
    void               onAnimFinished( ofxAnimatable::AnimationEvent &args );
    void               startInternalTimer( float dur );

    // paths
    string log_path = "logs";
    string materialsJson = "settings/Materials.json";

    // screen
    ofxScreenSetup ss;
    void           setupChanged( ofxScreenSetup::ScreenSetupArg &arg );

    // screenshot
    ofImage img;

    // grid
    ofVec2f maxSiz;

    // drawer
    MaterialWindow       *materialWindow;
    CFC::ScreenObjectData mData;
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