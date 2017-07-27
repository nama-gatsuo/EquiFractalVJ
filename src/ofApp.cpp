#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	shared_ptr<SceneBase> s0(new Mandelbox());
	s0->setup();
	scenes.push_back(s0);

	shared_ptr<SceneBase> s1(new Tgrad());
	s1->setup();
	scenes.push_back(s1);

	shared_ptr<SceneBase> s2(new PseudoKnightyan());
	s2->setup();
	scenes.push_back(s2);

	shared_ptr<SceneBase> s3(new Hartverdrahtet());
	s3->setup();
	scenes.push_back(s3);

	plane.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	windowResized(ofGetWidth(), ofGetHeight());

	cam.setDistance(5.);

	// MQTT
	/*client.begin("", 1111);
	client.connect("oF-EquiFracal", "try", "try");

	ofAddListener(client.onOnline, this, &ofApp::onOnline);
	ofAddListener(client.onOffline, this, &ofApp::onOffline);
	ofAddListener(client.onMessage, this, &ofApp::onMessage);*/
}

//--------------------------------------------------------------
void ofApp::update(){
	
	scenes[mode]->update(0.);
	cam.begin();
	cam.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	scenes[mode]->render(cam, plane);

	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), 10, 40);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') scenes[mode]->randomize();
	if (key == OF_KEY_RIGHT) {
		mode++;
		if (mode == scenes.size()) mode = 0;
	}
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

void ofApp::exit() {
	client.disconnect();
}

void ofApp::onOnline() {
	ofLog() << "online";
}

void ofApp::onOffline() {
	ofLog() << "offline";
}


void ofApp::onMessage(ofxMQTTMessage &msg) {
	ofLog() << "message: " << msg.topic << " - " << msg.payload;
}
