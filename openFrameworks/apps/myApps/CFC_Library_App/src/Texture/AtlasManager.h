//
//  AtlasManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once
#include "TextureAtlasCreator.h"
#include "TextureAtlasDrawer.h"
#include "ofxTimeMeasurements.h"

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

    // TextureAtlas ////////////////////////////////////////////
    void onAtlasCreationFinished( bool &arg );
    void onAtlasesLoaded( bool & );
    void setUpTextures();
    void createAtlas();
    void drawAtlas();
    bool loadAtlas();

  private:
    // atlas drawer
    TextureAtlasDrawer atlasManager;
    vector<string>     filesToDraw;

    // TEXTURE ATLAS /////////////////////////////////////////////////
    // atlas creation/loader
    TextureAtlasCreator atlasCreator;
    float               altasGrayscaleness = 0.5; // to mute the colors of the atlas so tile colors stand out more
    int                 atlasSize = 8192;         // fbo size for atlas
    GLint               internalFormat = GL_RGB;
    ofVec2f             maxItemSideSize = ofVec2f( 960, 400 ) * 0.5;
    float               padding = 8.0f;
    bool                generateMipMaps = true;
    float               atlasMipmapBias = -0.3;
    bool                createAtlasRegardless = false; // if true, always create atlas regardles of content freshess
};
