#include "ofApp.h"

//if mouse clicks on screen create leafclass
/*
	when L is pressed..

	program creates position for leaf at coordinates where mouse is clicked

	when shift is pressed..

	program scales leaf based on (mouseReleased - mouseClick) vector



*/


//--------------------------------------------------------------
void ofApp::setup() {


	
	//  Load tree image
	//
	if (!treeImage.load("images/tree.png")) {
		cout << "Can't load image" << endl;
		ofExit();
	}

	ofSetBackgroundColor(ofColor::black);


	// some simple sliders to play with parameters (these are examples; actuall ranges/values required
	// may neeed to be adjusted
	//
	gui.setup();
	gui.add(gravityVal.setup("Gravity", 7, 0, 20));
	gui.add(turbulenceVal.setup("Turbulence Multiplier", ofVec2f(0, 0), ofVec2f(0, 0), ofVec2f(60, 3)));
	gui.add(angleRangeVal.setup("Angle Range Multiplier", 0, 0, 200));
	gui.add(damping.setup("Damping", .99, .1, 1.0));



	leaves = LeafShape();
	
	// when click mouse set position for leaf and add to array


}
	
void LeafShape::integrate() {
	float dt = 1 / ofGetFrameRate();

	setpos(pos + velocity * dt);
	acceleration += (forces * (1.0 / mass));
	velocity.set(velocity + acceleration * dt);
	velocity.set(velocity * damping);
	forces.set(0,0,0);

	rotation += angleRange/100;
	if (abs(rotation) > 120 + initRot) {
		rotation = ofClamp(rotation, -120 + initRot, 120 + initRot);
		angleDir = -angleDir;
	}
	
}

//--------------------------------------------------------------
void ofApp::update() {
	if (bStartSim) {
		for (auto& p : particles) {
			if (!p.checkCollision() && p.simOn) {
				p.damping = damping;

				p.angleRange =  angleRangeVal * p.angleDir;


				p.gravityf = gravityVal;

				p.turb.x = turbulenceVal->x;
				p.turb.y = turbulenceVal->y;
			

				p.forces.x += ofRandom(-p.turb.x, p.turb.x);
				p.forces.y += ofRandom(-p.turb.y, p.turb.y) + p.gravityf * p.mass;


				p.integrate();
			}
		
		}
	}
  	prev_mousepos = mousepos;
	mousepos = glm::vec3(ofGetMouseX(),ofGetMouseY(),0);

	
	// modify scale and rotate val
}

//--------------------------------------------------------------
void ofApp::draw(){

	// draw background image;
	//
	ofSetColor(ofColor::white);
	treeImage.draw(0, 0);

	// draw the GUI
	if (!bHide) gui.draw();

	//draw particles
	if (mousePressed1) {
		leaves.draw();
	}

	for (auto& p : particles) {
		p.draw();
	}

	
	// Simulation On/Off Status
	//
	string str, state;
	state = (bStartSim ? "ON" : "OFF");
	str += "Simulation : " + state;
	ofSetColor(ofColor::black);
	ofDrawBitmapString(str, ofGetWindowWidth() - 170, 15);

}



//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch (key) {
	case 'a':		// simulate all leaves at once
		for (int i = 0; i < particles.size(); i++) {
			particles.at(i).simOn = true;
		}
		break;
	case 'r':					// clear (delete) all leaves
		break;
	case 'h':					//  show/hide GUI
		bHide = !bHide;
		break;
	case 'l':	// create a new leave at mouse location
		mousePressed1 = true;
		leaves.pos =mousepos;
		break;
	case OF_KEY_CONTROL:		// ctrl-left-mouse drag rotates the leaf
		bCtrlKeyDown = true;
		break;
	case OF_KEY_SHIFT:			// shift-left-mouse drag scales the leaf
		bShiftKeyDown = true;
		break;
	case ' ':					// space bar toggles simulation mode
	{
		bStartSim = !bStartSim;
		break;
	}
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	switch (key) {
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_LEFT_SHIFT:
		bShiftKeyDown = false;
		break;
	case 'l':	// create a new leave at mouse location
		particles.push_back(leaves);
		mousePressed1 = false;
		break;
	case 'r':
		particles.clear();


	default:
		break;
	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (!bStartSim) {
		if (bShiftKeyDown) {
			int i;

			for (i = 0; i < particles.size(); i++) {
				if (particles.at(i).inside(mousepos)) {
					dragging = i;
					break;
				}
			}
			dragging = (dragging < i) ? dragging : i;

			if (dragging < particles.size()) {
				particles.at(dragging).initRot += (x - prev_mousepos.x) / 10;
				particles.at(dragging).rotation += (x - prev_mousepos.x) / 10;

			}
		}
		else if (bCtrlKeyDown) {
			int i;

			for (i = 0; i < particles.size(); i++) {
				if (particles.at(i).inside(mousepos)) {
					dragging = i;
					break;
				}
			}
			dragging = (dragging < i) ? dragging : i;


			if (dragging < particles.size()) {
				particles.at(dragging).scalev += (x - prev_mousepos.x) / 500;

			}
		}
		else {
			int i;

			for (i = 0; i < particles.size(); i++) {
				if (particles.at(i).inside(mousepos)) {
					dragging = i;
					break;
				}
			}
			dragging = (dragging < i) ? dragging : i;


			if (dragging < particles.size()) {
				particles.at(dragging).pos = mousepos;

			}

			leaves.pos = mousepos;

		}
	}
	
}
	
//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if (bStartSim) {
		for (int i = 0; i < particles.size(); i++) {
			if (particles.at(i).inside(mousepos)) {
				particles.at(i).simOn = true;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//leaves.setPos(mousepos);
	dragging = particles.size();		


}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


