#include "ofApp.h"
// <videoPath>rodarte-fw13-480p.mp4</videoPath>

//--------------------------------------------------------------
void ofApp::setup(){

  ofSetFrameRate(60);
  ofBackground(255*.15);
  ofSetVerticalSync(true);
  ofEnableSmoothing();

  initRings();
  initChases();

  initTimeline();
  initCurves();

  opcClient.setup("127.0.0.1", 7890, 10);

  timeline.play();
}

//--------------------------------------------------------------
void ofApp::initCurves(){
  timeline.setPageName("bottom");
  for (int i = 0; i < 6; i++){
    timeline.addCurves("length_" + ofToString(i), ofRange(0, chases[i].getLength()*100));
    timeline.addCurves("speed_" + ofToString(i), ofRange(0, chases[i].getLength()*10));
    timeline.addColors("color_" + ofToString(i));
    if (i == 3 || i == 4){
      timeline.addLFO("breath_" + ofToString(i));
    }

  }
  timeline.addPage("last ring chases");
  timeline.setPageName("last ring chases");
 int moreChases = 8;
  for (int i = 6; i < 6 + moreChases; i++){
    timeline.addCurves("length_" + ofToString(i), ofRange(0, 100));
    timeline.addCurves("speed_" + ofToString(i), ofRange(0, 100));
    timeline.addColors("color_" + ofToString(i));

  }
  timeline.setCurrentPage(0);
}

//--------------------------------------------------------------
void ofApp::initTimeline(){
  ofxTimeline::removeCocoaMenusFromGlut("nike-chasing-light");
  timeline.setup();
  timeline.setFrameRate(30);
  timeline.setDurationInSeconds(5*60);

  timeline.enableSnapToOtherKeyframes(false);
  timeline.setLoopType(OF_LOOP_NORMAL);

  ofAddListener(timeline.events().bangFired, this, &ofApp::bangFired);

  timeline.setOffset(ofVec2f(0,300));
  timeline.setMinimalHeaders(true);
  timeline.setShowTimeControls(true);
}

//--------------------------------------------------------------
void ofApp::initChases(){
  chases.resize(14);
  for (int i = 0; i < 6; i++){
    int offset = 0;
    int length = rings[i].leds.size();
    chases[i].setLeds(rings[i].leds);
    chases[i].setStartIndex(offset);
    chases[i].setEndIndex(offset+length);
    chases[i].setChaseLength(20);
    chases[i].setSpeed(10);
    chases[i].setColor(ofColor(100));
    chases[i].setTimeline(timeline);
  }
  chases[3].setLinked(14);
  chases[0].setClockwise(-1);

  // add many small chases for the last ring
   int moreChases = 8;
  for (int i = 6; i < 6 + moreChases; i++){
    int offset = i * rings[5].leds.size()/moreChases;
    int length =  rings[5].leds.size()/moreChases;
    chases[i].setLeds(rings[5].leds);
    chases[i].setStartIndex(offset);
    chases[i].setEndIndex(offset+length);
    chases[i].setChaseLength(20);
    chases[i].setSpeed(10);
    chases[i].setColor(ofColor(100));
    chases[i].setTimeline(timeline);
  }
}


//--------------------------------------------------------------
void ofApp::bangFired(ofxTLBangEventArgs& args){
  cout << "bang fired!" << args.flag << endl;
}

//--------------------------------------------------------------
void ofApp::update(){
  for (int i = 0; i < rings.size(); i++){
    rings[i].black();
  }

  for (int i = 0; i < chases.size(); i++){
    chases[i].setChaseLength(timeline.getValue("length_" + ofToString(i)));
    chases[i].setSpeed(timeline.getValue("speed_" + ofToString(i)));
    chases[i].setColor(timeline.getColor("color_" + ofToString(i)));

    chases[i].update();
    if (i == 3 || i == 4){
      chases[i].setBrightness(timeline.getValue("breath_" + ofToString(i)));
    }
  }

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
      opcClient.writeAll(rings[0].leds);
      opcClient.writeAll(rings[1].leds, 0, 512*2);
  }
  opcClient.update();
}

//--------------------------------------------------------------
void ofApp::initRings(){
  rings.resize(6);
	if( xmlSettings.loadFile("settings.xml") ){
		ofLogNotice("settings.xml loaded!");
	}else{
		ofLogNotice("unable to load settings.xml check data/ folder");
	}
  ofVec2f previewPos = ofVec2f(50, 50);

  // zone 1
  float size = xmlSettings.getValue("leds:ringLow1:count", 700);
  float x = previewPos.x + 50; // Offset Value for grabber
  float y = previewPos.y + 50; // Offset Value for grabber
  float radius = 90;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    rings[0].leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  
  // zone 2
  size = xmlSettings.getValue("leds:ringLow2:count", 700);
  radius = 80;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    rings[1].leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // zone 3
  size = xmlSettings.getValue("leds:ringLow3:count", 700);
  radius = 70;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    rings[2].leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // zone 4
  size = xmlSettings.getValue("leds:ringLow4:count", 700);
  radius = 60;
  int linked = 14;
  for (int i = 0; i < size/linked; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size/linked);

    for (int j = 0; j < linked; j++)
    {
      int radius_ = radius - j;
      // Generate the position of the grabber points
      float rx = x + (radius_ * cos(angle));
      float ry = y + (radius_ * sin(angle));
      rings[3].leds.push_back(Led(ofVec2f(rx,ry), i+j));
    }
  

  }

  // zone 5
  size = xmlSettings.getValue("leds:ringLow5:count", 700);
  radius = 30;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    rings[4].leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // zone 6
  size = xmlSettings.getValue("leds:ringLow6:count", 700);
  radius = 20;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    rings[5].leds.push_back(Led(ofVec2f(rx,ry), i));
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
  for (int i = 0; i < 6; i++){
    rings[i].draw();
  }
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
