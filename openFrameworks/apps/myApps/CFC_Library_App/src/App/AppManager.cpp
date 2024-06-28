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
    ofAddListener( AtlasManager::get().atlasCreator.eventAtlasCreationFinished, this, &AppManager::onAtlasCreationFinished );
    ofAddListener( AtlasManager::get().atlasCreator.eventAllAtlasesLoaded, this, &AppManager::onAtlasesLoaded );
    // ofxNotificationCenter::one().addObserver( this, &AppManager::onContentStateFinished, "FINISHED_CONTENT_ID" );

    // Reset window size
    ofSetWindowShape( configs().getAppSize().x, configs().getAppSize().y );

    setAppState( AppState::LOADING );
}

void AppManager::setupObjects()
{
    // load am images into vector
    vector<string> imgList;
    string         path = "images";
    ofDirectory    dir( path );
    dir.allowExt( "jpg" );
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
                    string imgPath = "images\\" + material.value( "TopDown", "" );
                    if( std::find( imgList.begin(), imgList.end(), imgPath ) != imgList.end() ) {


                        objects.push_back( new CFCObject() );
                        auto &obj = objects[index];
                        index++;

                        obj->uid = material.value( "UID", "" );
                        obj->title = material.value( "Title", "" );
                        // obj->category = material.value("window_mode", "");
                        obj->description = material.value( "Description", "" );
                        obj->imagePath = material.value( "TopDown", "" );
                        obj->drawerLabel = material.value( "DrawerLabel", "" );
                        obj->color = material.value( "MaterialColor", "" );
                        obj->uses = material.value( "Uses", "" );
                        obj->unexpectedUses = material.value( "UnexpectedUses", "" );
                        obj->logoFilePath = material.value( "LogoFileName", "" );
                    }
                    else {
                        string title = material.value( "Title", "" );
                        ofLogError() << imgPath << " does not exists " << title;
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

    // set up screen objects
    for( int i = 0; i < numScreenObjects; i++ ) {
        screenObjects.push_back( std::make_unique<ScreenObject>() );
        auto &obj = screenObjects[i];
        int   wrappedIndex = i % objects.size();
        screenObjects[i]->setup( objects[wrappedIndex] );
    }

    // setup atlas
    AtlasManager::get().setup();

    if( AppSettings::one().getCreateAtlases() )
        AtlasManager::get().createAtlas( imgList );
    else
        AtlasManager::get().loadAtlas();

    layoutScreenObjects();
}

void AppManager::layoutScreenObjects()
{
    // grid information
    float maxColumnWidth_grid = configs().getAppSize().x / configs().getNumCols();
    float drawAbleScreenSpace = configs().getAppSize().y - 2 * outsideGridPad;
    float height_so = ( drawAbleScreenSpace - gridSpacing * ( configs().getNumRows() - 1 ) ) / configs().getNumRows();
    float yOrigin = outsideGridPad + height_so / 2;
    float xOrigin = maxColumnWidth_grid / 2;

    // setup size
    for( auto &obj : screenObjects ) {
        obj->setMaxSize( ofVec2f( maxColumnWidth_grid, height_so ) );
    }

    int index = 0;
    for( int row = 0; row < configs().getNumRows(); ++row ) {
        for( int col = 0; col < configs().getNumCols(); ++col ) {
            float x = xOrigin + col * maxColumnWidth_grid;
            float y = yOrigin + row * ( height_so + gridSpacing );
            screenObjects[index]->setPosition( ofVec2f( x, y ) );
            screenObjects[index]->setOnScreen( true );
            index++;
            // screenObjects.push_back( ScreenObject( x, y, maxColumnWidth_grid, height_so ) );
        }
    }

    // turn objects taht are offScreen off
    for( int i = index; i < screenObjects.size(); i++ ) {
        screenObjects[i]->setOnScreen( false );
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
        }
        break;
    }
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
        AtlasManager::get().drawDebug();
        break;
    case AppState::ATTRACT: {
        drawAtlas();
        break;
    }
    case AppState::DRAWER:
        break;
    case AppState::MATERIAL:
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
    mAppState = appState;

    switch( mAppState ) {
    case AppState::LOADING: {
        setupManagers();
        setupObjects();
        break;
    }
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
        break;
    }
    case 'x': {
        img.grabScreen( 0, 0, ofGetWidth(), ofGetHeight() );
        img.save( ofGetTimestampString() + "screenshot.png" );
        break;
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
