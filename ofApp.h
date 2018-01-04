#pragma once
#include "ofMain.h"
#include "ofFbo.h"
#include "Particle.h"

// Leap Motion SDK
#include "Leap.h"
#pragma comment(lib, "Leap.lib")

class ofApp : public ofBaseApp {

public:
	~ofApp();

	void setup();
	void update();
	void draw();

	void keyPressed(int key) {};
	void keyReleased(int key) {};
	void mouseMoved(int x, int y) {};
	void mouseDragged(int x, int y, int button) {};
	void mousePressed(int x, int y, int button) {};
	void mouseReleased(int x, int y, int button) {};
	void mouseEntered(int x, int y) {};
	void mouseExited(int x, int y) {};
	void windowResized(int w, int h) {};
	void dragEvent(ofDragInfo dragInfo) {};
	void gotMessage(ofMessage msg) {};

	ofFbo fbo;
	
	ofTrueTypeFont font;
	vector<vector<Particle*>> particles;
	vector<ofColor> words_color;

	// Leap Motion
	Leap::Controller leap;
};