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

    // setup drawer objects
    for( int i = 0; i < 4; i++ ) {
        objects.push_back( new DrawerObject() );
        objects[i]->setup( ofVec2f( 650, 650 ), ofVec2f( 650 * i + i * 91 + 100.0f, 554.0f ) );
        addChild( objects[i] );
    }

    setState( CFC::DrawerState::NOT_ACTIVE );
}

void DrawerWindow::update( float dt )
{
    alpha.update( dt );
    closeBtn->setAlpha( 255.0f * alpha.getCurrentValue() );
    //  drawerLabelWidth = FontManager::one().getDrawerLabelWidth( mDrawer );

    for( auto &obj : objects ) {
        obj->setAlpha( alpha.getCurrentValue() * 255 );
    }
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

        ofSetColor( 255, alphaVal );
        FontManager::one().drawDrawerCategory( mCategory + " Materials in " + mDrawer );
        // FontManager::one().drawDrawerLabel( mDrawer, drawerLabelWidth );


        TS_START( "DrawAtlas Drawer" );
        AtlasManager::get().atlasManager.beginBatchDraw();
        for( int i = 0; i < objects.size(); i++ ) {
            objects[i]->drawInBatch( alphaVal );
        }
        AtlasManager::get().atlasManager.endBatchDraw( false );
        TS_STOP( "DrawAtlas Drawer" );


        break;
    }
    default:
        break;
    }
}

void DrawerWindow::setCategory( string txt )
{
    if( !txt.empty() && txt.back() == ' ' ) {
        txt.pop_back();
    }
    mCategory = txt;
};

void DrawerWindow::setState( CFC::DrawerState state )
{
    mState = state;

    switch( mState ) {
    case CFC::DrawerState::NOT_ACTIVE:
        closeBtn->setSize( 0.0f, 0.0f );
        setSize( 0.0f, 0.0f );

        for( auto &obj : objects ) {
            obj->setSize( 0.0f, 0.0f );
        }
        break;
    case CFC::DrawerState::FADE_IN:
        setSize( mSize.x, mSize.y );

        for( auto &obj : objects ) {
            obj->setSize( 650, 650 );
        }
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


void DrawerWindow::passData( CFC::DrawerData data )
{

    mDrawer = data.drawerLabel;
    mCategory = data.categoryString;

    for( int i = 0; i < data.imgPaths.size(); i++ ) {
        if( i < objects.size() ) {
            objects[i]->setTexturePath( data.imgPaths[i] );
            objects[i]->calcCrop( 1.0 );
            objects[i]->setCompany( data.companies[i] );
            objects[i]->setTItle( data.titles[i] );
            objects[i]->setSoIndex( data.uids[i] );
        }
    }

    numActive = data.imgPaths.size();

    /*
    * NEED TO DO
    // set up texture
    float ar = td.height * td.width;
    float scaledWidth = size.y / ( ar );
    float percWidth = ( scaledWidth - size.x ) / scaledWidth;

    calcCrop( 1.0f );
    */

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

void DrawerWindow::onCloseBtnClicked( CFC::ScreenObjectData &data )
{
    setState( CFC::DrawerState::FADE_OUT );
}
