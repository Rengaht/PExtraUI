#pragma once
#ifndef SCENE_SLEEP_H
#define SCENE_SLEEP_H

#include "SceneBase.h"

class SceneSleep :public SceneBase{
private:
	ofVec2f _region_pos,_region_size;
public:

	SceneSleep(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"sleep.png",1){
		_arr_button=new PButton[1];
		_arr_button[0]=PButton(ofRectangle(816*_ptr_app->_SR,772*_ptr_app->_SR,292*_ptr_app->_SR,292*_ptr_app->_SR),0);
		_mbutton=1;

		_region_pos=ofVec2f(560*_ptr_app->_SR,165*_ptr_app->_SR);
		_region_size=ofVec2f(800*_ptr_app->_SR,450*_ptr_app->_SR);
	}
	void drawFront(){


		ofPushStyle();
		ofSetColor(255,255.0*_anim_in.getPortion()*(1.0-_anim_out.getPortion()));
			_source->_mov_demo.draw(_region_pos.x,_region_pos.y,
									_region_size.x,_region_size.y);
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
