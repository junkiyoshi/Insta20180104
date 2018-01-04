#include "ofApp.h"

//--------------------------------------------------------------
ofApp::~ofApp() {
	for (int i = this->particles.size() - 1; i > -1; i--) {
		for (int j = this->particles[i].size() - 1; j > -1; j--) {
			delete this->particles[i][j];
			this->particles[i].erase(this->particles[i].begin() + j);
		}
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(60);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	ofNoFill();
	ofSetLineWidth(1.5);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 130, true, false, true);

	vector<string> words;
	words.push_back("FINGER");
	words.push_back("Escape");

	ofColor body_color;
	
	for (int word_index = 0; word_index < words.size(); word_index++) {
		ofVec2f word_size = ofVec2f(this->font.stringWidth(words[word_index]), -this->font.stringHeight(words[word_index]));

		vector<ofTTFCharacter> char_paths = this->font.getStringAsPoints(words[word_index]);
		vector<Particle*> tmp_particles;

		for (int path_index = 0; path_index < char_paths.size(); path_index++) {

			vector<ofPolyline> outline = char_paths[path_index].getOutline();

			for (int outline_index = 0; outline_index < outline.size(); outline_index++) {
				outline[outline_index] = outline[outline_index].getResampledBySpacing(5);

				tmp_particles.clear();
				tmp_particles.shrink_to_fit();

				vector<ofPoint> vertices = outline[outline_index].getVertices();
				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {
					ofVec2f location = vertices[vertices_index] - word_size / 2;
					location.y = location.y + 75 * (word_index - 1);
					tmp_particles.push_back(new Particle(location, ofColor(255)));
				}

				this->particles.push_back(tmp_particles);

				ofColor moji_color;
				moji_color.setHsb(ofRandom(255), 255, 255);
				this->words_color.push_back(moji_color);
			}
		}
	}

	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofVec2f finger_point;
	float power;
	Leap::Frame frame = leap.frame();
	for (Leap::Hand hand : frame.hands()) {
		for (Leap::Finger finger : hand.fingers()) {
			if (finger.type() == Leap::Finger::TYPE_INDEX) {
				int x = ofMap(finger.tipPosition().x, -300, 300, -ofGetWidth() / 2, ofGetWidth() / 2);
				int y = ofMap(finger.tipPosition().y, 0, 300, ofGetHeight() / 2, -ofGetHeight() / 2);
				power = ofMap(finger.tipPosition().z, 100, -100, 0, 100);
				finger_point = ofVec3f(x, y);
			}
		}
	}

	this->fbo.begin();
	ofClear(0);
	ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	ofDrawCircle(finger_point, 3);

	for (int i = 0; i < this->particles.size(); i++) {
		for (int j = 0; j < this->particles[i].size(); j++) {
			float distance = finger_point.distance(this->particles[i][j]->get_location());

			if (distance <  power) {
				this->particles[i][j]->seek_r(finger_point);
				this->particles[i][j]->seek(this->particles[i][j]->get_start_location());
			}
			else {
				this->particles[i][j]->seek(this->particles[i][j]->get_start_location());
			}

			this->particles[i][j]->update();
		}
	}

	for (int i = 0; i < this->particles.size(); i++) {
		ofSetColor(this->words_color[i]);

		ofBeginShape();
		for (int j = 0; j < this->particles[i].size(); j++) {
			//this->particles[i][j]->draw();
			ofVertex(this->particles[i][j]->get_location().x, this->particles[i][j]->get_location().y);
		}
		ofEndShape(true);
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {
	this->fbo.draw(0, 0);
}

//--------------------------------------------------------------
int main() {
	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}