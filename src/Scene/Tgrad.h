#pragma once
#include "SceneBase.h"

class Tgrad : public SceneBase {
public:
	void setup() {
		shader.load("shader/passThru.vert", "shader/Tgrad.frag");
		resize(ofGetWidth(), ofGetHeight());

		string names[5] = {
			"min_r", "max_r", "min_c", "max_c", "scale"
		};
		ofVec3f range[5] = {
			ofVec3f(0.25, 0.001, 0.5),
			ofVec3f(1., 0.75, 1.25),
			ofVec3f(-0.94, -1.0, -0.8),
			ofVec3f(0.94, 0.8, 1.),
			ofVec3f(-3.12, -3.2, -3.),
		};

		panel.setup();
		for (int i = 0; i < 5; i++) {
			ofParameter<float> p;
			p.set(names[i], range[i].x, range[i].y, range[i].z);
			params.push_back(p);
			panel.add(params[i]);

			SmoothValue sv;
			sv.set(range[i].x);
			autoParams.push_back(sv);
		}

		offset.set("offset", ofVec3f(1.2, 49.0, 1.4), ofVec3f(1.), ofVec3f(50.));
		panel.add(offset);
		autoOffset.set(ofPoint(0.));

		p0.set("p0", ofVec3f(0.0, 1.59, -1.0), ofVec3f(-2.), ofVec3f(2.));
		panel.add(p0);
		autoP0.set(ofPoint(0.));

		panel.add(isAuto.setup("auto", false));

	};
	void update(float dt) {

		if (isAuto) {
			for (int i = 0; i < autoParams.size(); i++) {
				autoParams[i].update(dt);
				params[i].set(autoParams[i].get());
			}
			//autoOffset.update();
			//offset.set(autoOffset);

			autoP0.update(dt);
			p0.set(autoP0);
		}

	};
	void render(ofCamera& cam, ofMesh& rect) {

		shader.begin();
		shader.setUniform3f("cp", cam.getPosition());

		shader.setUniform1f("minr", params[0].get());
		shader.setUniform1f("maxr", params[1].get());
		shader.setUniform1f("minc", params[2].get());
		shader.setUniform1f("maxc", params[3].get());
		shader.setUniform1f("scale", params[4].get());
		shader.setUniform3f("offset", offset.get());
		shader.setUniform3f("p0", p0.get());
		rect.draw();
		shader.end();

		panel.draw();

	};
	void randomize() {
		for (int i = 0; i < autoParams.size(); i++) {
			autoParams[i].to(ofRandom(params[i].getMin(), params[i].getMax()));
		}
		//autoOffset.toRandom(offset.getMin(), offset.getMax());
		autoP0.toRandom(p0.getMin(), p0.getMax());
	};

private:

	ofParameter<ofVec3f> offset;
	SmoothPoint autoOffset;

	ofParameter<ofVec3f> p0;
	SmoothPoint autoP0;
	
};