#pragma once
#include "InsightStructs.h"
#include "ofMain.h"

struct Settings {
	virtual ~Settings() = default;
	virtual void   parseJson(const ofJson& json) = 0;
	virtual ofJson toJson() const = 0;
};

class AppSettings final {
public:
	static AppSettings& one()
	{
		static AppSettings instance;
		return instance;
	}
	~AppSettings() = default;

	void setup();

	// --- APP SETTINGS --- //
	bool getLogToFile() const { return mLogToFile; }
	bool getTesting() const { return mTesting; }
	int  getRemoteUiPort() const { return remoteiu_port; }
	bool getMouseOn() const { return mMouseOn; }
	void setMouseOn(bool mouse_on) { mMouseOn = mouse_on; }
    int  getWindowMode(){ return window_mode; }

	// --- CONTENT --- // 
	string getIdleVideo() const { return mIdleVideo; }
	string getTransitionWipe() const { return mTransitionWipe; }

	// --- WINDOW SETTINGS --- //
	glm::vec2 getAppSize() { return mAppSize; }

	// --- PATHS FOR CONTENT --- //
	const string& getImagePath() const { return ImagePath; }

	// --- FLGS --- //
	bool getDebugJson() const { return debug_json; }
	bool getAppDebug() { return appDebug; }
	void toggleAppDebug() { appDebug = !appDebug; }

private:
	AppSettings() = default;

	// --- APP SETTINGS --- //
	bool mLogToFile{ false };
	bool mTesting{ false };
	int  remoteiu_port{};
    int  window_mode {0}; 

	// --- WINDOW SETTINGS --- //
	glm::vec2 mAppSize; 

	// --- CONTENT --- //
	string ImagePath;
	string mIdleVideo;
	string mTransitionWipe; 

	// --- FLAGS --- //
	bool debug_json{ true };
	bool appDebug{ true };      // global debug state for application
	bool mMouseOn{ true };

	// --- CLASS SPECIFIC --- //
	string mAppSettingsFile = "settings/AppSettings.json";
};

static AppSettings& configs()
{
	return AppSettings::one();
}