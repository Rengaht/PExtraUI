#pragma once
#ifndef SCENE_RECORD_H
#define SCENE_RECORD_H

#include "SceneBase.h"

class SceneRecord :public SceneBase{
private:
	ofImage _img_shoot,_img_count,_img_hint,_img_delay;

	int _stage;
	FrameAnimation _ani_count,_ani_record,_ani_hint;
	FrameAnimation _ani_blink,_ani_delay;
	int _mblink;

	float _start_time;

	ofTrueTypeFont _font_count;
public:
	SceneRecord(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"empty.png",300){
		_mbutton=0;
		
		_ani_hint=FrameAnimation(3000,0);
		_ani_count=FrameAnimation(3000,500);
		/*_ani_count[1]=FrameAnimation(1000,1500);
		_ani_count[2]=FrameAnimation(1000,2500);*/

		_ani_record=FrameAnimation(_ptr_app->_global->RecordSpan,200);
		_ani_delay=FrameAnimation(1000);

		_mblink=0;
		_ani_blink=FrameAnimation(250);
			
		//_anim_out=FrameAnimation(500);

		_img_shoot.loadImage("shooting.png");
		_img_count.loadImage("countdown.png");
		_img_hint.loadImage("hint.png");
		_img_delay.loadImage("delay.png");

		_font_count.loadFont("verdanaz.ttf",60,true,true);
	}

	void drawBack(){

		/*ofPushStyle();
		float alpha_=255.0*_anim_in.getPortion()*(1-_anim_out.getPortion());
		ofSetColor(255,alpha_);*/

		if(_status>SceneStatus::BeforeInit){			
			_ptr_app->_webcam.draw(ofGetWidth(),0,-ofGetWidth(),ofGetHeight());
			
		
			float total_=0;
			/*for(int i=0;i<3;++i) total_+=_ani_count[i].getPortion();*/
			int c=ofClamp(3-floor(_ani_count.getPortion()/0.33),1,3);
			string num_=ofToString(c);

			float twid=_font_count.getStringBoundingBox(num_,0,0).getWidth();
			switch(_stage){
				case 0:
					_img_hint.draw(0,0,ofGetWidth(),ofGetHeight());
					break;
				case 1:
					ofPushMatrix();								
					ofTranslate(ofGetWidth()/2-twid/2,ofGetHeight()*.58);
						ofPushStyle();					
						ofSetColor(255);
							_font_count.drawString(num_,0,0);
						ofPopStyle();
					ofPopMatrix();
					_img_count.draw(0,0,ofGetWidth(),ofGetHeight());
					break;
				case 2:
					ofPushStyle();
					if(_status==SceneStatus::Due) ofSetColor(255,255.0*(_mblink%2));
						_img_shoot.draw(0,0,ofGetWidth(),ofGetHeight());
					ofPopStyle();
					_ptr_app->saveWebcamImage();
					//_ptr_app->_img_rec.push_back(_ptr_app->_webcam);
					break;
				case 3:
					_img_delay.draw(0,0,ofGetWidth(),ofGetHeight());
					break;
			}
		}
		//ofPopStyle();
	}
	void init(){
		SceneBase::init();
		_ptr_app->_fr_save=0;

		//for(int i=0;i<3;++i) _ani_count[i].restart();
		_ani_hint.restart();
		_source->_soundfx[4].play();

		//_ani_count.restart();

		_ptr_app->sendBlowerSignal(true);

		_ani_record.reset();
		_ani_count.reset();
		_ani_delay.reset();
		
		_stage=0;

		_mblink=0;
	}
	void update(float dt_){
		SceneBase::update(dt_);
		//for(int i=0;i<3;++i) _ani_count[i].update(dt_);
		_ani_count.update(dt_);
		_ani_record.update(dt_);

		_ani_hint.update(dt_);	
		_ani_delay.update(dt_);


		_ani_blink.update(dt_);
		if(_ani_blink.getPortion()==1){
			_mblink++;
			_ani_blink.restart();
		}

		if(_status==SceneStatus::Due){
			switch(_stage){
				case 0:
					if(_ani_hint.getPortion()==1){
						
						
						_source->_soundfx[3].play();
						_stage=1;
						_ani_count.restart();
						return;
					}
					break;
				case 1:
					if(_ani_count.getPortion()==1){
						_stage=2;
										
						_ptr_app->sendRecord();
						_source->_soundfx[2].play();

						_ani_record.restart();	
						_ani_blink.restart();
						
						return;
					}
					break;
				case 2:
					if(_ani_record.getPortion()==1){
						_stage=3;
						_ani_delay.restart();
						
					}
					break;
				case 3:
					if(_ani_delay.getPortion()==1){

						_ptr_app->sendBlowerSignal(false);
						prepareEnd();
					}
					break;
			}
		}

	}
	void reset(){
		SceneBase::reset();
		_ani_hint.reset();
		_ani_count.reset();
		_ani_record.reset();
		_stage=-1;
	}
	void end(){
		SceneBase::end();
		//for(int i=0;i<3;++i) _ani_count[i].reset();
		
		_ptr_app->changeScene(ofApp::SceneMode::PREVIEW);
	}

	void buttonEvent(int index){
	
	}

};

#endif
