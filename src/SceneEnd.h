#pragma once
#ifndef SCENE_END_H
#define SCENE_END_H

#include "SceneBase.h"

class SceneEnd :public SceneBase{

private:
	FrameAnimation _anim_move[3];
	FrameAnimation _anim_fade;
	ofImage _img_sticker;

	ofVec2f _sticker_pos[4];
	ofVec2f _sticker_size;

public:
	SceneEnd(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"empty.png",514){
		_mbutton=0;
		float mpf=1000.0/60.0;
		_anim_move[0]=FrameAnimation(20.0*mpf);
		_anim_move[1]=FrameAnimation(10.0*mpf,20.0*mpf);
		_anim_move[2]=FrameAnimation(10.0*mpf,40.0*mpf);
		_anim_fade=FrameAnimation(7.0*mpf,43.0*mpf);


		_sticker_size=ofVec2f(758*_ptr_app->_SR,758*_ptr_app->_SR);
		_sticker_pos[0]=ofVec2f(623*_ptr_app->_SR,126*_ptr_app->_SR);
		_sticker_pos[1]=ofVec2f(502*_ptr_app->_SR,126*_ptr_app->_SR);
		_sticker_pos[2]=ofVec2f(693*_ptr_app->_SR,138*_ptr_app->_SR);
		_sticker_pos[3]=ofVec2f(636*_ptr_app->_SR,138*_ptr_app->_SR);

	}
	
	void draw(bool debug_mode){
		SceneBase::draw(debug_mode);

		if(_status==SceneStatus::BeforeInit){
			ofPushStyle();
			ofSetColor(255,255.0-255.0*_anim_fade.getPortion());
				float x_=_sticker_pos[0].x;
				float y_=_sticker_pos[0].y;
				for(int i=0;i<3;++i){
					x_+=ofLerp(0,_sticker_pos[i+1].x-_sticker_pos[i].x,_anim_move[i].getPortion());
					y_+=ofLerp(0,_sticker_pos[i+1].y-_sticker_pos[i].y,_anim_move[i].getPortion());
				}
				_img_sticker.draw(x_,y_,_sticker_size.x,_sticker_size.y);		
			ofPopStyle();
		}

	}


	void prepareInit(){
		SceneBase::prepareInit();
		_img_sticker.loadImage(_ptr_app->_global->OutputFolder+"st_"+_ptr_app->_user_id+".png");
		for(int i=0;i<3;++i) _anim_move[i].restart();
		_anim_fade.restart();
	}
	void init(){
		SceneBase::init();
		_source->_mov_back.setPaused(false);
	}
	void update(float dt_){
		SceneBase::update(dt_);
		for(int i=0;i<3;++i) _anim_move[i].update(dt_);
		_anim_fade.update(dt_);

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

	void setStickerImage(ofImage sticker_){
		_img_sticker=sticker_;
	}

};

#endif
