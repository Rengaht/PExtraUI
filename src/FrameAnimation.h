#pragma once
#ifndef FRAME_ANIMATION_H
#define FRAME_ANIMATION_H
#include "ofMath.h"

class FrameAnimation{
	public:

		static float millis_per_frame;

		FrameAnimation(){
			setup(1,0,true);
		}
		FrameAnimation(float set_length){
			setup(set_length,0.0f,true);
		}
		/*FrameAnimation(float set_length,bool elastic){
		setup(set_length,0.0f,elastic);
		}*/
		FrameAnimation(float set_length,float set_delay){
			setup(set_length,set_delay,true);
		}
	
		void start(){
			ani_start=true;
			//ani_t=0;
		}
		bool isFinished(){
			return (ani_t==1);
		}
		bool isPlaying(){
			return ani_start;
		}
		float getPortion(){
			if(!ani_start) return 0;
			if(ani_t<0) return 0;
			if(ani_t==1) return 1;
			//return ofClamp(sinf(ani_t*(HALF_PI)),0,1);
			/*float k=0.5f;
			float r=is_elastic?(0.5f-0.51f*cosf(PI*ani_t+k*ani_t-k*0.5f)):(0.5-0.5*cosf(ani_t*(PI)));*/
			float r=ofClamp(ani_t/ani_due,0,1);
			return r;
		}
		
		void update(float delta_t){


			if(!ani_start) return;

			if(ani_t<ani_due) ani_t+=delta_t;

		}
		void reset(){
			ani_start=false;
			ani_t=-delay_t;
			// Start();
		}
		void restart(){
			reset();
			start();
		}

	
		void setElastic(bool set_elastic){
			is_elastic=set_elastic;
		}
	private:
		float ani_t,ani_vel,ani_due;
		float delay_t;

		
		bool ani_start;
		bool is_elastic;

		void setup(float set_length,float set_delay,bool set_elastic){

			//ani_vel=1/(set_length*millis_per_frame);
			//ofLog()<<ani_vel;
			ani_due=set_length;
			delay_t=set_delay;

			is_elastic=set_elastic;

			reset();
		}
};




#endif
