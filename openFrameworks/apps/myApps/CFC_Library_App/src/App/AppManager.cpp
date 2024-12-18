﻿//
//  AppManager.cpp
//
//  Created by Nicole Yi Messier
//

#include "AppManager.h"

using namespace CFC;

void AppManager::setup()
{


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
    ofAddListener( AtlasManager::get().atlasCreator.eventAtlasCreationFinished, this, &AppManager::onAtlasCreationFinished );
    ofAddListener( AtlasManager::get().atlasCreator.eventAllAtlasesLoaded, this, &AppManager::onAtlasesLoaded );
    // ofxNotificationCenter::one().addObserver( this, &AppManager::onContentStateFinished, "FINISHED_CONTENT_ID" );

    // Reset window size
    ofSetWindowShape( configs().getAppSize().x, configs().getAppSize().y );

    TIME_SAMPLE_DISABLE();
    setAppState( AppState::LOADING );
}

void AppManager::setupObjects()
{
    // load am images into vector
    vector<string> imgList;
    string         path = "images";
    ofDirectory    dir( path );
    dir.allowExt( "jpg" );
    dir.allowExt( "png" );
    dir.listDir();

    // go through and print out all the paths
    float total_width = 0.0f;
    for( int i = 0; i < dir.size(); i++ ) {
        imgList.push_back( dir.getPath( i ) );
    }

    ofFile file;

    if( file.open( ofToDataPath( materialsJson ) ) ) {
        ofJson materialSettings;

        try {
            // Load the JSON.
            file >> materialSettings;

            // Parse the JSON.
            if( materialSettings.contains( "materials" ) ) {
                const auto &materials = materialSettings["materials"];

                int index = 0;
                for( int i = 0; i < materials.size(); i++ ) {
                    const auto &material = materials[i];

                    // Create a new unique_ptr and set its fields
                    // string imgPath = "images\\" + material.value( "TopDown", "" );
                    string imgPathGrid = "images\\" + material.value( "Grid", "" );
                    string topImgPath = "images\\" + material.value( "TopDown", "" );
                    if( std::find( imgList.begin(), imgList.end(), imgPathGrid ) != imgList.end()
                        && std::find( imgList.begin(), imgList.end(), topImgPath ) != imgList.end() ) {


                        objects.push_back( new CFCObject() );
                        auto &obj = objects[index];
                        obj->indexUID = index;
                        index++;


                        obj->title = material.value( "Title", "" );
                        obj->categoryString = material.value( "MaterialCategory", "" );
                        obj->description = material.value( "Description", "" );
                        obj->imagePathTop = material.value( "TopDown", "" );
                        obj->imagePathGrid = material.value( "Grid", "" );
                        obj->drawerLabel = ofToUpper( material.value( "DrawerLabel", "" ) );
                        obj->color = material.value( "MaterialColor", "" );
                        obj->uses = material.value( "Uses", "" );
                        obj->logoFilePath = material.value( "Logo", "" );

                        obj->company = material.value( "CompanyName", "" );
                        obj->details = material.value( "CompanyAbout", "" );

                        obj->assignCategory();
                    }
                    else {
                        string title = material.value( "Title", "" );

                        if( std::find( imgList.begin(), imgList.end(), topImgPath ) == imgList.end() )
                            ofLogError() << topImgPath << " does not exists " << title;

                        if( std::find( imgList.begin(), imgList.end(), imgPathGrid ) == imgList.end() )
                            ofLogError() << imgPathGrid << " does not exists " << title;
                    }
                }
            }
        }
        catch( const exception &exc ) {
            ofLogError() << exc.what() << " while parsing " << materialSettings.dump( 1 );
        }
    }
    else {
        ofLogError( "AppSettings::setup" ) << "Unable to read " << materialsJson << "!";
    }

    ofLogNotice() << "Loaded " << objects.size() + 1 << " materials.";

    // shuffle vector
    auto rng = std::default_random_engine{};
    std::shuffle( std::begin( objects ), std::end( objects ), rng );

    // create drawers for each label
    /*
    - [ ]  A1-A12
    - [ ]  B1 - B12
    - [ ]  C1-C6
    - [ ]  D1-D6
    - [ ]  E1-E16
    - [ ]  F1-F16
    */
    for( int i = 1; i < 13; i++ ) {
        string key = "A" + ofToString( i );
        objectsByDrawer[key] = vector<int>{};

        key = "B" + ofToString( i );
        objectsByDrawer[key] = vector<int>{};
    }

    for( int i = 1; i < 7; i++ ) {
        string key = "C" + ofToString( i );
        objectsByDrawer[key] = vector<int>{};

        key = "D" + ofToString( i );
        objectsByDrawer[key] = vector<int>{};
    }

    for( int i = 1; i < 17; i++ ) {
        string key = "E" + ofToString( i );
        objectsByDrawer[key] = vector<int>{};

        key = "F" + ofToString( i );
        objectsByDrawer[key] = vector<int>{};
    }


    // organize screen objects into dictionary
    for( int i = 0; i < objects.size(); i++ ) {
        string key = objects[i]->drawerLabel;
        if( objectsByDrawer.count( key ) > 0 ) {
            objectsByDrawer[key].push_back( i );
        }
        else {
            ofLogNotice() << "Key '" << key << "' does not exist.";
            objectsByDrawer[key] = vector<int>{};
            objectsByDrawer[key].push_back( i );
        }
    }


    layoutScreenObjects();

    // setup atlas
    AtlasManager::get().setup();

    if( AppSettings::one().getCreateAtlases() )
        AtlasManager::get().createAtlas( imgList );
    else
        AtlasManager::get().loadAtlas();
}

void AppManager::layoutScreenObjects()
{
    // grid information
    float canvasWidth = configs().getAppSize().x + configs().getCanvasBuffer();
    float maxColumnWidth_grid = canvasWidth / configs().getNumCols();
    float drawAbleScreenSpace = configs().getAppSize().y - 2 * configs().getOutsideGridPad();
    float height_so = ( drawAbleScreenSpace - configs().getGridSpacing() * ( configs().getNumRows() - 1 ) ) / configs().getNumRows();
    float yOrigin = configs().getOutsideGridPad() + height_so / 2;
    float xOrigin = -configs().getCanvasBuffer() / 2 - maxColumnWidth_grid;

    // set up screen objects
    int index = 0;

    for( int row = 0; row < configs().getNumRows(); ++row ) {
        for( int col = 0; col < configs().getNumCols(); ++col ) {
            screenObjects.push_back( new ScreenObject() );
            auto &obj = screenObjects[index];
            wrappedIndex = index % objects.size();
            obj->setup( objects[wrappedIndex], index );
            obj->setMaxSize( ofVec2f( maxColumnWidth_grid, height_so ) );
            obj->setName( ofToString( index ) );
            addChild( obj );

            ofAddListener( obj->eventSoClicked, this, &AppManager::onScreenObjectClicked );
            index++;
        }
    }


    numScreenObjects = index;
    index = 0;
    for( int row = 0; row < configs().getNumRows(); ++row ) {
        for( int col = 0; col < configs().getNumCols(); ++col ) {
            float x = xOrigin + col * maxColumnWidth_grid;
            float y = yOrigin + row * ( height_so + configs().getGridSpacing() );
            screenObjects[index]->setPosition( ofVec2f( x, y ) );
            screenObjects[index]->setStartPosition( ofVec2f( x, y ) );
            screenObjects[index]->setOnScreen( true );
            index++;
            // screenObjects.push_back( ScreenObject( x, y, maxColumnWidth_grid, height_so ) );
        }
    }
}

void AppManager::setupChanged( ofxScreenSetup::ScreenSetupArg &arg )
{

    ofLogNotice() << "ofxScreenSetup setup changed from " << ss.stringForMode( arg.oldMode ) << " (" << arg.oldWidth << "x" << arg.oldHeight
                  << ") "
                  << " to " << ss.stringForMode( arg.newMode ) << " (" << arg.newWidth << "x" << arg.newHeight << ")";
}

void AppManager::setupManagers()
{
}


void AppManager::update( float dt )
{
    mAnim.update( dt );

    switch( mAppState ) {
    case AppState::LOADING:
        break;
    case AppState::ATTRACT: {
        for( auto &so : screenObjects ) {
            so->update( dt );

            if( so->getReplaceData() && !so->getOnScreen() ) {
                if( wrappedIndex < objects.size() - 2 )
                    wrappedIndex++;
                else
                    wrappedIndex = 0;

                so->setData( objects[wrappedIndex] );
                so->setReplaceData( false );
            }
        }
        break;
    }
    case AppState::DRAWER:
    case AppState::MATERIAL:
    default:
        break;
    }
}


void AppManager::draw()
{
    switch( mAppState ) {
    case AppState::LOADING:
        AtlasManager::get().drawDebug();
        break;
    case AppState::ATTRACT: {
        drawAtlas();
        break;
    }
    case AppState::DRAWER:
        drawAtlas();
        break;
    case AppState::MATERIAL:
        drawAtlas();
        break;
    default:
        break;
    }
}

void AppManager::drawAtlas()
{
    int  numAtlasDrawCalls = 0;
    bool debug = true;

    TS_START( "DrawAtlas" );
    AtlasManager::get().atlasManager.beginBatchDraw();

    // Draw Screen objects
    for( auto &so : screenObjects ) {
        if( so->getOnScreen() ) { // no need to draw using atlas if we are offscreen
            numAtlasDrawCalls++;
            so->drawInBatch();
        }
    }

    AtlasManager::get().atlasManager.endBatchDraw( debug );

    TS_STOP( "DrawAtlas" );
}


#pragma mark STATE MGMT

void AppManager::setAppState( AppState appState )
{
    mPreviousAppState = mAppState;
    mAppState = appState;

    switch( mAppState ) {
    case AppState::LOADING: {
        setupManagers();
        setupObjects();

        materialWindow = new MaterialWindow();
        materialWindow->setup();
        addChild( materialWindow );

        ofAddListener( materialWindow->fadeOutFinished, this, &AppManager::onFadeOutFinished );
        ofAddListener( materialWindow->eventBackBtn, this, &AppManager::onOpenDrawer );

        drawerWindow = new DrawerWindow();
        drawerWindow->setup();

        for( int i = 0; i < drawerWindow->objects.size(); i++ ) {
            ofAddListener( drawerWindow->objects[i]->eventDoClicked, this, &AppManager::onDrawerObjectClicked );
        }

        ofAddListener( drawerWindow->fadeOutFinished, this, &AppManager::onFadeOutFinished );
        addChild( drawerWindow );
        break;
    }
    case AppState::ATTRACT:
        mDrawerData.drawerLabel = "";
        mData.title = "";

        for( auto &obj : screenObjects ) {
            obj->setMDrift( true );
        }
        break;
    case AppState::DRAWER: {
        if( mPreviousAppState == AppState::MATERIAL )
            materialWindow->setState( CFC::DrawerState::FADE_OUT );

        drawerWindow->passData( mDrawerData );
        mData.title = "";

        for( auto &obj : screenObjects ) {
            obj->setMDrift( false );
        }
        break;
    }
    case AppState::MATERIAL: {

        if( mPreviousAppState == AppState::DRAWER )
            drawerWindow->setState( CFC::DrawerState::FADE_OUT );

        materialWindow->passData( mData );
        mDrawerData.drawerLabel = "";

        for( auto &obj : screenObjects ) {
            obj->setMDrift( false );
        }

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
    if( configs().getLiveQrCodeScanner() ) {
        if( arg.key != 'z' && arg.key != 13 ) {
            newQr.push_back( char( arg.key ) );
        }
        else if( arg.key == 'z' ) {
            activeQr = newQr;
            newQr = "";

            onQrCodeScanned( ofToUpper( activeQr ) );
        }
    }

    if( !configs().getLiveQrCodeScanner() ) {

        switch( arg.key ) {
        case 'd':
            bShowDebug = !bShowDebug;
            break;
        case 'h':
            ofHideCursor();
            break;
        case 'm':
            ofShowCursor();
            break;
        case 'n':
            nextAppState();
            break;
        case 's':
            ss.cycleToNextScreenMode();
            break;
        case 'x':
            img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
            img.save( ofGetTimestampString() + "screenshot.png" );
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'q':
            onQrCodeScanned( "A5" );
            break;
        default:
            break;
        }
    }
}

void AppManager::onQrCodeScanned( string txt )
{
    CFC::DrawerData data;
    data.drawerLabel = txt;

    if( objectsByDrawer.count( data.drawerLabel ) > 0 ) {

        for( int i = 0; i < objectsByDrawer[data.drawerLabel].size(); i++ ) {
            int index = objectsByDrawer[data.drawerLabel][i];
            data.categoryString = objects[index]->categoryString;
            data.uids.push_back( index );
            data.imgPaths.push_back( "images\\" + objects[index]->imagePathGrid );
            data.companies.push_back( objects[index]->company );
            data.titles.push_back( objects[index]->title );
        }
    }
    else {
        ofLogNotice() << "Key '" << data.drawerLabel << "' does not exist.";
    }

    mDrawerData = data;
    setAppState( CFC::AppState::DRAWER );
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


//--------------------------------------------------------------
void AppManager::onAtlasCreationFinished( bool &arg )
{
    // after the atlases are created, save them to disk
    AtlasManager::get().atlasCreator.saveToDisk( "textureCache", "png" );
    AtlasManager::get().atlasCreator.registerWithManager( AtlasManager::get().atlasManager );

    // get all the files that were in the atlas
    AtlasManager::get().filesToDraw = AtlasManager::get().atlasCreator.getAllImagePaths();
    ofLogNotice( "AppManager" ) << "Atlases have been created! There are " << AtlasManager::get().filesToDraw.size()
                                << " images in the atlases.";

    //  setupScene();
    //  setUpTextures();
    AtlasManager::get().loadAtlas();
}

//--------------------------------------------------------------
void AppManager::onAtlasesLoaded( bool & )
{

    // get all the files that were in the atlas
    AtlasManager::get().filesToDraw = AtlasManager::get().atlasCreator.getAllImagePaths();
    ofLogNotice( "AppManager" ) << "Atlases have loaded! There are " << AtlasManager::get().filesToDraw.size() << " images in the atlases.";
    AtlasManager::get().atlasCreator.registerWithManager( AtlasManager::get().atlasManager );

    // set up textures in screen objects
    for( auto &so : screenObjects )
        so->setupTexture();

    setAppState( AppState::ATTRACT );
}

void AppManager::onScreenObjectClicked( CFC::ScreenObjectData &data )
{
    ofLogNotice() << "button " << data.title << " clicked";


    if( drawerWindow->getState() == CFC::DrawerState::FADE_OUT || materialWindow->getState() == CFC::DrawerState::FADE_OUT ) {
        return;
    }
    else if( mAppState == CFC::AppState::DRAWER ) {
        drawerWindow->setState( CFC::DrawerState::FADE_OUT );
        setAppState( CFC::AppState::ATTRACT );
    }
    else if( mAppState == CFC::AppState::MATERIAL ) {
        materialWindow->setState( CFC::DrawerState::FADE_OUT );
        setAppState( CFC::AppState::ATTRACT );
    }
    else {
        mData = data;
        setAppState( CFC::AppState::MATERIAL );
    }
}

void AppManager::onDrawerObjectClicked( CFC::ScreenObjectData &data )
{
    ofLogNotice() << "Drawer Object Clicked";
    if( data.index < objects.size() - 1 ) {
        data.title = objects[data.index]->title;
        data.description = objects[data.index]->description;
        data.texturePath = "images\\" + objects[data.index]->imagePathTop;
        data.drawerLabel = objects[data.index]->drawerLabel;
        data.categoryString = objects[data.index]->categoryString;
        data.uses = objects[data.index]->uses;
        data.details = objects[data.index]->details;

        mData = data;
        setAppState( CFC::AppState::MATERIAL );
    }
}


void AppManager::onFadeOutFinished( CFC::ScreenObjectData &data )
{
    if( mAppState == CFC::AppState::MATERIAL && mPreviousAppState == CFC::AppState::ATTRACT ) {
        setAppState( CFC::AppState::ATTRACT );
    }
    else if( mAppState == CFC::AppState::DRAWER && mPreviousAppState == CFC::AppState::ATTRACT ) {
        setAppState( CFC::AppState::ATTRACT );
    }
    else if( mAppState == CFC::AppState::DRAWER && drawerWindow->getState() == CFC::DrawerState::FADE_OUT
        && materialWindow->getState() == CFC::DrawerState::NOT_ACTIVE ) {
        setAppState( CFC::AppState::ATTRACT );
    }
    else if( mAppState == CFC::AppState::MATERIAL && materialWindow->getState() == CFC::DrawerState::FADE_OUT
        && drawerWindow->getState() == CFC::DrawerState::NOT_ACTIVE ) {
        setAppState( CFC::AppState::ATTRACT );
    }
}

void AppManager::onOpenDrawer( CFC::ScreenObjectData &data )
{

    if( mAppState == CFC::AppState::MATERIAL && objectsByDrawer.count( data.drawerLabel ) ) {
        materialWindow->setState( CFC::DrawerState::NOT_ACTIVE );
        onQrCodeScanned( ofToUpper( data.drawerLabel ) );
    }
}