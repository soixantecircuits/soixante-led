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

  outputRectangle.set(500, 25, 250*16/9., 250);
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
  chase.setLeds(leds);
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
//update views
  if(!loaded){
    contentRectangle = ofRectangle(0,0, 16, 9); 
  }
  chase.setChaseLength(timeline.getValue("length"));
  chase.setSpeed(timeline.getValue("speed"));
  chase.setColor(timeline.getColor("color"));

  for (int i = 0; i < leds.size(); i++) {
    leds[i].c = ofColor::black;
  }
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
      opcClient.writeAllChannels(leds);
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

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  
  // zone 2
  radius = 80;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // zone 3
  radius = 70;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // zone 4
  radius = 60;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // zone 5
  radius = 50;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // zone 5
  radius = 40;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  for (int i = 0; i < leds.size(); i++)
  {
      leds[i].c = ofColor::black;
  }
  

}

//--------------------------------------------------------------
void ofApp::drawRings()
{
    // dessine les cercles
    for (int i = 0; i < leds.size(); i++)
    {
        ofFill();
        ofSetColor(leds[i].c);
        ofDrawCircle(leds[i].pos.x, leds[i].pos.y, 4);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
  ofBackground(255*.15);
  if(loaded){
    ofSetColor(255);
    timeline.getVideoPlayer("Video")->draw(outputRectangle);        
  }
  // Where to draw the ring!

  /*
    ofPushMatrix();
    float x = 50; // Offset Value for grabber
    float y = 50; // Offset Value for grabber
    ofTranslate(x, y);
    ofFill();
    ofSetColor(100);
    ofRect(-50,-50,100,100);
    ledRing();
    ofPopMatrix();
    */
  timeline.draw();	
  drawRings();
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
