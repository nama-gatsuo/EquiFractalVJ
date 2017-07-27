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
	//client.begin("", 1111);
	//client.connect("oF-EquiFracal", "try", "try");

	ofAddListener(client.onOnline, this, &ofApp::onOnline);
	ofAddListener(client.onOffline, this, &ofApp::onOffline);
	ofAddListener(client.onMessage, this, &ofApp::onMessage);

	receiver.setup(7401);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);

		string address = m.getAddress();
		vector<string> dirs = ofSplitString(address, "/");

		if (dirs[1] == "p") {
			int i = ofToInt(dirs[2]);
			float val = m.getArgAsInt(0) / 128.;
			if (i == 4) {
				dt = 0.1 + val * 3.0;
			} 
		} else if (dirs[1] == "bang") {
			int i = ofToInt(dirs[2]);

			if (i == 0) {
				cout << "bang0" << endl;
			} else if (i == 1) {
				cout << "bang1" << endl;
			}

		} else if (dirs[1] == "key") {
			int num = m.getArgAsInt(0);
			
			if (num == 28) {
				mode--;
				if (mode == -1) mode = scenes.size()-1;
			} else if (num == 29) {
				mode++;
				if (mode == scenes.size()) mode = 0;
			} else if (num == 32) {
				scenes[mode]->randomize();
			}
		}
	}

	scenes[mode]->update(dt);
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
	else if (key == OF_KEY_RIGHT) {
		mode++;
		if (mode == scenes.size()) mode = 0;
	}
	else if (key == OF_KEY_LEFT) {
		mode--;
		if (mode == -1) mode = scenes.size() - 1;
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
