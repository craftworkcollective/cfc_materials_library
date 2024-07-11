

#include "DrawerObject.h"

DrawerObject::DrawerObject()
{
}

DrawerObject::~DrawerObject()
{
    ofRemoveListener( eventTouchDown, this, &DrawerObject::onTouchDown );
    ofRemoveListener( eventTouchUp, this, &DrawerObject::onTouchUp );
    ofRemoveListener( eventClick, this, &DrawerObject::onClick );
}

void DrawerObject::setup( ofVec2f rectSize, ofVec2f position )
{
    // these come from ofxInterface::Node
    setPosition( position );
    setSize( rectSize.x, rectSize.y );

    color = ofColor( 255, 255, 0 );
    highlightColor = ofColor( 0, 0, 255 );
    UO_touched = false;

    // register touch events
    ofAddListener( eventTouchDown, this, &DrawerObject::onTouchDown );
    ofAddListener( eventTouchUp, this, &DrawerObject::onTouchUp );
    ofAddListener( eventClick, this, &DrawerObject::onClick );
}

void DrawerObject::update( float dt )
{
}

void DrawerObject::draw()
{

    // draw the touch anchor
    if( UO_touched ) {
        ofSetColor( highlightColor );
        ofDrawEllipse( touchAnchor, 10, 10 );
    }
}

void DrawerObject::onTouchDown( ofxInterface::TouchEvent &event )
{
    // event.position is always in global space, use toLocal
    ofVec2f local = toLocal( event.position );
    UO_touched = true;
    touchAnchor = local;
}


void DrawerObject::onTouchUp( ofxInterface::TouchEvent &event )
{
    ofVec2f local = toLocal( event.position );
    UO_touched = false;
}

void DrawerObject::onClick( ofxInterface::TouchEvent &event )
{
    /*
    umoData thisData;
    thisData.umoId = objectId;
    ofNotifyEvent( eventUmoClicked, thisData, this );
    */
}

void DrawerObject::calcCrop( float widthPerc )
{

     TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( textureFile );
    float realWidth = imgSize.y * td.aspectRatio;
    // bc screenobjects have a capped width, we need to already crop; this is the max width % we can show for that photo
    float cropWidthPct = ofClamp( imgSize.x / realWidth, 0, 1 );

    float wPct
        = ofMap( widthPerc, 0, 1, 0, 0.5 * cropWidthPct, true ); // and here we remap that max width % we can show to what "widthPerc" is
    float newW = realWidth * wPct;                               // pixels

    // drawPosition - pixels
    targetTexQuad.verts.tl = ofVec2f( 0, 0 );
    targetTexQuad.verts.tr = ofVec2f( +newW * 2, 0 );
    targetTexQuad.verts.br = ofVec2f( +newW * 2, +imgSize.y );
    targetTexQuad.verts.bl = ofVec2f( 0, +imgSize.y );


    // texture crop - normalized coords
    targetTexQuad.texCoords.tl = ofVec2f( 0.5f - wPct, 0 );
    targetTexQuad.texCoords.tr = ofVec2f( 0.5f + wPct, 0 );
    targetTexQuad.texCoords.br = ofVec2f( 0.5f + wPct, 1 );
    targetTexQuad.texCoords.bl = ofVec2f( 0.5f - wPct, 1 );
}

void DrawerObject::drawInBatch( float alpha )
{
    texQuad = targetTexQuad;
    TextureAtlasDrawer::TexQuad q = texQuad;

    //float padding = 91.0f;

    ofVec2f mPos = ofVec2f( getPosition().x, getPosition().y );
    q.verts.tl += mPos;
    q.verts.tr += mPos;
    q.verts.br += mPos;
    q.verts.bl += mPos;


    ofSetColor( 255 );
    AtlasManager::get().atlasManager.drawTextureInBatch( textureFile, q, ofColor( ofColor::white, alpha ) );
    ofSetColor( 255 );
}