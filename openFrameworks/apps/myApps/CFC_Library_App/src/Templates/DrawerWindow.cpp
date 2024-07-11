#include "DrawerWindow.h"

DrawerWindow::DrawerWindow()
{
}

DrawerWindow::~DrawerWindow()
{
}

void DrawerWindow::setup()
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
    ofAddListener( closeBtn->eventCloseClicked, this, &DrawerWindow::onCloseBtnClicked );

    // setup animations

    alpha.reset( 0.0f );
    alpha.setCurve( SINH );
    alpha.setRepeatType( PLAY_ONCE );
    alpha.setDuration( duration );
    alpha.animateTo( 1.0f );
    ofAddListener( alpha.animFinished, this, &DrawerWindow::onAnimValFinished );

    setState( CFC::DrawerState::NOT_ACTIVE );
}

void DrawerWindow::update( float dt )
{
    alpha.update( dt );
    closeBtn->setAlpha( 255.0f * alpha.getCurrentValue() );
}


void DrawerWindow::draw()
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

        

        break;
    }
    default:
        break;
    }
}

void DrawerWindow::drawInBatch( float alpha )
{
    texQuad = targetTexQuad;
    TextureAtlasDrawer::TexQuad q = texQuad;

    float padding = 100.0f;

    ofVec2f mPos = ofVec2f( padding, padding );
    q.verts.tl += mPos;
    q.verts.tr += mPos;
    q.verts.br += mPos;
    q.verts.bl += mPos;


    ofSetColor( 255 );
    //AtlasManager::get().atlasManager.drawTextureInBatch( mMaterialImgPath, q, ofColor( ofColor::white, alpha ) );
    ofSetColor( 255 );
}

void DrawerWindow::setState( CFC::DrawerState state )
{
    mState = state;

    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        setSize( 0.0f, 0.0f );
        break;
    case CFC::DrawerState::FADE_IN:
        setSize( mSize.x, mSize.y );
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

void DrawerWindow::onAnimValFinished( ofxAnimatable::AnimationEvent &event )
{
    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        break;
    case CFC::DrawerState::FADE_IN:
        setState( CFC::DrawerState::ACTIVE );
        break;
    case CFC::DrawerState::ACTIVE:
        break;
    case CFC::DrawerState::FADE_OUT:
        setState( CFC::DrawerState::NOT_ACTIVE );
        break;
    default:
        break;
    }
}


void DrawerWindow::passData( CFC::ScreenObjectData data )
{   

    mDrawer = data.drawerLabel;
    mCategory = data.categoryString;


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

void DrawerWindow::calcCrop( float widthPerc )
{

   // TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( mMaterialImgPath );
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
}


void DrawerWindow::onCloseBtnClicked( CFC::ScreenObjectData &data )
{
    setState( CFC::DrawerState::FADE_OUT );
}