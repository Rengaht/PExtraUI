#pragma once
#ifndef GLOBAL_SOURCE_H
#define GLOBAL_SOURCE_H

#include "ofxHapPlayer.h"

class GlobalSource{

	static GlobalSource* _instance;	
public:

	ofxHapPlayer _mov_back;
	ofxHapPlayer _mov_demo;

	GlobalSource(){
		loadSources();
	}
	static GlobalSource* GetInstance(){
		if(!_instance)
			_instance=new GlobalSource();
		return _instance;
	}
	void loadSources(){
		
		_mov_back.load("back.mov");
		_mov_back.setLoopState(ofLoopType::OF_LOOP_NONE);

		_mov_demo.load("demo.mov");
		_mov_demo.setLoopState(ofLoopType::OF_LOOP_NORMAL);
	}
	void update(){
		_mov_back.update();
		_mov_demo.update();
	}

};



#endif