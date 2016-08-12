#pragma once
#ifndef GLOBAL_SOURCE_H
#define GLOBAL_SOURCE_H

#include "ofxHapPlayer.h"

class GlobalSource{

	static GlobalSource* _instance;	
public:

	ofxHapPlayer _mov_back;
	ofxHapPlayer _mov_demo;
	ofSoundPlayer  _sound_bgm; //_sound_ok,_sound_camera,_sound_select,_sound_countdown;	
	ofSoundPlayer _soundfx[5];

	GlobalSource(){
		loadSources();
	}
	static GlobalSource* GetInstance(){
		if(!_instance)
			_instance=new GlobalSource();
		return _instance;
	}
	void loadSources(){
		
		_mov_back.load("back_0808.mov");
		_mov_back.setLoopState(ofLoopType::OF_LOOP_NONE);

		_mov_demo.load("demo.mov");
		_mov_demo.setLoopState(ofLoopType::OF_LOOP_NORMAL);

		_sound_bgm.loadSound("sound/back.mp3");
		_sound_bgm.setLoop(true);
		_sound_bgm.play();
		_sound_bgm.setVolume(.5);

		_soundfx[0].loadSound("sound/sel_button.wav");
		_soundfx[1].loadSound("sound/ok_button.wav");
		_soundfx[2].loadSound("sound/camera.wav");
		_soundfx[3].loadSound("sound/countdown_3.wav");
		_soundfx[4].loadSound("sound/notice.wav");
	}
	void update(){
		_mov_back.update();
		_mov_demo.update();
	}

};



#endif