#pragma once
#ifndef SCENE_RECORD_H
#define SCENE_RECORD_H

#include "SceneBase.h"

class SceneRecord :public SceneBase{
private:
	ofImage _img_shoot;

	int _stage;
	FrameAnimation _ani_count,_ani_record;
	float _start_time;

public:
	SceneRecord(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"countdown.png",300){
		_mbutton=0;
		_ani_count=FrameAnimation(3000,500);
		_ani_record=FrameAnimation(_ptr_app->_global->RecordSpan,300);
		
		_img_shoot.loadImage("shooting.png");
	}

	void drawContent(){

		ofPushStyle();
		float alpha_=255.0*_anim_in.getPortion()*(1-_anim_out.getPortion());
		ofSetColor(255,alpha_);

		if(_status>SceneStatus::BeforeInit) _ptr_app->_webcam.draw(0,0,ofGetWidth(),ofGetHeight());
		switch(_stage){
			case 0:
				ofPushMatrix();
				ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
				ofScale(5,5);
					ofPushStyle();					
					ofSetColor(255);
						ofDrawBitmapString(3-floor(_ani_count.getPortion()/.33),0,0);
					ofPopStyle();
				ofPopMatrix();
				_img_back.draw(0,0,ofGetWidth(),ofGetHeight());
				break;
			case 1:
				_img_shoot.draw(0,0,ofGetWidth(),ofGetHeight());
				_ptr_app->saveWebcamImage();
				//_ptr_app->_img_rec.push_back(_ptr_app->_webcam);
				break;
		}

		ofPopStyle();
	}
	void init(){
		SceneBase::init();
		_ptr_app->_img_rec.clear();

		_ani_count.restart();
		_ani_record.reset();
		_stage=0;
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_ani_count.update(dt_);
		_ani_record.update(dt_);

		if(_status==SceneStatus::Due){
			switch(_stage){
				case 0:
					if(_ani_count.getPortion()==1){
						_stage=1;
						_ani_record.restart();					
						_ptr_app->sendRecord();
					}
					break;
				case 1:
					if(_ani_record.getPortion()==1){
						prepareEnd();
					}
					break;
			}
		}

	}
	void end(){
		SceneBase::end();
		_ani_count.reset();
		_ani_record.reset();
		_stage=-1;
		_ptr_app->changeScene(ofApp::SceneMode::PREVIEW);
	}

	void buttonEvent(int index){
	
	}

};

#endif
