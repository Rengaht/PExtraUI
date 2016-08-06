#include "ofMain.h"
#include "ofApp.h"

//========================================================================
float FrameAnimation::millis_per_frame=16.6667;
string GlobalParam::ParamFilePath="_param.xml";
string GlobalParam::PosFilePath="_pos.xml";

int main(){
	ofSetupOpenGL(1280,720,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
