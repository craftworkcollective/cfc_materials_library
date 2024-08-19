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
    ofRemoveListener( eventTouchDown, this, &ScreenObject::onTouchDown );
    ofRemoveListener( eventTouchUp, this, &ScreenObject::onTouchUp );
    ofRemoveListener( eventClick, this, &ScreenObject::onClick );
    // ofRemoveListener( animVal.animFinished, this, &ScreenObject::onAnimValFinished );
}


void ScreenObject::setup( CFCObject *cfcObject, int uid )
{
    mCfcObject = cfcObject;
    mPos.x = ofRandom( ofGetWidth() );
    mPos.y = ofRandom( ofGetHeight() );
    uid = mUid;

    mColor = configs().getMaterialColor( mCfcObject->materialType );


    // register touch events
    ofAddListener( eventTouchDown, this, &ScreenObject::onTouchDown );
    ofAddListener( eventTouchUp, this, &ScreenObject::onTouchUp );
    ofAddListener( eventClick, this, &ScreenObject::onClick );

    mDrifting = true;
}


void ScreenObject::setStartPosition( ofVec2f pos )
{
    startPos = pos;
}


void ScreenObject::setMaxSize( ofVec2f size )
{
    maxSize = size;
    mTargetSize = size;

    setSize( maxSize.x, maxSize.y );
};

void ScreenObject::setupTexture()
{

    textureFile = "images\\" + mCfcObject->imagePathGrid;

    float ar = td.height * td.width;
    float scaledWidth = size.y / ( ar );
    float percWidth = ( scaledWidth - size.x ) / scaledWidth;

    calcCrop( 1.0f );
}

void ScreenObject::setData( CFCObject *cfcObject )
{
    mCfcObject = cfcObject;
    setupTexture();
};

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
    if( mDrifting ) {
        updateDrift();
        mPos = startPos + drift;
        setPosition( mPos - maxSize / 2 );
    }
    // updateVisibility();
}

void ScreenObject::updateDrift()
{


    drift.y = 0;
    drift.x += 1.0f;

    // configs().getAppSize().x + canvasBuffer / 2
    float renderX = configs().getAppSize().x + configs().getCanvasBuffer();
    float columnWidth = renderX / configs().getNumCols();
    float xOrigin = -configs().getCanvasBuffer() / 2;
    float xPosMax = configs().getAppSize().x + configs().getCanvasBuffer() / 2;

    if( mPos.x > xPosMax ) {
        startPos = ofVec2f( xOrigin, startPos.y );
        drift = ofVec2f( 0, 0 );
    }

    if( mPos.x < -size.x )
        mOnScreen = false;
    else
        mOnScreen = true;

    if( mPos.x > ofGetWidth() )
        mReplaceData = true;
}


void ScreenObject::drawDebug()
{
    Node::drawDebug();

    ofSetColor( ofColor::yellow );
    ofDrawBitmapString( mCfcObject->title, 0, -10 );
}


void ScreenObject::draw()
{
    /*
    if( mTouched ) {
        ofSetColor( 100, 100, 100, 100 );
        ofFill();
        ofDrawRectangle( 0, 0, maxSize.x, maxSize.y );
        ofNoFill();
    }
    */
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
        AtlasManager::get().atlasManager.drawTextureInBatch( textureFile, q, ofColor( ofColor::white, mAlpha ) );
        // AtlasManager::get().atlasManager.drawTextureInBatch( textureFile, q, ofColor( mColor, mAlpha * .3 ) );
        // AtlasManager::get().atlasManager.drawTextureInBatch( textureFile, q, ofColor( ofColor::white, mAlpha ) );
        ofSetColor( 255 );
    }
}


void ScreenObject::onTouchDown( ofxInterface::TouchEvent &event )
{
    // event.position is always in global space, use toLocal
    ofVec2f local = toLocal( event.position );
    mTouched = true;
    mTouchAnchor = local;
}


void ScreenObject::onTouchUp( ofxInterface::TouchEvent &event )
{
    ofVec2f local = toLocal( event.position );
    mTouched = false;
}

void ScreenObject::onClick( ofxInterface::TouchEvent &event )
{

    CFC::ScreenObjectData data;
    data.title = mCfcObject->title;
    data.index = mCfcObject->indexUID;
    data.description = mCfcObject->description;
    data.texturePath = "images\\" + mCfcObject->imagePathTop;
    data.drawerLabel = mCfcObject->drawerLabel;
    data.categoryString = mCfcObject->categoryString;
    data.uses = mCfcObject->uses;
    data.details = mCfcObject->details;

    ofNotifyEvent( eventSoClicked, data, this );
}