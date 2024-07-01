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

    float paddingX = (configs().getAppSize().x - mSize.x)/2; 
    float paddingY = ( configs().getAppSize().y - mSize.y ) / 2; 

    setPosition(paddingX, paddingY); 

   
}

void Drawer::draw()
{
    ofSetColor( CFCColors::brandRed );
    ofDrawRectangle(0.0f, 0.0f, mSize.x, mSize.y); 

    ofSetColor(255);
    FontManager::one().drawTitle(mTitle); 
    FontManager::one().drawBody( mDescription );
}

void Drawer::drawInBatch()
{

}

void Drawer::update( float dt )
{
}