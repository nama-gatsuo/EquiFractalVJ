#pragma once

#include "ofMain.h"
#include "scene/scenebase.h"
#include "scene/mandelbox.h"
#include "scene/Tgrad.h"
#include "scene/PseudoKnightyan.h"
#include "Scene/Hartverdrahtet.h"

#include "ofxOsc.h"
#include "ofxMQTT.h"

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

	ofxMQTT client;

	ofxOscReceiver receiver;

};
