//
//  CloseButton.cpp
//  interfaceExample
//
//  Created by Nicole Yi Messier
//
//

#include "CloseButton.h"

CloseButton::CloseButton(){
}

CloseButton::~CloseButton(){
    ofRemoveListener(eventTouchUp, this, &CloseButton::onTouchUp);
    ofRemoveListener(eventTouchDown, this, &CloseButton::onTouchDown);
    ofRemoveListener(eventClick, this, &CloseButton::onClick);
}

void CloseButton::setup(ofVec2f size, ofVec2f position){
    //set size + position
    setSize(size.x, size.y);
    setPosition(position.x, position.y);
    
    color = ofColor( 255, 255, 255 );
    highlightColor = ofColor(0, 255, 0);
    CB_touched = false;
    
    //set up touch events
    ofAddListener(eventTouchUp, this, &CloseButton::onTouchUp);
    ofAddListener(eventTouchDown, this, &CloseButton::onTouchDown);
    ofAddListener(eventClick, this, &CloseButton::onClick);

    img.load( "ui/close-icon.png" ); 
}

void CloseButton::update(float dt){
    
}

void CloseButton::draw(){
    //draw the frame
    ofSetColor( color, mAlpha );
    img.draw(0, 0, size.x, size.y); 
    
    //draw the touch anchor
    if(CB_touched){
        ofSetColor( highlightColor, mAlpha );
        ofDrawEllipse(touchAnchor, 10, 10);
    }
}

void CloseButton::onTouchDown(ofxInterface::TouchEvent &event){
    ofVec2f local = toLocal(event.position);
    CB_touched = true;
    touchAnchor = local;
}

void CloseButton::onTouchUp(ofxInterface::TouchEvent &event){
    ofVec2f local = toLocal(event.position);
    CB_touched = false;
}

void CloseButton::onClick(ofxInterface::TouchEvent &event){
    CFC::ScreenObjectData data;
    ofNotifyEvent( eventCloseClicked, data, this );
}
    

