#pragma once

#include "ofMain.h"

struct Settings {
    virtual ~Settings() = default;
    virtual void   parseJson( const ofJson &json ) = 0;
    virtual ofJson toJson() const = 0;
};

class AppSettings final {
  public:
    static AppSettings &one()
    {
        static AppSettings instance;
        return instance;
    }
    ~AppSettings() = default;

    void setup();

    // --- APP SETTINGS --- //
    bool getLogToFile() const { return mLogToFile; }
    bool getTesting() const { return mTesting; }
    bool getMouseOn() const { return mMouseOn; }
    void setMouseOn( bool mouse_on ) { mMouseOn = mouse_on; }
    int  getWindowMode() { return window_mode; }


    // --- WINDOW SETTINGS --- //
    glm::vec2 getAppSize() { return mAppSize; }

    // --- CONTENT --- //
    const string &getImagePath() const { return ImagePath; }
    bool          getCreateAtlases() const { return createAtlases; }

    // --- FLGS --- //
    bool getDebugJson() const { return debug_json; }
    bool getAppDebug() { return appDebug; }
    void toggleAppDebug() { appDebug = !appDebug; }

    int getNumRows() { return numRows; };
    int getNumCols() { return numCols; };
    float getCanvasBuffer() { return canvasBuffer; };
    int   getOutsideGridPad() {return outsideGridPad; };
    int   getGridSpacing() {return gridSpacing; };


  private:
    AppSettings() = default;

    // --- APP SETTINGS --- //
    bool mLogToFile{ false };
    bool mTesting{ false };
    int  window_mode{ 0 };

    // --- WINDOW SETTINGS --- //
    glm::vec2 mAppSize;

    // --- CONTENT --- //
    string ImagePath;
    string mTransitionWipe;
    bool   createAtlases{ false };

    // --- FLAGS --- //
    bool debug_json{ true };
    bool appDebug{ true }; // global debug state for application
    bool mMouseOn{ true };

    // --- CLASS SPECIFIC --- //
    string mAppSettingsFile = "settings/AppSettings.json";

    // internal
    int numRows{ 4 };
    int numCols{ 12 };
    float canvasBuffer{ 2000.0f };
    int   outsideGridPad{ 50 };
    int   gridSpacing{ 50 };
};

static AppSettings &configs()
{
    return AppSettings::one();
}