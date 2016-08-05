#pragma once
#ifndef SCENE_PREVIEW_H
#define SCENE_PREVIEW_H

#include "SceneBase.h"

class ScenePreview :public SceneBase{
private:
	ofVec2f _region_pos,_region_size;
	bool _redo;

	float _cur_fr;
	int _mfr;

public:
	ScenePreview(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"preview.png",327){
		_arr_button=new PButton[2];
		_arr_button[0]=PButton(ofRectangle(188*_ptr_app->_SR,672*_ptr_app->_SR,220*_ptr_app->_SR,220*_ptr_app->_SR),0);
		_arr_button[1]=PButton(ofRectangle(1518*_ptr_app->_SR,672*_ptr_app->_SR,220*_ptr_app->_SR,220*_ptr_app->_SR),1);
		_mbutton=2;

		_region_pos=ofVec2f(570*_ptr_app->_SR,382*_ptr_app->_SR);
		_region_size=ofVec2f(778*_ptr_app->_SR,528*_ptr_app->_SR);
	}
	void drawContent(){
		
		if(_cur_fr>=0 && _cur_fr<_mfr)
			_ptr_app->_img_rec[int(_cur_fr)].draw(_region_pos.x,_region_pos.y,_region_size.x,_region_size.y);
		
	}
	void init(){
		SceneBase::init();
		_cur_fr=0;
		_mfr=_ptr_app->_img_rec.size();
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_cur_fr+=1.0/_ptr_app->_global->PreviewRate;
		_cur_fr=fmod(_cur_fr,_mfr);
	}
	void end(){
		SceneBase::end();
		
		if(_redo) _ptr_app->changeScene(ofApp::SceneMode::RECORD);
		else _ptr_app->changeScene(ofApp::SceneMode::STICKER);

	}

	void buttonEvent(int index){
		switch(index){
			case 0:
				_redo=true;
				prepareEnd();
				break;
			case 1:
				_ptr_app->sendCompose();

				_redo=false;
				prepareEnd();
				break;
		}
	}

};

#endif