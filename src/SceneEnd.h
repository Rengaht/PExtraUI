#pragma once
#ifndef SCENE_END_H
#define SCENE_END_H

#include "SceneBase.h"

class SceneEnd :public SceneBase{

private:
	FrameAnimation _anim_fade;

	ofImage _img_sticker;

	FrameAnimation _anim_move[3];
	FrameAnimation _anim_notice_sound;

	//ofVec2f _sticker_pos[4];
	float _sticker_rotate[4];
	ofVec2f _sticker_size;
	ofVec2f _sticker_pos;
	//float _sticker_keyframe[4];


public:
	SceneEnd(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"empty.png",809){
		_mbutton=0;
		float mpf=1000.0/30.0;
		_anim_move[0]=FrameAnimation(15.0*mpf,5.0*mpf);
		_anim_move[1]=FrameAnimation(10.0*mpf,20.0*mpf);
		_anim_move[2]=FrameAnimation(10.0*mpf,40.0*mpf);
		//_anim_fade=FrameAnimation(7.0*mpf,43.0*mpf);
		_anim_fade=FrameAnimation(9.0*mpf,42.0*mpf);

		_anim_notice_sound=FrameAnimation(103*mpf);

		/*_sticker_keyframe[0]=405;
		_sticker_keyframe[1]=420;
		_sticker_keyframe[2]=430;
		_sticker_keyframe[3]=440;


		_sticker_size=ofVec2f(758*_ptr_app->_SR,758*_ptr_app->_SR);
		_sticker_pos[0]=ofVec2f(556*_ptr_app->_SR,126*_ptr_app->_SR);
		_sticker_pos[1]=ofVec2f(404*_ptr_app->_SR,116*_ptr_app->_SR);
		_sticker_pos[2]=ofVec2f(638*_ptr_app->_SR,138*_ptr_app->_SR);
		_sticker_pos[3]=ofVec2f(576*_ptr_app->_SR,141*_ptr_app->_SR);*/
		
		_sticker_rotate[0]=0;
		_sticker_rotate[1]=30;
		_sticker_rotate[2]=-20;
		_sticker_rotate[3]=0;

		_sticker_size=ofVec2f(758*_ptr_app->_SR,758*_ptr_app->_SR);
		_sticker_pos=ofVec2f(556*_ptr_app->_SR,126*_ptr_app->_SR);

	}
	
	void draw(bool debug_mode){
		SceneBase::draw(debug_mode);

		if(_status==SceneStatus::BeforeInit){
			ofPushStyle();

			float fr_=_source->_mov_back.getCurrentFrame();

			float a_=ofLerp(0.0,255.0,ofClamp(ofMap(fr_,442,451,0,1.0),0,1));
			//cout<<a_<<" ";
			ofSetColor(255,255.0-255.0*a_);

			
				//float x_=_sticker_pos[0].x;
				//float y_=_sticker_pos[0].y;
				float r_=_sticker_rotate[0];
				//
				//
				for(int i=0;i<3;++i){
					float p_=_anim_move[i].getPortion();
				//	if(fr_>=_sticker_keyframe[i] && fr_<=_sticker_keyframe[i+1]){
				//		float p_=ofMap(fr_,_sticker_keyframe[i],_sticker_keyframe[i+1],0,1);
						float t_=getEaseInOutPosition(p_);
				//	
				//	//if(_anim_move[i].getPortion()==1){
				//	//	//stop
				//	//	ofLog()<<"anim "<<i<<" finish "<<_source->_mov_back.getCurrentFrame();
				//	//	_source->_mov_back.setPaused(true);
				//	//}
				//		_source->_mov_back.setPaused(false);
				//		x_+=ofLerp(0,_sticker_pos[i+1].x-_sticker_pos[i].x,t_);
				//		y_+=ofLerp(0,_sticker_pos[i+1].y-_sticker_pos[i].y,t_);
						r_+=ofLerp(0,_sticker_rotate[i+1]-_sticker_rotate[i],t_);
				//	}
				}

				int nfr=ofClamp(fr_-400,0,40);
				float x_=_sticker_pos.x+_ptr_app->_global->_frame_pos[nfr].x*_ptr_app->_SR;
				float y_=_sticker_pos.y+_ptr_app->_global->_frame_pos[nfr].y*_ptr_app->_SR;

				ofPushMatrix();
				ofTranslate(x_,y_);

				ofTranslate(_sticker_size.x/2,_sticker_size.y/2);
				ofRotate(r_);
				ofTranslate(-_sticker_size.x/2,-_sticker_size.y/2);

					_img_sticker.draw(0,0,_sticker_size.x,_sticker_size.y);		

				ofPopMatrix();
			ofPopStyle();
		}

	}


	void prepareInit(){
		SceneBase::prepareInit();
		
		_img_sticker.loadImage(_ptr_app->_global->OutputFolder+"st_"+_ptr_app->_user_id+".png");

		for(int i=0;i<3;++i) _anim_move[i].restart();
		_anim_fade.restart();
		_anim_notice_sound.restart();
	}
	void init(){
		SceneBase::init();
		_source->_mov_back.setPaused(false);

	}
	void update(float dt_){
		SceneBase::update(dt_);
		for(int i=0;i<3;++i) _anim_move[i].update(dt_);
		_anim_fade.update(dt_);
		_anim_notice_sound.update(dt_);
		if(_anim_notice_sound.getPortion()==1){
			_source->_soundfx[4].play();
			_anim_notice_sound.reset();
		}


		if(_status==SceneStatus::Due && _source->_mov_back.getIsMovieDone()){
			prepareEnd();
		}
	}
	void end(){
		SceneBase::end();
		
		_source->_mov_back.setPaused(false);
		_source->_mov_back.play();
		_source->_mov_back.setFrame(0);

		_ptr_app->changeScene(ofApp::SceneMode::SLEEP);
	}

	void buttonEvent(int index){
		
	}

	void setStickerImage(ofImage sticker_){
		_img_sticker=sticker_;
	}

	float getEaseInOutPosition(float pos){
		pos/=.5;
		if(pos<1) return .5*pos*pos;
		pos-=1;
		return -.5*(pos*(pos-2)-1);
	
	}

};

#endif
