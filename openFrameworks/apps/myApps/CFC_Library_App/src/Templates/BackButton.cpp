//
//  BackButton.cpp
//  interfaceExample
//
//  Created by Nicole Yi Messier
//
//

#include "BackButton.h"

BackButton::BackButton()
{
}

BackButton::~BackButton()
{
    ofRemoveListener( eventTouchUp, this, &BackButton::onTouchUp );
    ofRemoveListener( eventTouchDown, this, &BackButton::onTouchDown );
    ofRemoveListener( eventClick, this, &BackButton::onClick );
}

void BackButton::setup( ofVec2f size, ofVec2f position )
{
    //set size + position
    setSize(size.x, size.y);
    setPosition(position.x, position.y);
    
    color = ofColor( 255, 255, 255 );
    highlightColor = ofColor(0, 255, 0);
    CB_touched = false;
    
    //set up touch events
    ofAddListener( eventTouchUp, this, &BackButton::onTouchUp );
    ofAddListener( eventTouchDown, this, &BackButton::onTouchDown );
    ofAddListener( eventClick, this, &BackButton::onClick );

    img.load( "ui/close-icon.png" ); 
}

void BackButton::update( float dt )
{
    
}

void BackButton::draw()
{
    //draw the frame
    ofSetColor( ofColor::pink, mAlpha );
    ofDrawRectangle(0, 0, size.x, size.y); 
    
    /*
    //draw the touch anchor
    if(CB_touched){
        ofSetColor( highlightColor, mAlpha );
        ofDrawEllipse(touchAnchor, 10, 10);
    }
    */
}

void BackButton::onTouchDown( ofxInterface::TouchEvent &event )
{
    ofVec2f local = toLocal(event.position);
    CB_touched = true;
    touchAnchor = local;
}

void BackButton::onTouchUp( ofxInterface::TouchEvent &event )
{
    ofVec2f local = toLocal(event.position);
    CB_touched = false;
}

void BackButton::onClick( ofxInterface::TouchEvent &event )
{
    CFC::ScreenObjectData data;
    ofNotifyEvent( eventBackBtn, data, this );
}
    

