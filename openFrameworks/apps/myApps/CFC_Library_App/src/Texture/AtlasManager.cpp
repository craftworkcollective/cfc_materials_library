#include "AtlasManager.h"

void AtlasManager::setup()
{
    ofAddListener( atlasCreator.eventAtlasCreationFinished, this, &AtlasManager::onAtlasCreationFinished );
    createAtlas();
}

//--------------------------------------------------------------
void AtlasManager::createAtlas()
{
    vector<string> imgList;

    /*
    for( auto u : umassObjects ) {
        string id = u->imagePath;
        imgList.push_back( id );
    }
    */

    // images
    string      path = "images";
    ofDirectory dir( path );
    dir.allowExt( "jpg" );
    dir.listDir();

    // go through and print out all the paths
    float total_width = 0.0f;
    for( int i = 0; i < dir.size(); i++ ) {
        imgList.push_back( dir.getPath( i ) );
    }


    // create as many atlas as required to fit all the images in an atlas of 4096X4096
    ofLogNotice( "AtlasManager" ) << "Create atlas for " << imgList.size() << " images";
    atlasCreator.setNumImagesPerUpdate( 20 );
    atlasCreator.createAtlases( imgList, atlasSize, internalFormat, maxItemSideSize, padding, generateMipMaps, atlasMipmapBias );
}

//--------------------------------------------------------------
bool AtlasManager::loadAtlas()
{
    atlasCreator.setNumImagesPerUpdate( 1 );
    return atlasCreator.loadAtlasesFromDisk( internalFormat, // internal format
        "textureCache",                                      // dir
        "jpeg",                                              // image format
        true,                                                // gen mipmaps
        atlasMipmapBias                                      // mipmap bias
    );
}


//--------------------------------------------------------------
void AtlasManager::onAtlasCreationFinished( bool &arg )
{
    // after the atlases are created, save them to disk
    atlasCreator.saveToDisk( "textureCache", "png" );
    atlasCreator.registerWithManager( atlasManager );

    // get all the files that were in the atlas
    filesToDraw = atlasCreator.getAllImagePaths();
    ofLogNotice( "AtlasManager" ) << "Atlases have been created! There are " << filesToDraw.size() << " images in the atlases.";
    // ppStartupState = UMASS::FINISHED_LOADING_ATLAS;
    //  setupScene();
    //  setUpTextures();
}

//--------------------------------------------------------------
void AtlasManager::onAtlasesLoaded( bool & )
{

    // get all the files that were in the atlas
    filesToDraw = atlasCreator.getAllImagePaths();
    ofLogNotice( "AtlasManager" ) << "Atlases have loaded! There are " << filesToDraw.size() << " images in the atlases.";
    atlasCreator.registerWithManager( atlasManager );
}

//--------------------------------------------------------------
void AtlasManager::setUpTextures()
{
    // vector<ScreenObject *> screenObjects = eventMan->appMan->objectMan->screenObjects;

    /*
    for( int i = 0; i < screenObjects.size(); i++ ) {
        screenObjects[i]->setupTexture();
    }
    */
}

//--------------------------------------------------------------
void AtlasManager::drawAtlas()
{

    // camera.disableMouseInput();
    // camera.begin();

    // ofTranslate( -ofxApp::get().getRenderSize().x / 2, -ofxApp::get().getRenderSize().y / 2 );
    int numAtlasDrawCalls = 0;

    TS_START( "DrawAtlas" );
    atlasManager.beginBatchDraw();

    /*
    // Draw Screen objects
    for( auto &s : eventMan->appMan->objectMan->screenObjects ) {
        if( s->animationState != ScreenObject::VIDEO_MODE  ) {
            if( s->onScreen ) { // no need to draw using atlas if we are offscreen
                numAtlasDrawCalls++;
                s->drawInBatch();
            }
        }
    }

    // Draw sphere objects
    for( auto &win : eventMan->appMan->windowMan->infoWindows ) {
        if( win->getState() == InfoWindow::INTERACT ) {
            for( auto &so : win->sphere->sphereObjects ) {
                if( !so->umoObject->isFullyLoaded() && !so->umoObject->isReadyToDraw() ) {
                    so->drawInBatch();
                    numAtlasDrawCalls++;
                }
            }
        }
    }
    */

    // atlasManager.endBatchDraw( debug );
    // camera.end();
    TS_STOP( "DrawAtlas" );
}

void AtlasManager::drawDebug()
{

    float scale = 1.0f;

    ofDrawBitmapString( "Progress: " + ofToString( atlasCreator.getPercentDone() * 100, 1 ), 30, 50 );

    if( atlasCreator.isCreating() ) {
        // atlass construction process
        ofTexture *atlas = atlasCreator.getCurrentlyCreatedAtlas();
        atlas->draw( 0, 0, ofGetHeight(), ofGetHeight() );
        ofDrawBitmapStringHighlight(
            "Creating Atlas: " + ofToString( atlasCreator.getPercentDone(), 2 ) + "\n" + atlasCreator.getCurrentCreatingFileName(), 30,
            30 );
    }
    if( atlasCreator.isIdle() ) {
        int x = 0;
        for( int i = 0; i < atlasCreator.getNumAtlases(); i++ ) {
            ofFbo &fbo = atlasCreator.getAtlasAtIndex( i )->getFbo();
            fbo.draw( ofGetMouseX() + x, ofGetMouseY(), fbo.getWidth() * scale, fbo.getHeight() * scale );
            x += fbo.getWidth() * scale;
        }
    }

    if(  atlasCreator.isIdle() ) {
        ofClear( 0 );
        int x = 0;
        for( int i = 0; i < atlasCreator.getNumAtlases(); i++ ) {
            atlasCreator.getAtlasAtIndex( i )->drawDebug( ofGetMouseX() + x, ofGetMouseY() );
            x += atlasCreator.getAtlasAtIndex( i )->getFbo().getWidth();
        }
    }
}