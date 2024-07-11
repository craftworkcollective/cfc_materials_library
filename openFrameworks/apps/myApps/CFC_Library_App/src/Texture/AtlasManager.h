//
//  AtlasManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once
#include "TextureAtlasCreator.h"
#include "TextureAtlasDrawer.h"
#include "ofxTimeMeasurements.h"

#define SLANT ( ofMap( ofGetMouseX() / float( ofGetWidth() ), 0, 1, 0, 1, true ) )

class AtlasManager {
  public:
    static AtlasManager &get()
    {
        static AtlasManager sInstance;
        return sInstance;
    }

    AtlasManager(){};
    ~AtlasManager(){};

    void setup();
    void drawDebug();
    void testDraw(); 

    void setUpTextures();
    void createAtlas(vector<string>imgList);
    bool loadAtlas();

    // atlas drawer

    TextureAtlasCreator         atlasCreator;
    TextureAtlasDrawer atlasManager;
    vector<string>     filesToDraw;
    TextureAtlasDrawer::TexQuad getParalelogramForRect( const ofRectangle &r );

  private:
    // TEXTURE ATLAS /////////////////////////////////////////////////
    // atlas creation/loader
    float               altasGrayscaleness = 0.5; // to mute the colors of the atlas so tile colors stand out more
    int                 atlasSize = 8192;         // fbo size for atlas
    GLint               internalFormat = GL_RGB;
    ofVec2f             maxItemSideSize = ofVec2f( 1200, 1200 );
    float               padding = 8.0f;
    bool                generateMipMaps = true;
    float               atlasMipmapBias = -0.3;
    bool                createAtlasRegardless = false; // if true, always create atlas regardles of content freshess

};
