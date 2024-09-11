#include "MaterialWindow.h"

MaterialWindow::MaterialWindow()
{
}

MaterialWindow::~MaterialWindow()
{
}

void MaterialWindow::setup()
{
    setName( "DRAWER" );

    float paddingX = ( configs().getAppSize().x - mSize.x ) / 2;
    float paddingY = ( configs().getAppSize().y - mSize.y ) / 2;

    setPosition( paddingX, paddingY );
    setSize( mSize );

    // set up close btn
    closeBtn = new CloseButton();
    float btnSize = 50.0f;
    closeBtn->setup( ofVec2f( btnSize, btnSize ), ofVec2f( mSize.x - 2 * btnSize, btnSize ) );
    addChild( closeBtn );
    ofAddListener( closeBtn->eventCloseClicked, this, &MaterialWindow::onCloseBtnClicked );


    // set up back btn
    backBtn = new BackButton();
    btnSize = 200.0f;
    backBtn->setup( ofVec2f( btnSize, btnSize ), ofVec2f( 100.0f, 100.0f ) );
    addChild( backBtn );
    ofAddListener( backBtn->eventBackBtn, this, &MaterialWindow::onBackBtn );

    // setup animations
    alpha.reset( 0.0f );
    alpha.setCurve( SINH );
    alpha.setRepeatType( PLAY_ONCE );
    alpha.setDuration( duration );
    alpha.animateTo( 1.0f );
    ofAddListener( alpha.animFinished, this, &MaterialWindow::onAnimValFinished );

    setState( CFC::DrawerState::NOT_ACTIVE );
}
void MaterialWindow::setCategory( string txt )
{
    if( !txt.empty() && txt.back() == ' ' ) {
        txt.pop_back();
    }

    mCategory = txt;


    if( ofToUpper( mCategory ) == ofToUpper( "Common" ) ) {
        mType = CFC::MaterialTypes::COMMON;
    }
    else if( ofToUpper( mCategory ) == ofToUpper( "Novel Sustainable" ) || ofToUpper( mCategory ) == ofToUpper( "Novel" ) ) {
        mType = CFC::MaterialTypes::NOVEL;
    }
    else if( ofToUpper( mCategory ) == ofToUpper( "Manufactured" ) ) {
        mType = CFC::MaterialTypes::MANUFACTURED;
    }
    else if( ofToUpper( mCategory ) == ofToUpper( "Meta" ) ) {
        mType = CFC::MaterialTypes::META;
    }
    else {
        ofLogError() << txt << " is not a type! Defaulting to common";
        mType = CFC::MaterialTypes::COMMON;
    }
}

void MaterialWindow::update( float dt )
{
    alpha.update( dt );
    closeBtn->setAlpha( 255.0f * alpha.getCurrentValue() );
}


void MaterialWindow::draw()
{
    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        break;
    case CFC::DrawerState::FADE_IN:
    case CFC::DrawerState::ACTIVE:
    case CFC::DrawerState::FADE_OUT: {

        float alphaVal = 255.0f * alpha.getCurrentValue();
        ofSetColor( CFCColors::brandRed, alphaVal );
        ofDrawRectangle( 0.0f, 0.0f, size.x, size.y );

        ofSetColor( 255, alphaVal );
        FontManager::one().drawTitle( mTitle );
        //( string description, string composite, string uses, string details)
        // FontManager::one().drawBody( mDescription, mCompositeMaterials, mPrimaryUses, mDetails );
        FontManager::one().drawBody( mDescription, "", mPrimaryUses, mDetails );

        TS_START( "DrawAtlas Drawer" );
        AtlasManager::get().atlasManager.beginBatchDraw();
        drawInBatch( alphaVal );
        AtlasManager::get().atlasManager.endBatchDraw( false );
        TS_STOP( "DrawAtlas Drawer" );

        ofSetColor( 0, alphaVal );
        FontManager::one().drawDrawer( mDrawer );
        FontManager::one().drawMaterialCategory( mCategory );

        ofSetColor( configs().getMaterialColor( mType ), alphaVal );
        float radius = 50.0f;
        ofDrawCircle( imgSize.x - 10, imgSize.y + 10, radius );

        break;
    }
    default:
        break;
    }
}

void MaterialWindow::drawInBatch( float alpha )
{
    texQuad = targetTexQuad;
    TextureAtlasDrawer::TexQuad q = texQuad;

    float padding = 100.0f;

    ofVec2f mPos = ofVec2f( padding, padding );
    q.verts.tl += mPos;
    q.verts.tr += mPos;
    q.verts.br += mPos;
    q.verts.bl += mPos;

    texQuadLogo = targetTexQuadLogo;
    TextureAtlasDrawer::TexQuad qLogo = texQuadLogo;

    ofVec2f logoPos = ofVec2f( mSize.x - logoSize.x - padding, mSize.y - logoSize.y - padding);
    qLogo.verts.tl += logoPos;
    qLogo.verts.tr += logoPos;
    qLogo.verts.br += logoPos;
    qLogo.verts.bl += logoPos;


    ofSetColor( 255 );
    //AtlasManager::get().atlasManager.drawTextureInBatch( mMaterialImgPath, q, ofColor( ofColor::white, alpha ) );
    if( mLogoImgPath.size())
     AtlasManager::get().atlasManager.drawTextureInBatch( mLogoImgPath, qLogo, ofColor( ofColor::white, alpha ) );
    ofSetColor( 255 );
}

void MaterialWindow::setState( CFC::DrawerState state )
{
    mState = state;

    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        setSize( 0.0f, 0.0f );
        closeBtn->setSize( 0.0f, 0.0f );
        break;
    case CFC::DrawerState::FADE_IN:
        setSize( mSize.x, mSize.y );
        closeBtn->setSize( 50.0f, 50.0f );
        alpha.animateFromTo( 0, 1 );
        break;
    case CFC::DrawerState::ACTIVE:
        break;
    case CFC::DrawerState::FADE_OUT:
        alpha.animateFromTo( 1, 0 );
        break;
    default:
        break;
    }
}

void MaterialWindow::onAnimValFinished( ofxAnimatable::AnimationEvent &event )
{
    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        break;
    case CFC::DrawerState::FADE_IN:
        setState( CFC::DrawerState::ACTIVE );
        break;
    case CFC::DrawerState::ACTIVE:
        break;
    case CFC::DrawerState::FADE_OUT: {

        CFC::ScreenObjectData tempData;
        ofNotifyEvent( fadeOutFinished, tempData, this );
        setState( CFC::DrawerState::NOT_ACTIVE );
        break;
    }
    default:
        break;
    }
}


void MaterialWindow::passData( CFC::ScreenObjectData data )
{
    mTitle = data.title;
    mDescription = data.description;
    mMaterialImgPath = data.texturePath;
    mLogoImgPath = data.logoFilePath; 
    mDrawer = data.drawerLabel;
    setCategory( data.categoryString );
    mDetails = data.details;
    mPrimaryUses = data.uses;
    mDetails = data.details;

    // set up texture
    float ar = td.height * td.width;
    float scaledWidth = size.y / ( ar );
    float percWidth = ( scaledWidth - size.x ) / scaledWidth;

    calcCrop( 1.0f );

    setState( CFC::DrawerState::FADE_IN );

    /*
    string  categoryString;
    string  color;
    string  uses;
    string  unexpectedUses;
    string  logoFilePath;
    ofVec2f position;
    */
}

void MaterialWindow::calcCrop( float widthPerc )
{

    TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( mMaterialImgPath );
    float                                 realWidth = imgSize.y * td.aspectRatio;
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

    if(mLogoImgPath.size()) {
        TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( mLogoImgPath );
        float                                 realWidth = logoSize.y * td.aspectRatio;
        // bc screenobjects have a capped width, we need to already crop; this is the max width % we can show for that photo
        float cropWidthPct = ofClamp( logoSize.x / realWidth, 0, 1 );

        float wPct = ofMap(
            widthPerc, 0, 1, 0, 0.5 * cropWidthPct, true ); // and here we remap that max width % we can show to what "widthPerc" is
        float newW = realWidth * wPct;                      // pixels

        // drawPosition - pixels
        targetTexQuadLogo.verts.tl = ofVec2f( 0, 0 );
        targetTexQuadLogo.verts.tr = ofVec2f( +newW * 2, 0 );
        targetTexQuadLogo.verts.br = ofVec2f( +newW * 2, +logoSize.y );
        targetTexQuadLogo.verts.bl = ofVec2f( 0, +logoSize.y );


        // texture crop - normalized coords
        targetTexQuadLogo.texCoords.tl = ofVec2f( 0.5f - wPct, 0 );
        targetTexQuadLogo.texCoords.tr = ofVec2f( 0.5f + wPct, 0 );
        targetTexQuadLogo.texCoords.br = ofVec2f( 0.5f + wPct, 1 );
        targetTexQuadLogo.texCoords.bl = ofVec2f( 0.5f - wPct, 1 );
    }

}


void MaterialWindow::onCloseBtnClicked( CFC::ScreenObjectData &data )
{
    setState( CFC::DrawerState::FADE_OUT );
}

void MaterialWindow::onBackBtn( CFC::ScreenObjectData &data )
{
    data.drawerLabel = mDrawer;
    ofNotifyEvent( eventBackBtn, data, this );
}