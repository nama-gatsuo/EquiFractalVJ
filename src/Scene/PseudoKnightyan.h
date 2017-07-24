#pragma once

#include "SceneBase.h"

class PseudoKnightyan : public SceneBase {
public:
	void setup() {
		shader.load("shader/passThru.vert", "shader/PseudoKnightyan.frag");
		resize(ofGetWidth(), ofGetHeight());

		string names[2] = {
			"k_factor", "offset"
		};
		ofVec3f range[2] = {
			ofVec3f(0.70968, 0.6, 0.95),
			ofVec3f(0.92784, 0.75, 1.15)
		};

		panel.setup();
		for (int i = 0; i < 2; i++) {
			ofParameter<float> p;
			p.set(names[i], range[i].x, range[i].y, range[i].z);
			params.push_back(p);
			panel.add(params[i]);

			SmoothValue sv;
			sv.set(range[i].x);
			autoParams.push_back(sv);
		}

		csize.set("C_size", ofVec3f(0.63248, 0.78632, 0.775), ofVec3f(0.5), ofVec3f(1.));
		panel.add(csize);
		autoCsize.set(ofPoint(0.));

		panel.add(isAuto.setup("auto", false));

	};
	void update(float dt) {

		if (isAuto) {
			for (int i = 0; i < autoParams.size(); i++) {
				autoParams[i].update();
				params[i].set(autoParams[i].get());
			}

			autoCsize.update();
			csize.set(autoCsize);
		}

	};
	void render(ofCamera& cam, ofMesh& rect) {

		shader.begin();
		shader.setUniform3f("cp", cam.getPosition());

		shader.setUniform1f("kFactor", params[0].get());
		shader.setUniform1f("offset", params[1].get());
		shader.setUniform3f("cSize", csize);

		rect.draw();
		shader.end();

		panel.draw();

	};
	void randomize() {
		for (int i = 0; i < autoParams.size(); i++) {
			autoParams[i].to(ofRandom(params[i].getMin(), params[i].getMax()));
		}
		
		autoCsize.toRandom(csize.getMin(), csize.getMax());
	};

private:

	ofParameter<ofVec3f> csize;
	SmoothPoint autoCsize;

};