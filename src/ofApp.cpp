#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	shared_ptr<SceneBase> s0(new Tgrad());
	s0->setup();
	scenes.push_back(s0);

	plane.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	windowResized(ofGetWidth(), ofGetHeight());

	cam.setDistance(15.);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	scenes[0]->update(0.);
	cam.begin();
	cam.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	scenes[0]->render(cam, plane);

	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') scenes[0]->randomize();
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
	plane.clearVertices();
	plane.clearTexCoords();

	plane.addVertex(ofVec3f(0, 0, 0));
	plane.addTexCoord(ofVec2f(0, h));
	plane.addVertex(ofVec3f(0, h, 0));
	plane.addTexCoord(ofVec2f(0, 0));
	plane.addVertex(ofVec3f(w, 0, 0));
	plane.addTexCoord(ofVec2f(w, h));
	plane.addVertex(ofVec3f(w, h, 0));
	plane.addTexCoord(ofVec2f(w, 0));

	for (int i = 0; i < scenes.size(); i++) {
		scenes[i]->resize(w,h);
	}
}
