#include "SceneBase.h"
#include "ofApp.h"

SceneBase::SceneBase(){}

SceneBase::SceneBase(ofApp *set_ptr,GlobalSource *source_,string back_file_,int stop_){
	_ptr_app=set_ptr;
	_source=source_;
	_img_back.loadImage(back_file_);
	_stop_point=stop_;


	_anim_in=FrameAnimation(300);
	_anim_out=FrameAnimation(300);
}

SceneBase::~SceneBase(){
	if(_mbutton>0) delete [] _arr_button;
}

void SceneBase::draw(bool debug_mode){
	
	ofPushStyle();
	float alpha_=255.0*_anim_in.getPortion()*(1-_anim_out.getPortion());
	ofSetColor(255,alpha_);
		
		drawBack();
		
		if(_img_back.bAllocated()){
				_img_back.draw(0,0,ofGetWidth(),ofGetHeight());
		}
		drawFront();

	ofPopStyle();

	if(debug_mode){
		for(int i=0;i<_mbutton;++i) 
			if(_arr_button[i].isEnable()) _arr_button[i].draw();
	}



}


void SceneBase::handleMousePressed(float mouse_x, float mouse_y){
	for(int i=0;i<_mbutton;++i) 
		if(_arr_button[i].isEnable()){
			if(_arr_button[i].checkClicked(mouse_x,mouse_y)){
				_source->_soundfx[_arr_button[i].getSoundType()].play();
				buttonEvent(_arr_button[i].getIndex());
				//ptr_app->playSound(arr_button[i].getSoundType());
			}
		}
}

void SceneBase::update(float dt_){

	int f_=_source->_mov_back.getCurrentFrame();
	switch(_status){
		case BeforeInit:			
			if(f_>=_stop_point){
				//cout<<"pause at "<<f_;
				_source->_mov_back.setFrame(_stop_point);
				_source->_mov_back.setPaused(true);
				init();
			}
			break;
		case Init:
			_anim_in.update(dt_);
			if(_anim_in.getPortion()==1){
				_status=SceneStatus::Due;
				for(int i=0;i<_mbutton;++i) _arr_button[i].setEnable(true);
			}
			break;		
		case BeforeEnd:
			_anim_out.update(dt_);
			if(_anim_out.getPortion()==1){
				end();
			}
			break;		
	}
}

void SceneBase::prepareInit(){
	ofLog()<<"Prepare init scene";
	_status=SceneStatus::BeforeInit;	
	_source->_mov_back.setPaused(false);

	for(int i=0;i<_mbutton;++i) _arr_button[i].setEnable(false);
}

void SceneBase::prepareEnd(){
	ofLog()<<"Prepare end scene";
	_status=SceneStatus::BeforeEnd;
	_anim_out.restart();

	for(int i=0;i<_mbutton;++i) _arr_button[i].setEnable(false);
}

void SceneBase::init(){
	ofLog()<<"Init scene";
	_status=SceneStatus::Init;
	_anim_in.restart();
	_anim_out.reset();
}

void SceneBase::reset(){
	ofLog()<<"Reset scene";
	_anim_in.reset();
	_anim_out.reset();
}

void SceneBase::end(){
	reset();
	ofLog()<<"End scene";
	_status=SceneStatus::End;
	
}

