#include "ofApp.h"
#include "SceneSleep.h"
#include "SceneSign.h"
#include "SceneRecord.h"
#include "ScenePreview.h"
#include "SceneSticker.h"
#include "SceneEnd.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	_SR=float(ofGetWidth())/1920.0;
	_global=new GlobalParam();
	_source=new GlobalSource();

	ofSetFrameRate(60);

	_webcam.setDeviceID(0);
	_webcam.setDesiredFrameRate(60);
	bool webcam_fail=!_webcam.initGrabber(1920,1080);
	if(webcam_fail){
		ofLog()<<"[Device Check] webcam fail!";
	}

	loadScene();
	_source->_mov_back.play();
	
	initCommunication();

	changeScene(SceneMode::SLEEP);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	_source->update();
	_webcam.update();

	_dmillis=ofGetElapsedTimeMillis()-_last_millis;
	_last_millis=ofGetElapsedTimeMillis();

	_scene[_icur_scene]->update(_dmillis);

	if(_osc_receive.hasWaitingMessages()){
		ofxOscMessage message_;
		_osc_receive.getNextMessage(message_);
		ofLog()<<"Get message "<<message_.getAddress();
	}


	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(0);
	_source->_mov_back.draw(0,0,ofGetWidth(),ofGetHeight());
	_scene[_icur_scene]->draw(true);


}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	_scene[_icur_scene]->handleMousePressed(ofGetMouseX(),ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
	case 'q':
		createNewUser();
		uploadSticker(_global->OutputFolder+"st_201685039.png");
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}


//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::loadScene(){
	_scene[0]=new SceneSleep(this,_source);
	_scene[1]=new SceneSign(this,_source);
	_scene[2]=new SceneRecord(this,_source);
	_scene[3]=new ScenePreview(this,_source);
	_scene[4]=new SceneSticker(this,_source);
	_scene[5]=new SceneEnd(this,_source);

}

void ofApp::changeScene(SceneMode mode_){
	ofLog()<<"change scene..."<<mode_;

	if(mode_==SceneMode::STICKER){
		int mfr=_img_rec.size();
		ofImage grab_photo[3];
		grab_photo[0]=_img_rec[0];
		grab_photo[1]=_img_rec[mfr/2];
		grab_photo[2]=_img_rec[mfr-1];

		((SceneSticker*)_scene[mode_])->setStickerImage(((SceneSign*)_scene[SceneMode::SIGN])->getSignImage(),grab_photo);
		_img_rec.clear();
	}
	_icur_scene=mode_;
	_scene[_icur_scene]->prepareInit();

}


void ofApp::createNewUser(){
	_user_id=ofToString(ofGetYear())+ofToString(ofGetMonth())+ofToString(ofGetDay())+ofToString(ofGetHours())+ofToString(ofGetMinutes());
	ofLog()<<"create new user: "<<_user_id;
}



void ofApp::uploadSticker(string path_){
	ofxHttpForm form;
	form.action=_global->ServerURL+"upload/action.php";
	form.method=OFX_HTTP_POST;	
	form.addFormField("action", "upload_sticker");
	form.addFormField("guid", _user_id);
	form.addFile("file",path_);
	_http_util.submitForm(form);

}

void ofApp::newResponse(ofxHttpResponse& response){
	ofLog()<<"http response: "<<response.status<<"  "<<response.responseBody;
}

void ofApp::saveWebcamImage(){
	
	ofImage img_;
	img_.setFromPixels(_webcam.getPixels(),_webcam.getWidth(),_webcam.getHeight(),OF_IMAGE_COLOR);
	_img_rec.push_back(img_);

}

void ofApp::initCommunication(){
	_osc_receive.setup(_global->OscPort);
	_osc_sender.setup(_global->CameraOscIP,_global->CameraOscPort);

	_http_util.setTimeoutSeconds(160);
	_http_util.start();
	ofAddListener(_http_util.newResponseEvent,this,&ofApp::newResponse);

}
void ofApp::sendRecord(){
	ofxOscMessage message_;
	message_.setAddress("/video_start");
	message_.addStringArg(_user_id);
	_osc_sender.sendMessage(message_);
}

void ofApp::sendCompose(){
	ofxOscMessage message_;
	message_.setAddress("/compose_start");
	message_.addStringArg(_user_id);
	_osc_sender.sendMessage(message_);
}


