#pragma once
#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "ofMain.h"
#include "PButton.h"
#include "FrameAnimation.h"
#include "GlobalSource.h"

class ofApp;

class SceneBase{

	public:

		enum SceneStatus {BeforeInit,Init,Due,BeforeEnd,End};

		ofApp* _ptr_app;
		GlobalSource* _source;

		PButton* _arr_button;
		int _mbutton;
		int _stop_point;

		int _status;
		ofImage _img_back;

		FrameAnimation _anim_in,_anim_out;

		SceneBase();
		SceneBase(ofApp *set_ptr,GlobalSource *source_,string back_file_,int stop_);
		virtual ~SceneBase();

		virtual void draw(bool debug_mode=false);
		virtual void drawFront(){}
		virtual void drawBack(){}

		void handleMousePressed(float mouse_x,float mouse_y);
		
		
		virtual void prepareInit();
		void prepareEnd();

		
		virtual void init();
		virtual void update(float dt_);
		virtual void end();
		virtual void reset();

		virtual void buttonEvent(int index){}

		

};



#endif // !SCENE_BASE_H