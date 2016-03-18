#include "ofApp.h"
// <videoPath>rodarte-fw13-480p.mp4</videoPath>

//--------------------------------------------------------------
void ofApp::setup(){

  ofSetFrameRate(60);
  ofBackground(255*.15);
  ofSetVerticalSync(true);
  ofEnableSmoothing();

  initTimeline();
	timeline.addCurves("length", ofRange(0, 150));
	timeline.addCurves("speed", ofRange(0, 200));
	timeline.addColors("color");

  initRings();
  initChase();
  opcClient.setup("127.0.0.1", 7890);

  timeline.play();
}

//--------------------------------------------------------------
void ofApp::initTimeline(){
  ofxTimeline::removeCocoaMenusFromGlut("nike-chasing-light");
  timeline.setup();
  timeline.setFrameRate(30);

  timeline.enableSnapToOtherKeyframes(false);
  timeline.setLoopType(OF_LOOP_NORMAL);

  ofAddListener(timeline.events().bangFired, this, &ofApp::bangFired);

  timeline.setOffset(ofVec2f(0,300));
  timeline.setMinimalHeaders(true);
  timeline.setShowTimeControls(true);
  timeline.setCurrentPage(0);
}

//--------------------------------------------------------------
void ofApp::initChase(){
  chase.setLeds(ringLow1.leds);
  chase.setStartIndex(0);
  chase.setEndIndex(64);
  chase.setChaseLength(20);
  chase.setSpeed(10);
  chase.setColor(ofColor(100));
}


//--------------------------------------------------------------
void ofApp::bangFired(ofxTLBangEventArgs& args){
  cout << "bang fired!" << args.flag << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
  chase.setChaseLength(timeline.getValue("length"));
  chase.setSpeed(timeline.getValue("speed"));
  chase.setColor(timeline.getColor("color"));

  ringLow1.black();
  ringLow2.black();
  ringLow3.black();
  ringLow4.black();
  ringLow5.black();
  ringLow6.black();

  chase.update();

  updateOPC();
}

//--------------------------------------------------------------
void ofApp::updateOPC(){


  // If the client is not connected do not try and send information
  if (!opcClient.isConnected())
  {
      // Will continue to try and reconnect to the Pixel Server
      opcClient.tryConnecting();
  }
  else
  {
      opcClient.writeAllChannels(ringLow1.leds);
  }
  opcClient.update();
}

//--------------------------------------------------------------
void ofApp::initRings(){
  ofVec2f previewPos = ofVec2f(50, 50);

  // zone 1
  float size = 700;
  float x = previewPos.x + 50; // Offset Value for grabber
  float y = previewPos.y + 50; // Offset Value for grabber
  float radius = 90;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    ringLow1.leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  
  // zone 2
  radius = 80;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    ringLow2.leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // zone 3
  radius = 70;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    ringLow3.leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // zone 4
  radius = 60;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    ringLow4.leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // zone 5
  radius = 50;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    ringLow5.leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // zone 6
  radius = 40;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    ringLow6.leds.push_back(Led(ofVec2f(rx,ry), i));
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(255*.15);
  timeline.draw();	
  drawRings();
}

//--------------------------------------------------------------
void ofApp::drawRings(){

  ringLow1.draw();
  ringLow2.draw();
  ringLow3.draw();
  ringLow4.draw();
  ringLow5.draw();
  ringLow6.draw();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == 'm'){
  }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
