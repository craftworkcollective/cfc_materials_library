//
//  AtlasManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once
#include "AppSettings.h"
#include "AtlasManager.h"
#include "BackButton.h"
#include "CFCColors.h"
#include "CloseButton.h"
#include "FontManager.h"
#include "ofxAnimatableFloat.h"
#include "ofxInterface.h"
#include "ofxTimeMeasurements.h"

class MaterialWindow : public ofxInterface::Node {
  public:
    MaterialWindow();
    ~MaterialWindow();


    void setup();
    void draw();
    void drawInBatch( float alpha );
    void update( float dt );
    void passData( CFC::ScreenObjectData data );


    // setters
    void setTitle( string txt ) { mTitle = txt; };
    void setDescription( string txt ) { mDescription = txt; };
    void setCompositeMaterials( string txt ) { mCompositeMaterials = txt; };
    void setPrimaryUses( string txt ) { mPrimaryUses = txt; };
    void setCategory( string txt );
    void setDrawer( string txt ) { mDrawer = txt; };
    void setLogoPath( string path ) { mLogoImgPath = path; };
    void setMaterialImgPath( string path ) { mMaterialImgPath = path; }
    void setState( CFC::DrawerState state );

    // texture
    void             calcCrop( float widthPerc );
    CFC::DrawerState getState() { return mState; };

    // events
    ofEvent<CFC::ScreenObjectData> fadeOutFinished;
    ofEvent<CFC::ScreenObjectData> eventBackBtn;

  private:
    string mTitle{ "Mock Rib Pocket Stretch Sensor" };
    string mDescription{
        "A knitted stretch sensor can be connected to an output such as a light to decrease or increase the brightness. The more a "
        "resistive material is stretched, the more its resistance will increase because it has more surface area to cover. This sensor was "
        "made with conductive and non-conductive yarns using a Brother KH-910 knitting machine."
    };
    string             mCompositeMaterials{ "Conductive Yarn, Non-Conductive Yarn" };
    string             mPrimaryUses{ "" };
    string             mCategory{ "" };
    string             mDrawer{ "" };
    string             mMaterialImgPath{ "" };
    string             mLogoImgPath{ "" };
    string             mDetails{ "" };
    CFC::MaterialTypes mType;

    ofVec2f imgSize{ ofVec2f( 1188, 1188 ) }; // image
    ofVec2f mSize{ ofVec2f( 3072.0f, 1388.0f ) };
    float   padding{ 100.0f };

    // UI
    CloseButton *closeBtn;
    void         onCloseBtnClicked( CFC::ScreenObjectData &data );


    BackButton *backBtn;
    void        onBackBtn( CFC::ScreenObjectData &data );

    // state
    CFC::DrawerState mState{ CFC::DrawerState::NOT_ACTIVE };


    // texture
    TextureAtlasDrawer::TexQuad           texQuad;
    TextureAtlasDrawer::TexQuad           targetTexQuad;
    TextureAtlasDrawer::TextureDimensions td;
    bool                                  drifting = false;

    TextureAtlasDrawer::TexQuad           texQuadLogo;
    TextureAtlasDrawer::TexQuad           targetTexQuadLogo;
    TextureAtlasDrawer::TextureDimensions tdLogo;
    ofVec2f                               logoSize{ ofVec2f( 300, 300 ) }; // image

    // animations
    ofxAnimatableFloat alpha;
    float              duration = 0.5;
    void               onAnimValFinished( ofxAnimatable::AnimationEvent &event );
};