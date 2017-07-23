#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	ofGLWindowSettings s;
    s.setGLVersion(3, 2);
    s.width = 1024;
    s.height = 768;
    ofCreateWindow(s);
	ofRunApp(new ofApp());
}
