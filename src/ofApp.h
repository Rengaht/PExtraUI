#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "HttpFormManager.h"
#include "GlobalParam.h"
#include "GlobalSource.h"
#include "SceneBase.h"
#include "StickerCreate.h"

class ofApp : public ofBaseApp{
	private:
		float _last_millis,_dmillis;
		int _icur_scene;

	public:
		bool _ADJUST_MODE;
		float _SR;

		enum SceneMode {SLEEP,SIGN,RECORD,PREVIEW,STICKER,END};

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		GlobalSource *_source;
		GlobalParam *_global;

		SceneBase* _scene[6];

		void loadScene();
		void changeScene(SceneMode mode_);

		string _user_id;
		void createNewUser();

		ofVideoGrabber _webcam;
		vector<ofImage> _img_rec;
		ofFbo _tmp_fbo;
		int _fr_save;
		void saveWebcamImage();
		int _frame_to_grab[3];

		//OSC
		ofxOscReceiver _osc_receive;
		ofxOscSender _osc_sender;
		void initCommunication();
		void sendRecord();
		void sendCompose();


		//upload
		/*ofxHttpUtils _http_util;
		void newResponse(ofxHttpResponse &response);*/
		HttpFormManager _form_manager;
		void newResponse(HttpFormResponse &response);

		void uploadFile(string& id_);
		void uploadSticker(string sticker_path_,string thumb_path_);

		CreateSticker* _sticker_thread;
		//void gotoEndScene(string& id_);

		bool _redo_once;

		ofSerial _serial;
		void sendBlowerSignal(bool blow_);

		bool _got_video_finish;
		void createQRCodeOrder(string _qrid);


		void sendSlackMessage(string message_);
};


