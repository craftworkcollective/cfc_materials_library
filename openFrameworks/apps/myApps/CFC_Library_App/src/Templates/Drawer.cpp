#include "Drawer.h"

Drawer::Drawer()
{
}

Drawer::~Drawer()
{
}

void Drawer::setup()
{
    setName( "DRAWER" );

    float paddingX = ( configs().getAppSize().x - mSize.x ) / 2;
    float paddingY = ( configs().getAppSize().y - mSize.y ) / 2;

    setPosition( paddingX, paddingY );
    setSize( mSize );
}

void Drawer::draw()
{
    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        break;
    case CFC::DrawerState::FADE_IN:
    case CFC::DrawerState::ACTIVE:
    case CFC::DrawerState::FADE_OUT: {

        ofSetColor( CFCColors::brandRed );
        ofDrawRectangle( 0.0f, 0.0f, size.x, size.y );

        ofSetColor( 255 );
        FontManager::one().drawTitle( mTitle );
        FontManager::one().drawBody( mDescription );


        TS_START( "DrawAtlas Drawer" );
        AtlasManager::get().atlasManager.beginBatchDraw();
        drawInBatch();
        AtlasManager::get().atlasManager.endBatchDraw( false );
        TS_STOP( "DrawAtlas Drawer" );

        break;
    }
    default:
        break;
    }
}

void Drawer::drawInBatch()
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
    AtlasManager::get().atlasManager.drawTextureInBatch( mMaterialImgPath, q, ofColor( ofColor::white, 255.0f ) );
    ofSetColor( 255 );
}

void Drawer::update( float dt )
{
}

void Drawer::setState( CFC::DrawerState state )
{
    mState = state;

    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        break;
    case CFC::DrawerState::FADE_IN:
        break;
    case CFC::DrawerState::ACTIVE:
        break;
    case CFC::DrawerState::FADE_OUT:
        break;
    default:
        break;
    }
}


void Drawer::passData( CFC::ScreenObjectData data )
{
    mTitle = data.title;
    mDescription = data.description;
    mMaterialImgPath = data.texturePath;

    // set up texture
    float ar = td.height * td.width;
    float scaledWidth = size.y / ( ar );
    float percWidth = ( scaledWidth - size.x ) / scaledWidth;

    calcCrop( 1.0f );

    setState( CFC::DrawerState::FADE_IN );

    /*
    string  title;
    string  categoryString;
    string  description;
    string  imagePath;
    string  drawerLabel;
    string  color;
    string  uses;
    string  unexpectedUses;
    string  logoFilePath;
    ofVec2f position;
    */
}

void Drawer::calcCrop( float widthPerc )
{

    TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( mMaterialImgPath );
    float                                 realWidth = imgSize.y * td.aspectRatio;
    // bc screenobjects have a capped width, we need to already crop; this is the max width % we can show for that photo
    float cropWidthPct = ofClamp( imgSize.x / realWidth, 0, 1 );

    float wPct
        = ofMap( widthPerc, 0, 1, 0, 0.5 * cropWidthPct, true ); // and here we remap that max width % we can show to what "widthPerc" is
    float newW = realWidth * wPct;                               // pixels

    // drawPosition - pixels
    targetTexQuad.verts.tl = ofVec2f( 0, 0); 
    targetTexQuad.verts.tr = ofVec2f( +newW*2, 0 ); 
    targetTexQuad.verts.br = ofVec2f( +newW*2, +imgSize.y );
    targetTexQuad.verts.bl = ofVec2f( 0, +imgSize.y  );


    // texture crop - normalized coords
    targetTexQuad.texCoords.tl = ofVec2f( 0.5f - wPct, 0 );
    targetTexQuad.texCoords.tr = ofVec2f( 0.5f + wPct, 0 );
    targetTexQuad.texCoords.br = ofVec2f( 0.5f + wPct, 1 );
    targetTexQuad.texCoords.bl = ofVec2f( 0.5f - wPct, 1 );
}
