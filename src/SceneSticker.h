#pragma once
#ifndef SCENE_STICKER_H
#define SCENE_STICKER_H

#include "SceneBase.h"

class SceneSticker :public SceneBase{
private:
	int _sel,_next_sel;
	ofVec2f _region_pos,_region_size;
	ofVec2f _sign_pos,_sign_size;
	
	ofImage _img_mask,_img_top;
	ofFbo _img_sign;
	ofImage _img_sticker[3];

	FrameAnimation _anim_change;
	ofImage _img_final_sticker;

	ofImage createSticker(ofImage back_){

		ofVec2f ps_=_ptr_app->_global->PrintSize;
		back_.crop(back_.getWidth()/2-back_.getHeight()/2,0,back_.getHeight(),back_.getHeight());
		back_.resize(ps_.x,ps_.y);

		float rx=_img_mask.getWidth()/_ptr_app->_global->MaskSize.x;
		float ry=_img_mask.getHeight()/_ptr_app->_global->MaskSize.y;

		//back_.update();
		ofImage img_;
		img_.allocate(ps_.x,ps_.y,OF_IMAGE_COLOR_ALPHA);
		for(int i=0;i<ps_.x;++i)
			for(int j=0;j<ps_.y;++j){

				ofColor c=_img_mask.getColor(floor(ofMap(i,0,ps_.x,0,_img_mask.getWidth())),
					floor(ofMap(j,0,ps_.y,0,_img_mask.getHeight())));
				ofColor src=back_.getColor(i,j);
				img_.setColor(i,j,ofColor(src,c.a));         
			}
		img_.update();
		//img_.save(ofToString(ofRandom(20))+".png");
		return img_;
	}
	void createStickerPrint(string id_,ofImage back_){
		
		createSingleSticker(back_);
		string path_=_ptr_app->_global->OutputFolder+"st_"+id_+".png";
		_img_final_sticker.saveImage(path_);
		_ptr_app->uploadSticker(path_);

		ofFbo print_;
		print_.allocate(1500,900);

		print_.begin();
		ofClear(255);
		_img_final_sticker.draw(100,900/2-back_.getHeight()/2);
		_img_final_sticker.draw(800,900/2-back_.getHeight()/2);
		print_.end();

		ofPixels pix_;
		print_.readToPixels(pix_);

		ofImage img_;
		img_.setFromPixels(pix_);

		img_.saveImage(ofFile(_ptr_app->_global->OutputFolder+"print_"+id_+".png"));
		
		ofBuffer buf_;
		buf_.append(id_);
		bool wrtten=ofBufferToFile(_ptr_app->_global->OrderFolder+"print_"+id_,buf_);
		
	}
	void createSingleSticker(ofImage back_){
		
		ofVec2f ps_=_ptr_app->_global->PrintSize;

		ofFbo st_;
		st_.allocate(ps_.x,ps_.y);

		st_.begin();
		ofClear(255);
		back_.draw(ps_.x,0,-ps_.x,ps_.y);
		_img_top.draw(0,0,ps_.x,ps_.y);
		_img_sign.draw(_sign_pos.x*ps_.x,_sign_pos.y*ps_.y,
			_sign_size.x*ps_.x,_sign_size.y*ps_.y);

		st_.end();

		ofPixels pix_;
		st_.readToPixels(pix_);

		//ofImage img_;
		_img_final_sticker.setFromPixels(pix_);
		
		st_.destroy();
		
		//return img_;
	}


public:
	SceneSticker(ofApp *set_ptr,GlobalSource *source_):SceneBase(set_ptr,source_,"sticker.png",400){
		_arr_button=new PButton[3];
		_arr_button[0]=PButton(ofRectangle(320*_ptr_app->_SR,396*_ptr_app->_SR,130*_ptr_app->_SR,220*_ptr_app->_SR),0);
		_arr_button[1]=PButton(ofRectangle(1422*_ptr_app->_SR,396*_ptr_app->_SR,130*_ptr_app->_SR,220*_ptr_app->_SR),1);
		_arr_button[2]=PButton(ofRectangle(816*_ptr_app->_SR,772*_ptr_app->_SR,292*_ptr_app->_SR,292*_ptr_app->_SR),2);
		_mbutton=3;

		_region_pos=ofVec2f(556*_ptr_app->_SR,126*_ptr_app->_SR);
		_region_size=ofVec2f(758*_ptr_app->_SR,758*_ptr_app->_SR);

		_sign_pos=ofVec2f(0.689,0.572);
		_sign_size=ofVec2f(0.214,0.145);

		_img_mask.loadImage("sticker_mask.png");
		_img_top.loadImage("sticker_top.png");

		_next_sel=0;
		_sel=1;
		

		_anim_change=FrameAnimation(500);
		//_anim_change.restart();

	}
	void drawBack(){
		if(_status>SceneStatus::BeforeInit){
			if(_next_sel!=_sel){
				ofPushStyle();
				ofSetColor(255,255-255*ofClamp(_anim_change.getPortion(),0,0.5)*2);
					_img_sticker[_sel].draw(_region_pos.x+_region_size.x,_region_pos.y,-_region_size.x,_region_size.y);
				ofPopStyle();

				ofPushStyle();
				ofSetColor(255,255*ofClamp(_anim_change.getPortion()-0.5,0,0.5)*2);
					_img_sticker[_next_sel].draw(_region_pos.x+_region_size.x,_region_pos.y,-_region_size.x,_region_size.y);
				ofPopStyle();	
			}else{
				_img_sticker[_sel].draw(_region_pos.x+_region_size.x,_region_pos.y,-_region_size.x,_region_size.y);
			}

			_img_top.draw(_region_pos.x,_region_pos.y,_region_size.x,_region_size.y);
			_img_sign.draw(_region_pos.x+_sign_pos.x*_region_size.x,_region_pos.y+_sign_pos.y*_region_size.y,
							_sign_size.x*_region_size.x,_sign_size.y*_region_size.y);
		}
		
	}
	void drawFront(){
		ofPushStyle();
		ofSetColor(255,0,0);
			ofDrawBitmapString(ofToString(_sel)+"->"+ofToString(_next_sel),20,20);
		ofPopStyle();
	}	
	void init(){
		SceneBase::init();
		_next_sel=1;
		_sel=0;
		_anim_change.restart();
	}
	void update(float dt_){
		SceneBase::update(dt_);
		_anim_change.update(dt_);
		if(_anim_change.getPortion()==1 && _sel!=_next_sel){
			_sel=_next_sel;			
		}

	}
	void end(){
		SceneBase::end();
		_ptr_app->changeScene(ofApp::SceneMode::END);

	}

	void buttonEvent(int index){
		if(_anim_change.getPortion()!=1) return;
		switch(index){
			case 0:
				_next_sel=(_sel-1+3)%3;
				_anim_change.restart();
				break;
			case 1:
				_next_sel=(_sel+1)%3;
				_anim_change.restart();
				break;
			case 2:
				string id_=_ptr_app->_user_id;
				
				createStickerPrint(id_,_img_sticker[_sel]);
				
				prepareEnd();
				break;
		}
	}
	void setStickerImage(ofFbo sign_,ofImage* img_){
		_img_sign=sign_;
		for(int i=0;i<3;++i) _img_sticker[i]=createSticker(img_[i]);
	}
	

};

#endif
