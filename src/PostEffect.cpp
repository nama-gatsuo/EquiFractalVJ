#include "PostEffect.h"

void PostEffect::setup() {

	ofFbo::Settings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.internalformat = GL_RGB32F;

	base.allocate(s);

	composite.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);

	passThru.load("shader/passThru.vert", "shader/pfx/passThru.frag");
	greyConv.load("shader/passThru.vert", "shader/pfx/GreyConv.frag");
}

void PostEffect::begin() {

	base.begin();
	ofClear(0, 0);

}
void PostEffect::end() {

	base.end();

}
void PostEffect::draw() {
	ofClear(0, 0);
	switch (mode) {
		case 0: {
			passThru.begin();
			passThru.setUniform1f("opacity", opacity);
			break;
		}
		case 1: {
			greyConv.begin();
			greyConv.setUniform1i("isNega", 0);
			greyConv.setUniform1f("opacity", opacity);
			break; 
		}
		case 2: {
			greyConv.begin();
			greyConv.setUniform1i("isNega", 1);
			greyConv.setUniform1f("opacity", opacity);
			break;
		}
		default: break;
	}

	base.draw(0, 0);

	switch (mode) {
		case 0: passThru.end(); break;
		case 1: greyConv.end(); break;
		case 2: greyConv.end(); break;
		default: break;
	}
}

void PostEffect::resize(int w, int h) {
	ofFbo::Settings s;
	s.width = w;
	s.height = h;
	s.internalformat = GL_RGB32F;
	base.clear();
	base.allocate(s);

	composite.clear();
	composite.allocate(w, h, GL_RGB32F);
}