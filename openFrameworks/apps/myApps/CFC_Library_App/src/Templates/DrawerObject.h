

#pragma once

#include "TextureAtlasDrawer.h"
#include "ofMain.h"
#include "ofxInterface.h"
#include "AtlasManager.h"

class DrawerObject : public ofxInterface::Node {
  public:
    DrawerObject();
    ~DrawerObject();

    void setup( ofVec2f rectSize, ofVec2f position );
    void update( float dt );
    void draw();
    void drawInBatch( float alpha );

    // touch
    void onTouchDown( ofxInterface::TouchEvent &event );
    void onTouchUp( ofxInterface::TouchEvent &event );
    void onClick( ofxInterface::TouchEvent &event );

    // ofEvent<umoData> eventUmoClicked;

    // texture
    void calcCrop( float widthPerc );
    void setTexturePath( string path ) { textureFile = path; };

  private:
    ofVec2f touchAnchor;
    bool    UO_touched;

    // attributes
    ofColor color;
    ofColor highlightColor;
    ofVec2f size;
    ofVec2f startPosition;
    string  objectId;

    // texture
    ofVec2f                               imgSize{ ofVec2f( 650, 650 ) }; // image
    string                                textureFile;
    TextureAtlasDrawer::TexQuad           texQuad;
    TextureAtlasDrawer::TexQuad           targetTexQuad;
    TextureAtlasDrawer::TextureDimensions td;
    bool                                  drifting = false;
};