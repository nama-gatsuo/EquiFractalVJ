#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	font.loadFont("font/OpenSans-Light.ttf", 12, true, true);

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

	pe.setup();
	cam.setDistance(5.);
	
	// MQTT
	mqtt.begin("acid.iamas.ac.jp", 1883);
	mqtt.connect("ofx2", "senstick", "SenStick");

	ofAddListener(mqtt.onOnline, this, &ofApp::onOnline);
	ofAddListener(mqtt.onOffline, this, &ofApp::onOffline);
	ofAddListener(mqtt.onMessage, this, &ofApp::onMessage);

	// OSC
	receiver.setup(7401);
}

//--------------------------------------------------------------
void ofApp::update(){
	
	mqtt.update();

	while (receiver.hasWaitingMessages()) {
		ofxOscMessage m;
		receiver.getNextMessage(m);

		string address = m.getAddress();
		vector<string> dirs = ofSplitString(address, "/");

		if (dirs[1] == "p") {
			int i = ofToInt(dirs[2]);
			float val = m.getArgAsInt(0) / 128.;
			if (i == 3) {
				pe.setOpacity(val);
			}
			else if (i == 4) {
				dt = 0.1 + val * 3.0;
			} 
		} else if (dirs[1] == "bang") {
			int i = ofToInt(dirs[2]);

			if (i == 0) {
				float coin = ofRandom(1.);
				if (coin < 0.1) mode = floor(ofRandom(scenes.size()));

				scenes[mode]->randomize();
			} else if (i == 1) {
				float coin = ofRandom(1.);
				if (coin < 0.4) pe.setMode(0);
				else if (coin < 0.7) pe.setMode(1);
				else pe.setMode(2);
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
	pe.begin();
	scenes[mode]->render(cam, plane);
	pe.end();

	pe.draw();

	ofSetColor(0);
	ofDrawRectangle(0, ofGetHeight() - 50,200,ofGetHeight()-50);
	ofSetColor(255);
	//font.drawStringAsShapes(ofToString(timeStr), 10, 10, ofGetHeight() - 15);
	font.drawStringAsShapes("FPS: " + ofToString(ofGetFrameRate()), 10, ofGetHeight() - 15);
	font.drawStringAsShapes("FPS: " + ofToString(ofGetFrameRate()), 10, ofGetHeight());
	
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
	else if (key == 's') {
		for (int i = 0; i < scenes.size(); i++) {
			scenes[i]->togglePanel();
		}
	}
	else if (key == 'q') {
		pe.setMode(0);
	}
	else if (key == 'w') {
		pe.setMode(1);
	}
	else if (key == 'e') {
		pe.setMode(2);
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

	pe.resize(w, h);
}

void ofApp::exit() {
	mqtt.disconnect();
}

void ofApp::onOnline() {
	cout << "online" << endl;
	mqtt.subscribe("senstick");
}

void ofApp::onOffline() {
	ofLog() << "offline";
}

void ofApp::onMessage(ofxMQTTMessage &msg) {
	
	json.parse(msg.payload);
	string dstr = json["date"].asString();
	//cout << json << endl;
	
	timeStr = dstr;
	
	ofxJSONElement::Members mem = json.getMemberNames();

	for (int i = 0; i < mem.size(); i++) {
		string str = mem[i];

		if (str == "AccX") {
			accel = ofVec3f(
				json[mem[i]].asFloat(),
				json[mem[i + 1]].asFloat(),
				json[mem[i + 2]].asFloat()
			);

			accel.x = abs(accel.x) * 0.5;
			accel.y = abs(accel.y) * 0.5;
			accel.z = abs(accel.z) * 0.5;
			scenes[mode]->setColor(accel);

			cout << accel << endl;

		} else if (str == "GyroX") {
			gyro = ofVec3f(
				json[mem[i]].asFloat(),
				json[mem[i + 1]].asFloat(),
				json[mem[i + 2]].asFloat()
			);
		} else if (str == "Magnetic") {
			magnetic = json[mem[i]].asFloat();
		} else if (str == "UV") {
			uv = json[mem[i]].asFloat();
		} else if (str == "AirPressure") {
			air = json[mem[i]].asFloat();
		} else if (str == "Humidity") {
			humidity = json[mem[i]].asFloat();
		} else if (str == "Brightness") {
			brigtness = json[mem[i]].asFloat();
		}
		break;
	}
}
