#pragma once
#ifndef SCENE_SIGN_H
#define SCENE_SIGN_H

#include "SceneBase.h"

class SceneSign :public SceneBase{
private:
	ofImage _brush;
	ofVec2f _region_pos,_region_size;
	ofFbo _fbo_brush;
public:
	SceneSign(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"sign.png",154){
		_arr_button=new PButton[2];
		_arr_button[0]=PButton(ofRectangle(188*_ptr_app->_SR,672*_ptr_app->_SR,220*_ptr_app->_SR,220*_ptr_app->_SR),0);
		_arr_button[1]=PButton(ofRectangle(1518*_ptr_app->_SR,672*_ptr_app->_SR,220*_ptr_app->_SR,220*_ptr_app->_SR),1,1);
		_mbutton=2;

		_region_pos=ofVec2f(570.0*_ptr_app->_SR,382.0*_ptr_app->_SR);
		_region_size=ofVec2f(778.0*_ptr_app->_SR,528.0*_ptr_app->_SR);
		
		_fbo_brush.allocate((int)_region_size.x,(int)_region_size.y,GL_RGBA);
		

	}
	void drawFront(){
		
		_fbo_brush.draw(_region_pos.x,_region_pos.y,_region_size.x,_region_size.y);

	}
	void init(){
		SceneBase::init();
		clearRegion();
	}
	void update(float dt_){
		
		SceneBase::update(dt_);

		if(ofGetMousePressed() && inSignRegion(ofGetMouseX(),ofGetMouseY())){
			_fbo_brush.begin();
			
			ofPushMatrix();

			ofTranslate(-_region_pos.x,-_region_pos.y);
			ofVec2f p_(ofGetPreviousMouseX(),ofGetPreviousMouseY());
			ofVec2f m_(ofGetMouseX(),ofGetMouseY());
			float speed=p_.distance(m_);

			//ofPushStyle();
			ofFill();
			ofSetColor(0);
			float sp_=1.0/ofMap(speed,0,20,1,20);
			for(float i=0;i<=1;i+=sp_){
				float x_=ofLerp(p_.x,m_.x,i);
				float y_=ofLerp(p_.y,m_.y,i);
				ofEllipse(x_,y_,12,12);					
			}
			//ofPopStyle();
		
			ofPopMatrix();

			_fbo_brush.end();
		}

		
	}
	void end(){
		SceneBase::end();
		_ptr_app->changeScene(ofApp::SceneMode::RECORD);

	}

	void buttonEvent(int index){
		switch(index){
			case 1:
				prepareEnd();
				//TODO: save sign;
				break;
			case 0:
				// clear sign board
				clearRegion();
				break;
		}
	}
	void clearRegion(){
		_fbo_brush.begin();
		ofClear(255,0);
		_fbo_brush.end();
	}

	bool inSignRegion(int mx_,int my_){
		return (_region_pos.x<=mx_ && _region_pos.x+_region_size.x>=mx_)
			&& (_region_pos.y<=my_ && _region_pos.y+_region_size.y>=my_);
	}

	ofFbo getSignImage(){
		/*ofImage img_;
		ofPixels pixels;
		_fbo_brush.readToPixels(pixels);
		img_.setFromPixels(pixels,_fbo_brush.getWidth(),_fbo_brush.getHeight(),OF_IMAGE_COLOR);
		return img_;*/
		return _fbo_brush;
	}
};

#endif
