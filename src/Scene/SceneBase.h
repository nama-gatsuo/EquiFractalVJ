#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "../CommonUtil.h"

class SceneBase {
public:
	virtual void setup() {};
	virtual void update(float dt) {};
	virtual void render(ofCamera& cam, ofMesh& rect) {};
	virtual void randomize() {};

	void setAuto(bool isAuto) { this->isAuto = isAuto; };
	void resize(int w, int h) {
		shader.begin();
		shader.setUniform2f("size", w, h);
		shader.end();
	};

	void setColor(ofVec3f& c) {
		color = c;
	};
	void togglePanel() { isShowPanel = !isShowPanel; }

	ofShader shader;
	ofxPanel panel;
	bool isShowPanel = false;

	vector<SmoothValue> autoParams;
	vector<ofParameter<float>> params;

	ofxToggle isAuto;

	ofVec3f color;
};