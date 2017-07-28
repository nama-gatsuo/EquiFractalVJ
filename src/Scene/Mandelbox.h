#pragma once
#include "SceneBase.h"

class Mandelbox : public SceneBase {
public:
	void setup() {
		shader.load("shader/passThru.vert", "shader/Mandelbox.frag");
		resize(ofGetWidth(), ofGetHeight());

		string names[4] = {
			"min_radius_2", "fixed_radius_2", "folding_limit", "scale" 
		};
		ofVec3f range[4] = {
			ofVec3f(0.1, 0.01, 2.0),
			ofVec3f(0.9, 1., 3.5),
			ofVec3f(0.9, 0.5, 2.0),
			ofVec3f(-3.12, -3.2, -2.5)
		};

		panel.setup();
		for (int i = 0; i < 4; i++) {
			ofParameter<float> p;
			p.set(names[i], range[i].x, range[i].y, range[i].z);
			params.push_back(p);
			panel.add(params[i]);

			SmoothValue sv;
			sv.set(range[i].x);
			sv.setSpeed(0.01);
			autoParams.push_back(sv);
		}

		offset.set("offset", ofVec3f(0.), ofVec3f(-5.), ofVec3f(5.));
		panel.add(offset);
		autoOffset.set(ofPoint(0.));
		autoOffset.setSpeed(0.01);

		rep.set("reptition", 0., 0.0, 8.0);

		panel.add(isAuto.setup("auto", true));

	};
	void update(float dt) {
		
		if (isAuto) {
			for (int i = 0; i < autoParams.size(); i++) {
				autoParams[i].update(dt);
				params[i].set(autoParams[i].get());
			}
			autoOffset.update(dt);
			offset.set(autoOffset);
		}

	};
	void render(ofCamera& cam, ofMesh& rect) {
		
		shader.begin();
		shader.setUniform3f("cp", cam.getPosition());
		shader.setUniform3f("uCol", color);
		
		shader.setUniform1f("minRadius2", params[0].get());
		shader.setUniform1f("fixedRadius2", params[1].get());
		shader.setUniform1f("foldingLimit", params[2].get());
		shader.setUniform1f("scale", params[3].get());
		shader.setUniform3f("offset", offset.get());
		shader.setUniform1f("rep", rep.get());

		rect.draw();
		shader.end();

		if (isShowPanel) panel.draw();

	};
	void randomize() {
		for (int i = 0; i < autoParams.size(); i++) {
			autoParams[i].to(ofRandom(params[i].getMin(), params[i].getMax()));
		}
		autoOffset.toRandom(offset.getMin(), offset.getMax());
	};

private:
	
	ofParameter<ofVec3f> offset;
	SmoothPoint autoOffset;

	ofParameter<float> rep;
};