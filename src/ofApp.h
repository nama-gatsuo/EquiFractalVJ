#pragma once

#include "ofMain.h"
#include "Scene/scenebase.h"
#include "Scene/mandelbox.h"
#include "Scene/Tgrad.h"
#include "Scene/PseudoKnightyan.h"
#include "Scene/Hartverdrahtet.h"

#include "PostEffect.h"

#include "ofxOsc.h"
#include "ofxMQTT.h"
#include "ofxJSON.h"
#include "ofxTrueTypeFontUC.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void windowResized(int w, int h);

	void exit();
	void onOnline();
	void onOffline();
	void onMessage(ofxMQTTMessage& msg);
	
private:
	ofEasyCam cam;
	ofVboMesh plane;
	ofFbo target;
	
	unsigned mode = 0;
	float dt = 1.0;
	vector<shared_ptr<SceneBase>> scenes;

	PostEffect pe;

	ofxJSONElement json;
	ofxMQTT mqtt;
	// mqtt senser values from senstick
	string timeStr;
	ofVec3f accel; // accelerometer
	ofVec3f gyro; // gyro
	float magnetic;  // earth magnetic
	float humidity; // humidity
	float uv; // ultraviolet
	float brigtness; // brightness
	float air; // air presssure

	ofxOscReceiver receiver;
	ofxTrueTypeFontUC font;
};
