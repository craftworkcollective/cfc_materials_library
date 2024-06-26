#include "AppSettings.h"

void AppSettings::setup()
{
	ofFile file;

	if (file.open(ofToDataPath(mAppSettingsFile))) {
		ofJson app_settings;

		try {
			// Load the JSON.
			file >> app_settings;

			// Parse the JSON.
			if (app_settings.contains("configurable")) {
				const auto& configurable = app_settings["configurable"];

				if (configurable.contains("app")) {
					const auto& app = configurable.value("app", ofJson{});
                    window_mode = app["window_mode"];
					mLogToFile = app.value("log_to_file", true);
					mTesting = app.value("testing", false);
					debug_json = app.value("debug_json", false);
					remoteiu_port = app.value("remote_ui", 8888);
					mMouseOn = app.value("mouse_on", true);
					appDebug = app.value("app_debug", false);
					mAppSize.x = app.value("app_width", 1920);
					mAppSize.y = app.value("app_height", 500);
				}
				else
					ofLogWarning("AppSettings") << "No app settings found.";


				if (configurable.contains("content")) {
					const auto& app = configurable.value("content", ofJson{});
					mIdleVideo = app.value("idle_video", "content/idle.mp4");
					mTransitionWipe = app.value("transition_wipe", "content/Transition-Wipe");
				}
				else
					ofLogWarning("AppSettings") << "No content settings found.";

			}
		}
		catch (const exception& exc) {
			ofLogError() << exc.what() << " while parsing " << app_settings.dump(1);
		}
	}
	else {
		ofLogError("AppSettings::setup") << "Unable to read " << mAppSettingsFile << "!";

		// NEED TO DO: Add pause and then kill app
	}
}

