#pragma once
#ifndef SCENE_RECORD_H
#define SCENE_RECORD_H

#include "SceneBase.h"

class SceneRecord :public SceneBase{
private:
	ofImage _img_shoot,_img_count;

	int _stage;
	FrameAnimation _ani_count,_ani_record;
	FrameAnimation _ani_blink;
	int _mblink;

	float _start_time;

	ofTrueTypeFont _font_count;
public:
	SceneRecord(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"empty.png",300){
		_mbutton=0;
		_ani_count=FrameAnimation(3000,500);
		_ani_record=FrameAnimation(_ptr_app->_global->RecordSpan,200);
		
		_mblink=0;
		_ani_blink=FrameAnimation(250);
			


		_img_shoot.loadImage("shooting.png");
		_img_count.loadImage("countdown.png");
		_font_count.loadFont("verdanaz.ttf",60,true,true);
	}

	void drawBack(){

		/*ofPushStyle();
		float alpha_=255.0*_anim_in.getPortion()*(1-_anim_out.getPortion());
		ofSetColor(255,alpha_);*/

		if(_status>SceneStatus::BeforeInit){			
			_ptr_app->_webcam.draw(ofGetWidth(),0,-ofGetWidth(),ofGetHeight());
		}

		string num_=ofToString(3-floor(_ani_count.getPortion()/.33));
		float twid=_font_count.getStringBoundingBox(num_,0,0).getWidth();
		switch(_stage){
			case 0:
				ofPushMatrix();								
				ofTranslate(ofGetWidth()/2-twid/2,ofGetHeight()*.58);
					ofPushStyle();					
					ofSetColor(255);
						_font_count.drawString(num_,0,0);
					ofPopStyle();
				ofPopMatrix();
				_img_count.draw(0,0,ofGetWidth(),ofGetHeight());
				break;
			case 1:
				ofPushStyle();
				if(_status==SceneStatus::Due) ofSetColor(255,255.0*(_mblink%2));
					_img_shoot.draw(0,0,ofGetWidth(),ofGetHeight());
				ofPopStyle();
				_ptr_app->saveWebcamImage();
				//_ptr_app->_img_rec.push_back(_ptr_app->_webcam);
				break;
		}
	
		//ofPopStyle();
	}
	void init(){
		SceneBase::init();
		_ptr_app->_fr_save=0;

		_ani_count.restart();
		_ani_record.reset();
		_stage=0;

		_mblink=0;
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_ani_count.update(dt_);
		_ani_record.update(dt_);

		_ani_blink.update(dt_);
		if(_ani_blink.getPortion()==1){
			_mblink++;
			_ani_blink.restart();
		}

		if(_status==SceneStatus::Due){
			switch(_stage){
				case 0:
					if(_ani_count.getPortion()==1){
						_stage=1;
						_ani_record.restart();					
						_ptr_app->sendRecord();
						_ani_blink.restart();
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
