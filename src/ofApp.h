#pragma once

#include "ofMain.h"
#include "scene/scenebase.h"
#include "scene/mandelbox.h"
#include "scene/Tgrad.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void windowResized(int w, int h);

private:
	ofEasyCam cam;
	ofVboMesh plane;
	ofFbo target;
	
	vector<shared_ptr<SceneBase>> scenes;

};
