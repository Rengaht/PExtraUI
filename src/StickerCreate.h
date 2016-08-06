#pragma once
#ifndef STICKER_CREATE_H
#define STICKER_CREATE_H


#include "ofMain.h"

class CreateSticker:public ofThread{

	ofImage _sticker;
	ofImage _img_top;
	ofFbo _img_sign;
	ofImage _back;
	ofImage _thumb;

	ofVec2f _ps;

	string _id;
	string _output_path;
	ofVec2f _sign_pos,_sign_size;

	
public:

	ofEvent<string> _ready_to_upload;
	ofEvent<string> _ready_to_end;

	CreateSticker(ofVec2f ps_,string path_){
		_img_top.loadImage("sticker_top.png");
		_ps=ps_;
		_output_path=path_;

		_sign_pos=ofVec2f(0.689,0.572);
		_sign_size=ofVec2f(0.214,0.145);
	}
	void setup(string id_,ofImage sticker_,ofImage thumb_){
		_id=id_;
		//_back=back_;		
		//_img_sign=sign_;		
		_sticker=sticker_;
		_thumb=thumb_;
	}

	void threadedFunction(){

		//createSingleSticker();
		string path_=_output_path+"st_"+_id+".png";		
		_sticker.saveImage(path_);

		//ofNotifyEvent(_ready_to_end,_id);

		string thpath_=_output_path+"thumb_"+_id+".png";
		_thumb.mirror(false,true);
		_thumb.saveImage(thpath_);

		ofNotifyEvent(_ready_to_upload,_id);

		createStickerPrint();
		
	}

	void createStickerPrint(){



		//_ptr_app->uploadSticker(path_,thpath_);

		ofFbo print_;
		print_.allocate(1500,900);

		print_.begin();
		ofClear(255);
		_sticker.draw(100,900/2-_back.getHeight()/2);
		_sticker.draw(800,900/2-_back.getHeight()/2);
		print_.end();

		ofPixels pix_;
		print_.readToPixels(pix_);

		ofImage img_;
		img_.setFromPixels(pix_);

		img_.saveImage(ofFile(_output_path+"print_"+_id+".png"));

		ofBuffer buf_;
		buf_.append(_id);
		bool wrtten=ofBufferToFile(_output_path+"print_"+_id,buf_);

	}
	//void createSingleSticker(){

	//	
	//	ofFbo st_;
	//	st_.allocate(_ps.x,_ps.y);

	//	st_.begin();
	//	ofClear(255);
	//	_back.draw(_ps.x,0,-_ps.x,_ps.y);
	//	_img_top.draw(0,0,_ps.x,_ps.y);
	//	_img_sign.draw(_sign_pos.x*_ps.x,_sign_pos.y*_ps.y,
	//		_sign_size.x*_ps.x,_sign_size.y*_ps.y);

	//	st_.end();

	//	ofPixels pix_;
	//	st_.readToPixels(pix_);

	//	//ofImage img_;
	//	_sticker.setFromPixels(pix_);

	//	st_.destroy();

	//	//return img_;
	//}
};



#endif