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


void ScreenObject::setup( ofVec2f maxSize, ofVec2f position )
{
}

void ScreenObject::update( float dt )
{
}

void ScreenObject::draw()
{
}

void ScreenObject::drawDebug()
{
}
