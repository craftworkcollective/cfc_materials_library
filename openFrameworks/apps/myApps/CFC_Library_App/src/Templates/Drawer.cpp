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
    setSize( mSize );

    float paddingX = ( configs().getAppSize().x - mSize.x ) / 2;
    float paddingY = ( configs().getAppSize().y - mSize.y ) / 2;

    setPosition( paddingX, paddingY );
}

void Drawer::draw()
{
    ofSetColor( CFCColors::brandRed );
    ofDrawRectangle( 0.0f, 0.0f, mSize.x, mSize.y );

    ofSetColor( 255 );
    FontManager::one().drawTitle( mTitle );
    FontManager::one().drawBody( mDescription );
}

void Drawer::drawInBatch()
{
    float paddingX = ( configs().getAppSize().x - mSize.x ) / 2;
    float paddingY = ( configs().getAppSize().y - mSize.y ) / 2;

    /*
    TextureAtlasDrawer::TextureDimensions td = AtlasManager::get().atlasManager.getTextureDimensions( file );
    ofRectangle                           r = ofRectangle( offsetX, offsetY, s * td.aspectRatio, s );
    TextureAtlasDrawer::TexQuad           tq = getParalelogramForRect( r );

    texQuad = targetTexQuad;
    TextureAtlasDrawer::TexQuad q = texQuad;
    q.verts.tl += mPos;
    q.verts.tr += mPos;
    q.verts.br += mPos;
    q.verts.bl += mPos;


    ofSetColor( 255 );
    AtlasManager::get().atlasManager.drawTextureInBatch( mMaterialImgPath, q, ofColor( ofColor::white, 255.0f ) );
    ofSetColor( 255 );
    */
}

void Drawer::update( float dt )
{
}

TextureAtlasDrawer::TexQuad Drawer::getParalelogramForRect( const ofRectangle &r )
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
