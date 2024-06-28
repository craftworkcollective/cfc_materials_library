//
//  ScreenObject.cpp
//
//

#include "ScreenObject.h"

ScreenObject::ScreenObject()
{
}

ScreenObject::~ScreenObject()
{
    /*
    ofRemoveListener( eventTouchDown, this, &ScreenObject::onTouchDown );
    ofRemoveListener( eventTouchUp, this, &ScreenObject::onTouchUp );
    ofRemoveListener( eventClick, this, &ScreenObject::onClick );
    ofRemoveListener( animVal.animFinished, this, &ScreenObject::onAnimValFinished );
    */
}


void ScreenObject::setup( CFCObject *cfcObject )
{
    mCfcObject = cfcObject;
    mPos.x = ofRandom( ofGetWidth() );
    mPos.y = ofRandom( ofGetHeight() );
}

void ScreenObject::setPosition( ofVec2f pos )
{
    mPos = pos;
}

void ScreenObject::setStartPosition( ofVec2f pos )
{
    startPos = pos;
}


 void ScreenObject::setMaxSize( ofVec2f size )
{
    maxSize = size;
    mTargetSize = size; 
};

void ScreenObject::setupTexture()
{

    textureFile = "images\\" + mCfcObject->imagePath;

    float ar = td.height * td.width;
    float scaledWidth = size.y / ( ar );
    float percWidth = ( scaledWidth - size.x ) / scaledWidth;

    calcCrop( 1.0f );
}

void ScreenObject::calcCrop( float widthPerc )
{

    TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( textureFile );
    float                                 realWidth = maxSize.y * td.aspectRatio;
    // bc screenobjects have a capped width, we need to already crop; this is the max width % we can show for that photo
    float cropWidthPct = ofClamp( maxSize.x / realWidth, 0, 1 );

    float wPct
        = ofMap( widthPerc, 0, 1, 0, 0.5 * cropWidthPct, true ); // and here we remap that max width % we can show to what "widthPerc" is
    float newW = realWidth * wPct;                               // pixels

    // drawPosition - pixels
    targetTexQuad.verts.tl = ofVec2f( -newW, -maxSize.y * 0.5 ); // keep in mind those dont include pos - only size
    targetTexQuad.verts.tr = ofVec2f( +newW, -maxSize.y * 0.5 ); // pos is taken in account only at draw time
    targetTexQuad.verts.br = ofVec2f( +newW, +maxSize.y * 0.5 );
    targetTexQuad.verts.bl = ofVec2f( -newW, +maxSize.y * 0.5 );

    // update the target obj size
    mTargetSize.x = texQuad.verts.tr.x - texQuad.verts.tl.x;
    mTargetSize.y = texQuad.verts.br.y - texQuad.verts.bl.y;

    // texture crop - normalized coords
    targetTexQuad.texCoords.tl = ofVec2f( 0.5f - wPct, 0 );
    targetTexQuad.texCoords.tr = ofVec2f( 0.5f + wPct, 0 );
    targetTexQuad.texCoords.br = ofVec2f( 0.5f + wPct, 1 );
    targetTexQuad.texCoords.bl = ofVec2f( 0.5f - wPct, 1 );
}


void ScreenObject::update( float dt )
{
    updateDrift();
    mPos = startPos +  drift;
    setPosition(mPos); 
    //updateVisibility();
}

void ScreenObject::updateDrift()
{
    mDrifting = true;

    drift.y = 0;
    drift.x += 1.0f;

    //configs().getAppSize().x + canvasBuffer / 2
    float renderX = configs().getAppSize().x + configs().getCanvasBuffer() ;
    float columnWidth = renderX / configs().getNumCols();
    float xOrigin =  - configs().getCanvasBuffer() / 2;
    float xPosMax = configs().getAppSize().x + configs().getCanvasBuffer()/2;

    if( mPos.x > xPosMax ) {
        // NEED TO DO: RESET
        startPos = ofVec2f( xOrigin, startPos.y );
        drift = ofVec2f( 0, 0 );
    }

   
}

void ScreenObject::draw()
{
}


void ScreenObject::drawInBatch()
{
    if( mOnScreen ) {
        texQuad = targetTexQuad;
        TextureAtlasDrawer::TexQuad q = texQuad;
        q.verts.tl += mPos;
        q.verts.tr += mPos;
        q.verts.br += mPos;
        q.verts.bl += mPos;

        ofSetColor( 255 );
        AtlasManager::get().atlasManager.drawTextureInBatch( textureFile, q, ofColor( mColor, mAlpha ) );
        ofSetColor( 255 );
    }
}


void ScreenObject::drawDebug()
{
}

TextureAtlasDrawer::TexQuad ScreenObject::getParalelogramForRect( const ofRectangle &r )
{

    float slant = r.height;
    float ar = r.width / r.height;

    TextureAtlasDrawer::TexQuad quad;
    quad.verts.tl = ofVec3f( r.x, r.y );
    quad.verts.tr = ofVec3f( r.x + r.width - slant, r.y );
    quad.verts.br = ofVec3f( r.x + r.width - 2 * slant, r.y + r.height );
    quad.verts.bl = ofVec3f( r.x - slant, r.y + r.height );

    quad.texCoords.tl = ofVec2f( ( slant ) / ( r.width ), 0 );
    quad.texCoords.tr = ofVec2f( 1, 0 );
    quad.texCoords.br = ofVec2f( ( r.width - ( slant ) ) / ( r.width ), 1 );
    quad.texCoords.bl = ofVec2f( 0, 1 );

    return quad;
}