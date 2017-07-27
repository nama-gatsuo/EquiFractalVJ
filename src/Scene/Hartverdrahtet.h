#pragma once
#include "SceneBase.h"

class Hartverdrahtet : public SceneBase {
public:
	void setup() {
		shader.load("shader/passThru.vert", "shader/Hartverdrahtet.frag");
		resize(ofGetWidth(), ofGetHeight());

		string names[3] = {
			"fs", "fu", "fd"
		};
		ofVec3f range[3] = {
			ofVec3f(1., 0.8, 1.2),
			ofVec3f(10., 0.75, 10.1),
			ofVec3f(0.763, 0.7, 0.8)
		};

		panel.setup();
		for (int i = 0; i < 3; i++) {
			ofParameter<float> p;
			p.set(names[i], range[i].x, range[i].y, range[i].z);
			params.push_back(p);
			panel.add(params[i]);

			SmoothValue sv;
			sv.set(range[i].x);
			autoParams.push_back(sv);
		}

		cs.set("cs", ofVec3f(.808, .808, 1.167), ofVec3f(0.5), ofVec3f(1.5, 1.5,1.1));
		panel.add(cs);
		a_cs.set(ofPoint(0.));

		fc.set("fc", ofVec3f(0.), ofVec3f(-0.4), ofVec3f(0.4));
		panel.add(fc);
		a_fc.set(ofPoint(0.));

		panel.add(isAuto.setup("auto", false));

	};
	void update(float dt) {

		if (isAuto) {
			for (int i = 0; i < autoParams.size(); i++) {
				autoParams[i].update(dt);
				params[i].set(autoParams[i].get());
			}

			a_cs.update(dt);
			cs.set(a_cs);

			/*a_fc.update();
			fc.set(a_fc);*/

		}

	};
	void render(ofCamera& cam, ofMesh& rect) {

		shader.begin();
		shader.setUniform3f("cp", cam.getPosition());

		shader.setUniform3f("cs", cs);
		shader.setUniform1f("fs", params[0].get());
		shader.setUniform3f("fc", fc);
		shader.setUniform1f("fu", params[1].get());
		shader.setUniform1f("fd", params[2].get());

		rect.draw();
		shader.end();

		panel.draw();

	};
	void randomize() {
		for (int i = 0; i < autoParams.size(); i++) {
			autoParams[i].to(ofRandom(params[i].getMin(), params[i].getMax()));
		}

		a_cs.toRandom(cs.getMin(), cs.getMax());
		/*a_fc.toRandom(fc.getMin(), fc.getMax());*/
	};

private:

	ofParameter<ofVec3f> cs;
	SmoothPoint a_cs;

	ofParameter<ofVec3f> fc;
	SmoothPoint a_fc;
};