#pragma once
#ifndef SCENE_SLEEP_H
#define SCENE_SLEEP_H

#include "SceneBase.h"

class SceneSleep :public SceneBase{
public:

	SceneSleep(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"sleep.png",1){
		_arr_button=new PButton[1];
		_arr_button[0]=PButton(ofRectangle(816*_ptr_app->_SR,772*_ptr_app->_SR,292*_ptr_app->_SR,292*_ptr_app->_SR),0);
		_mbutton=1;
	}
	void drawContent(){
		ofPushStyle();
		ofSetColor(255,255.0*_anim_in.getPortion()*(1.0-_anim_out.getPortion()));
			_source->_mov_demo.draw(ofGetWidth()/2-_source->_mov_demo.getWidth()/2,20);
		ofPopStyle();
	}
	void init(){
		SceneBase::init();
		_source->_mov_demo.play();
	}
	void update(float dt_){
		SceneBase::update(dt_);
	}
	void end(){
		_ptr_app->changeScene(ofApp::SceneMode::SIGN);
		_source->_mov_demo.stop();
	}

	void buttonEvent(int index){
		switch(index){
			case 0:
				_ptr_app->createNewUser();
				prepareEnd();
				break;
		}
	}

};

#endif
