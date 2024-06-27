#include "AtlasManager.h"

void AtlasManager::setup()
{
    ofAddListener( atlasCreator.eventAtlasCreationFinished, this, &AtlasManager::onAtlasCreationFinished );
    ofAddListener( atlasCreator.eventAllAtlasesLoaded, this, &AtlasManager::onAtlasesLoaded );
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
        "png",                                              // image format
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

   
    */

    // atlasManager.endBatchDraw( debug );
    // camera.end();
    TS_STOP( "DrawAtlas" );
}

void AtlasManager::testDraw()
{
    TSGL_START( "draw materials" );
    bool  debug = true;
    float scale = 1.0f;

    float s = 256 * scale;
    float slant = s * SLANT;
    float padding = 10 * scale;
    float offsetX = slant + padding;
    float offsetY = padding;
    atlasManager.beginBatchDraw();

    // i want to draw 2500 tiles, my file list only has N, so lets repeat
    int nTimes = ceil( 2500.0f / filesToDraw.size() );


    for( int i = 0; i < nTimes; i++ ) {
        for( string file : filesToDraw ) {
            TextureAtlasDrawer::TextureDimensions td = atlasManager.getTextureDimensions( file );
            ofRectangle                           r = ofRectangle( offsetX, offsetY, s * td.aspectRatio, s );
            TextureAtlasDrawer::TexQuad           tq = getParalelogramForRect( r );

            atlasManager.drawTextureInBatch( file, tq );
            offsetX += s * td.aspectRatio - slant + padding;
            if( offsetX > ofGetWidth() ) {
                offsetX = 0;
                offsetY += s + padding;
            }
        }
    }

    ofSetColor( 255 );
    int numCats = atlasManager.endBatchDraw( debug ); // draws! returns num tiles drawn
    ofDrawBitmapStringHighlight("materials: " + ofToString(numCats) + "\n"
									"slant: " + ofToString(SLANT) +
									"\nMouse scrollWheel to zoom",
									30, 50);
    TSGL_STOP( "draw materials" );

}

float                       p;

TextureAtlasDrawer::TexQuad AtlasManager::getParalelogramForRect( const ofRectangle &r )
{

    float slant = r.height * SLANT;
    float ar = r.width / r.height;

    TextureAtlasDrawer::TexQuad quad;
    quad.verts.tl = ofVec3f( r.x, r.y );
    quad.verts.tr = ofVec3f( r.x + r.width - slant, r.y );
    quad.verts.br = ofVec3f( r.x + r.width - 2 * slant, r.y + r.height );
    quad.verts.bl = ofVec3f( r.x - slant, r.y + r.height );

    quad.texCoords.tl = ofVec2f( ( slant + p ) / ( r.width ), 0 );
    quad.texCoords.tr = ofVec2f( 1, 0 );
    quad.texCoords.br = ofVec2f( ( r.width - ( p + slant ) ) / ( r.width ), 1 );
    quad.texCoords.bl = ofVec2f( 0, 1 );

    return quad;
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