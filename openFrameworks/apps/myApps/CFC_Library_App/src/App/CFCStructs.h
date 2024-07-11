
//
//  Created by Nicole Yi Messier
//

#pragma once

#include "ofMain.h"
#include "ofxAnimatableFloat.h"

namespace CFC {

enum class AppState { LOADING, ATTRACT, DRAWER, MATERIAL, NUM_STATES };
enum class DrawerState { NOT_ACTIVE, FADE_IN, ACTIVE, FADE_OUT, NUM_STATES };
enum class MaterialTypes { MANUFACTURED, META, COMMON, NOVEL, NUM_STATES };

// --- CONTENT --- //
enum class ContentType { NUM_CONTENT_TYPES };


struct ScreenObjectData {
    int     uid;
    string  title;
    string  categoryString;
    string  description;
    string  texturePath;
    string  drawerLabel;
    string  color;
    string  uses;
    string  materials;
    string  logoFilePath;
    string details; 
    ofVec2f position;
};

struct DrawerData {
    string categoryString;
    string drawerLabel;
    vector<int> uids; 
    vector<string> imgPaths; 
};

} // namespace CFC