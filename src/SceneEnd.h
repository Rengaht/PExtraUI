#pragma once
#ifndef SCENE_END_H
#define SCENE_END_H

#include "SceneBase.h"

class SceneEnd :public SceneBase{
public:
	SceneEnd(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"end.png",514){
		_mbutton=0;
	}
	void drawContent(){

	}
	void init(){
		SceneBase::init();
		_source->_mov_back.setPaused(false);
	}
	void update(float dt_){
		SceneBase::update(dt_);
		if(_status==SceneStatus::Due && _source->_mov_back.getIsMovieDone()){
			prepareEnd();
		}
	}
	void end(){
		SceneBase::end();
		_ptr_app->changeScene(ofApp::SceneMode::SLEEP);
		_source->_mov_back.setPaused(false);
		_source->_mov_back.play();
		_source->_mov_back.setFrame(0);
	}

	void buttonEvent(int index){
		
	}

};

#endif
