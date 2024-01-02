#pragma once

#include "ofMain.h"
#include "ofxGUI.h"

class Shape {
public:
	Shape() {}
	virtual void draw() {}
	virtual bool inside(glm::vec3 p) {
		return true;
	}
	vector<glm::vec3> verts;

	glm::mat4 getMatrix() {
		glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::mat4 scale = glm::scale(glm::mat4(1.0), scalev );
		return (trans * rot *scale );
	}
	glm::vec3 pos = glm::vec3(0, 0, 0);
	float rotation = 0.0;    // degrees 
	glm::vec3 scalev = glm::vec3(.5, .5, .5);

	void setpos(glm::vec3 p) {
		pos = p;
	}
};

class LeafShape : public Shape {
public:
	// Constructor(s) go here; this is a default one; you may want to add some additional constructor(s) to make
	// it convenient to create new leaves as specific location on the screen.
	//

	ofImage img;


	LeafShape() {
		img.load("images/leaf.png");
		simOn = false;
		int w = img.getWidth();
		int h = img.getHeight();
		velocity.set(0, 0, 0);
		acceleration.set(0, 0, 0);
		forces.set(0, 0, 0);
		lifespan = 5;
		birthtime = 0;
		radius = .1;
		damping = .99;
		mass = 1;
		turb.x = 0;
		turb.y = 0;
		angleRange = 0;
		initRot = 0;
		angleDir = ofSign(ofRandom(-1,1));
		verts.push_back(glm::vec3(-w/2, -h/2, 0));
		verts.push_back(glm::vec3(-w/2, h/2, 0));
		verts.push_back(glm::vec3(w/2, h/2, 0));
		verts.push_back(glm::vec3(w/2, -h/2, 0));

	}

	// Some function(s) you might need to develop.  Note: if functions are more than 4-5 lines long,
	// then suggest moving their implementation to the .cpp file.
	//

	void draw() {

		int w = img.getWidth();
		int h = img.getHeight();

		ofPushMatrix();
		ofMultMatrix(getMatrix());

		if (inside(glm::vec3(ofGetMouseX(), ofGetMouseY(),0))) {
			ofSetColor(ofColor::black);
			ofNoFill();
			ofDrawRectangle(glm::vec3(-w / 2,-h / 2, 0), w, h);
			ofSetColor(ofColor::white);
		}




		//this->img.load("images/leaf.png");
		img.draw(- w/ 2, -h/ 2);


		ofPopMatrix();
	};

	bool inside(glm::vec3 p) {
		
		glm::vec4 vec(p, 1);
		glm::mat4 inversepoints = inverse(getMatrix());
		glm::vec4 z = inversepoints * vec;
		glm::vec3 finalpoint = glm::vec3(z);

		if (inside(finalpoint, verts)) {
			return true;
		}

		return false;
	}

	bool inside(glm::vec3 p, vector<glm::vec3> vecs) {

		glm::vec3 v1 = glm::normalize(vecs.at(0) - p);
		glm::vec3 v2 = glm::normalize(vecs.at(1) - p);
		glm::vec3 v3 = glm::normalize(vecs.at(2) - p);
		glm::vec3 v4 = glm::normalize(vecs.at(3) - p);

		float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
		float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
		float a3 = glm::orientedAngle(v3, v4, glm::vec3(0, 0, 1));
		float a4 = glm::orientedAngle(v4, v1, glm::vec3(0, 0, 1));

		if (a1 < 0 && a2 < 0 && a3 < 0 && a4 < 0)
			return true;
		else
			return false;

	}


	bool checkCollision() {
		bool onGround = false;
		if (this->pos.x >= ofGetWindowWidth()) {
			this->pos.x = ofGetWindowWidth();
		}
		if (this->pos.x <= 0) {
			this->pos.x = 0;
		}

		if (this->pos.y >= ofGetWindowHeight()) {
			this->pos.y = ofGetWindowHeight();
			onGround = true;
		}

		if (this->pos.y <= 0) {
			this->pos.y = 0;
		}
		return onGround;
	}
	void setVelocity(const ofVec3f& p) {
		velocity = p;
	}
	void integrate();

	// Physics data goes here...
	//
	//ofVec3f mousepos = glm::vec3(ofGetMouseX(), ofGetMouseY(), 0);
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float gravityf;
	ofVec2f turb;

	bool simOn;

	float angleRange;
	float angleDir;
	float initRot;
	// General Parameters go here...
	//
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
	float   birthtime;

};
class GravityForce : public LeafShape {
	ofVec3f gravity;
public:
	GravityForce(const ofVec3f& gravity);
	void set(const ofVec3f& g);
	void updateForce(LeafShape*);
};

class TurbulenceForce : public LeafShape {
	ofVec3f tmin, tmax;
public:
	TurbulenceForce(const ofVec3f& min, const ofVec3f& max);
	TurbulenceForce();
	void set(const ofVec3f& min, const ofVec3f& max);
	void updateForce(LeafShape*);
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		float framerate;
		vector<LeafShape> particles;

		// state variables
		//
		bool bCtrlKeyDown = false;
		bool bShiftKeyDown = false;
		bool bStartSim = false;

		// GUI
		//

		bool bHide = true;
		ofxVec3Slider velocityslide;
		ofxFloatSlider gravityVal;
		ofxFloatSlider damping;
		ofxVec2Slider turbulenceVal;
		ofxFloatSlider angleRangeVal;
		ofxPanel gui;
		
		//leaves
		LeafShape leaves;
		//mousepos vec
		ofVec3f mousepos = glm::vec3(ofGetMouseX(), ofGetMouseY(), 0);
		ofVec3f prev_mousepos = mousepos;
		// images
		//
		ofImage treeImage;

		//booleans
		bool mousePressed1 = ofGetMousePressed();
		int dragging;

};

