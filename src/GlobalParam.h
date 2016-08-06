#pragma once
#ifndef GLOBAL_PARAM_H
#define GLOBAL_PARAM_H

#include "ofMain.h"
#include "ofxXmlSettings.h"

class GlobalParam{

		static GlobalParam* _instance;	
	
public:

		static string ParamFilePath;
		static string PosFilePath;
		
		float RecordSpan;
		float PreviewRate;
		
		string OutputFolder;
		string OrderFolder;

		ofVec2f MaskSize,MaskPos;
		ofVec2f PrintSize;
		
		int OscPort;
		string CameraOscIP;
		int CameraOscPort;

		string ServerURL;

		vector<ofVec2f> _frame_pos;

		GlobalParam(){
			readParam();
			
		}
		static GlobalParam* GetInstance(){
			if(!_instance)
				_instance=new GlobalParam();
			return _instance;
		}
		void readParam(){
			ofxXmlSettings _param;
			bool file_exist=true;
			if(_param.loadFile(ParamFilePath) ){
				ofLog()<<"mySettings.xml loaded!";
			}else{
				ofLog()<<"Unable to load xml check data/ folder";
				file_exist=false;
			}
			
			RecordSpan=_param.getValue("RECORD_SPAN",2000);			
			PreviewRate=_param.getValue("PREVIEW_RATE",5);
			
			OutputFolder=_param.getValue("OUTPUT_FOLDER","");
			OrderFolder=_param.getValue("ORDER_FOLDER","");

			MaskSize=ofVec2f(_param.getValue("MASK_WIDTH",600),_param.getValue("MASK_HEIGHT",600));
			MaskPos=ofVec2f(_param.getValue("MASK_X",0),_param.getValue("MASK_Y",0));
			
			PrintSize=ofVec2f(_param.getValue("PRINT_WIDTH",566),_param.getValue("PRINT_HEIGHT",566));

			
			OscPort=_param.getValue("OSC_PORT",12888);
			CameraOscIP=_param.getValue("CAMERA_OSC_IP","192.168.1.1");
			CameraOscPort=_param.getValue("CAMERA_OSC_PORT",12777);

			ServerURL=_param.getValue("SERVER_URL","");
			if(!file_exist) saveParameterFile();

			readEndingPos();

		}
		void saveParameterFile(){


			ofxXmlSettings _param;
			
			_param.setValue("RECORD_SPAN",RecordSpan);
			_param.setValue("PREVIEW_RATE",PreviewRate);
			
			_param.setValue("OUTPUT_FOLDER",OutputFolder);
			_param.setValue("ORDER_FOLDER",OrderFolder);

			_param.setValue("MASK_WIDTH",MaskSize.x);
			_param.setValue("MASK_HEIGHT",MaskSize.y);

			_param.setValue("MASK_X",MaskPos.x);
			_param.setValue("MASK_Y",MaskPos.y);
			
			_param.setValue("PRINT_WIDTH",PrintSize.x);
			_param.setValue("PRINT_HEIGHT",PrintSize.y);

			_param.setValue("OSC_PORT",OscPort);
			_param.setValue("CAMERA_OSC_IP",CameraOscIP);
			_param.setValue("CAMERA_OSC_PORT",CameraOscPort);

			_param.setValue("SERVER_URL",ServerURL);

			_param.save(ParamFilePath);


		}

		void readEndingPos(){

			_frame_pos.clear();

			ofxXmlSettings _param;
			if (_param.loadFile(PosFilePath)){
				ofLog()<<"pos xml loaded!";
			}else{
				ofLog()<<"Unable to load xml check data/ folder";
				return;
			}
			int mfr=_param.getNumTags("FRAME");

			for(int i=0;i<mfr;++i){
				ofVec2f pos;
				pos.x=_param.getAttribute("FRAME","X",0,i);
				pos.y=_param.getAttribute("FRAME","Y",0,i);
				_frame_pos.push_back(pos);
			}

		}

};

#endif