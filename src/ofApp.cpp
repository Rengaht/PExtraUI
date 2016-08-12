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

	_ADJUST_MODE=false;

#ifndef _DEBUG
	ofSetFullscreen(true);
	ofHideCursor();
#endif


	vector<ofVideoDevice> devices = _webcam.listDevices();
	
	bool webcam_fail=false;
	if(devices[0].deviceName.find("Logitech")!=-1){
		_webcam.setDeviceID(0);
		_webcam.setDesiredFrameRate(60);
		webcam_fail=!_webcam.initGrabber(1920,1080);
	}
	if(webcam_fail){
		ofLog()<<"[Device Check] webcam fail!";
	}
	
	_tmp_fbo.allocate(_webcam.getWidth(),_webcam.getHeight(),GL_RGB);
	int mfr=60;
	_img_rec=vector<ofImage>(mfr);
	for(int i=0;i<mfr;++i) _img_rec[i].allocate(_webcam.getWidth(),_webcam.getHeight(),OF_IMAGE_COLOR);

	_frame_to_grab[0]=1;
	_frame_to_grab[1]=mfr/2;
	_frame_to_grab[2]=mfr-2;

	loadScene();
	_source->_mov_back.play();
	
	//_sticker_thread=new CreateSticker(_global->PrintSize,_global->OutputFolder);
	///ofAddListener(_sticker_thread->_ready_to_upload,this,&ofApp::uploadFile);

	initCommunication();


	changeScene(SceneMode::SLEEP);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	
	_webcam.update();
	if(_ADJUST_MODE) return;

	_source->update();
	

	_dmillis=ofClamp(ofGetElapsedTimeMillis()-_last_millis,0,1000.0/60.0);
	_last_millis+=_dmillis;
	
	_scene[_icur_scene]->update(_dmillis);

	if(_osc_receive.hasWaitingMessages()){
		ofxOscMessage message_;
		_osc_receive.getNextMessage(message_);
		ofLog()<<"Get message "<<message_.getAddress();
		if(message_.getAddress()=="/video_finish"){
			if(_icur_scene==SceneMode::SLEEP){
				((SceneSleep*)_scene[_icur_scene])->enableStart(true);
				createQRCodeOrder(message_.getArgAsString(0));


			}else _got_video_finish=true;

		}

	}


	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw(){
	//ofBackground(0);

	if(_ADJUST_MODE){
		_webcam.draw(ofGetWidth(),0,-ofGetWidth(),ofGetHeight());
		((SceneSticker*)_scene[SceneMode::STICKER])->_img_top.draw(ofGetWidth()*.5-ofGetHeight()*.5,0,ofGetHeight(),ofGetHeight());
		return;
	}

	_source->_mov_back.draw(0,0,ofGetWidth(),ofGetHeight());
#ifdef _DEBUG
	_scene[_icur_scene]->draw(true);
#else
	_scene[_icur_scene]->draw(false);
#endif // DEBUG
}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	_scene[_icur_scene]->handleMousePressed(ofGetMouseX(),ofGetMouseY());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 'a':
			sendRecord();
			break;
		case 'c':
			sendCompose();
			break;
		case 'f':
			ofToggleFullscreen();
			_SR=float(ofGetWidth())/1920.0;
			break;
		case 'r':
			_scene[_icur_scene]->reset();
			changeScene(SceneMode::SLEEP);
			break;
		case 'd':
			_ADJUST_MODE=!_ADJUST_MODE;
			break;
		case 'b':
			sendBlowerSignal(true);
			break;
		case 's':
			sendBlowerSignal(false);
			break;
		case 'o':
			if(_icur_scene==SceneMode::SLEEP) ((SceneSleep*)_scene[_icur_scene])->enableStart(true);
			break;
		case 'l':
			if(_icur_scene==SceneMode::SLEEP) ((SceneSleep*)_scene[_icur_scene])->enableStart(false);
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


	switch(mode_){
		case SceneMode::RECORD:
			if(_icur_scene==SceneMode::PREVIEW) _redo_once=true;
			else if(_icur_scene==SceneMode::SIGN) _redo_once=false;
			break;
		case SceneMode::PREVIEW:
			((ScenePreview*)_scene[mode_])->setCanRedo(!_redo_once);
			break;
		case SceneMode::STICKER:
			ofImage grab_photo[3];
			if(_fr_save>0) for(int i=0;i<3;++i) grab_photo[i]=_img_rec[ofClamp(_frame_to_grab[i],0,_fr_save-1)];

			((SceneSticker*)_scene[mode_])->setStickerBackImage(((SceneSign*)_scene[SceneMode::SIGN])->getSignImage(),grab_photo);		
			break;
	}

	_icur_scene=mode_;
	_scene[_icur_scene]->prepareInit();

}


void ofApp::createNewUser(){
	_user_id=ofToString(ofGetYear())+ofToString(ofGetMonth())+ofToString(ofGetDay())+ofToString(ofGetHours())+ofToString(ofGetMinutes());
	ofLog()<<"create new user: "<<_user_id;
}


void ofApp::uploadFile(string& id_){

	_icur_scene=SceneMode::END;
	_scene[_icur_scene]->prepareInit();

	((SceneSleep*)_scene[SceneMode::SLEEP])->enableStart(false);

	string s_=_global->OutputFolder+"st_"+id_+".png";
	string t_=_global->OutputFolder+"thumb_"+id_+".png";
	uploadSticker(s_,t_);
}
void ofApp::uploadSticker(string sticker_path_,string thumb_path_){
	/*ofxHttpForm form;
	form.action=_global->ServerURL+"upload/action.php";
	form.method=OFX_HTTP_POST;	
	form.addFormField("action", "upload_sticker");
	form.addFormField("guid", _user_id);
	form.addFile("file",path_);
	_http_util.submitForm(form);*/

	HttpForm f=HttpForm(_global->ServerURL+"upload/action.php");
	f.addFile("file",sticker_path_,"image/png");
	f.addFile("thumb",thumb_path_,"image/png");
	f.addFormField("action","upload_sticker");
	f.addFormField("guid",_user_id);

	_form_manager.submitForm(f,false);

	((SceneSleep*)_scene[SceneMode::SLEEP])->enableStart(false);
}

//void ofApp::newResponse(ofxHttpResponse& response){
//	ofLog()<<"http response: "<<response.status<<"  "<<response.responseBody;
//}

void ofApp::newResponse(HttpFormResponse &response){
	ofLog()<<"http response: "<<response.status<<" "<<response.responseBody;
	if(response.status==200){
		sendCompose();
	}
}

void ofApp::saveWebcamImage(){
	
	if(!_webcam.isInitialized()) return;

	/*_tmp_fbo.begin();
		_webcam.draw(0,0);
	_tmp_fbo.end();

	ofPixels pix_;
	_tmp_fbo.readToPixels(pix_);
	ofImage img_;
	img_.setFromPixels(pix_);*/
	
	if(_fr_save>=0 && _fr_save<60){
		_img_rec[_fr_save].setFromPixels(_webcam.getPixels());
		_fr_save++;
	}

	/*img_.allocate(_webcam.getWidth(),_webcam.getHeight(),OF_IMAGE_COLOR);
	img_.setFromPixels(_webcam.getPixels(),_webcam.getWidth(),_webcam.getHeight(),OF_IMAGE_COLOR);*/
	//_img_rec.push_back(img_);

}

void ofApp::initCommunication(){
	_osc_receive.setup(_global->OscPort);
	_osc_sender.setup(_global->CameraOscIP,_global->CameraOscPort);

	/*_http_util.setTimeoutSeconds(160);
	_http_util.start();*/

	//_form_manager.setVerbose(true);
	_form_manager.setTimeOut(180);
	ofAddListener(_form_manager.formResponseEvent, this, &ofApp::newResponse);

	_serial.setup(_global->ArduinoPort,9600);

	//ofAddListener(_http_util.newResponseEvent,this,&ofApp::newResponse);

}
void ofApp::sendRecord(){

	ofLog()<<"Send video start...";

	ofxOscMessage message_;
	message_.setAddress("/video_start");
	message_.addStringArg(_user_id);
	_osc_sender.sendMessage(message_);


	sendSlackMessage("Start Record User: "+_user_id);

}

void ofApp::sendCompose(){
	
	ofLog()<<"Send compose start...";
	ofxOscMessage message_;
	message_.setAddress("/compose_start");
	message_.addStringArg(_user_id);
	_osc_sender.sendMessage(message_);
	
	sendSlackMessage("Send Compose User: "+_user_id);

}

void ofApp::sendBlowerSignal(bool blow_){
	
	if(blow_) _serial.writeByte('b');
	else _serial.writeByte('s');

}
void ofApp::sendSlackMessage(string message_){
	
	/*try{
		HttpForm f=HttpForm(_global->SlackAddress);
		string text_str="{\"text\":\""+message_+"\"}";
		f.addFormField("payload",text_str);

		_form_manager.submitForm(f,false);

	}catch(Exception e){
		cout<<e.message();
	}*/

}

void ofApp::createQRCodeOrder(string qrid_){
	ofBuffer buf_;
	buf_.append(qrid_);
	bool wrtten=ofBufferToFile(_global->QRcodeFolder+qrid_,buf_);

}