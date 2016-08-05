#pragma once
#ifndef PBUTTON_H
#define PBUTTON_H

#include "ofRectangle.h"
#include "ofMain.h"

class PButton{
	public:
		PButton(){
		}
		PButton(ofRectangle set_area,int set_index){
			button_area=set_area;
			button_index=set_index;
			setSoundType(0);
		}
		PButton(ofRectangle set_area,int set_index, int set_sound){
			button_area=set_area;
			button_index=set_index;
			setSoundType(set_sound);
		}
		bool isEnable(){
			return enabled;
		}
		void setEnable(bool set_enable){
			enabled=set_enable;
		}
		void draw(){
			ofPushStyle();
			ofSetColor(255,0,0,80);
			ofRect(button_area);
			ofPopStyle();
		}
		bool checkClicked(float mouse_x,float mouse_y){
			return (button_area.inside(mouse_x,mouse_y));
		}
		int getIndex(){
			return button_index;
		}
		void setSoundType(int set_type){
			sound_type=set_type;
		}
		int getSoundType(){
			return sound_type;
		}

	private:
		bool enabled;
		int button_index;
		int sound_type;
		ofRectangle button_area;
};



#endif