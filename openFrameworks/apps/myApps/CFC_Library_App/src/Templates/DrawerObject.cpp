

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

    color = ofColor( 255, 0, 0 );
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
    // draw the frame
    ofSetColor( color );
    ofDrawRectangle( 0, 0, getWidth(), getHeight() );

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