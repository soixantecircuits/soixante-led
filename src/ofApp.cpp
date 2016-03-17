#include "ofApp.h"
// <videoPath>rodarte-fw13-480p.mp4</videoPath>

//--------------------------------------------------------------
void ofApp::setup(){

  ofSetFrameRate(60);
  ofBackground(255*.15);
  ofSetVerticalSync(true);
  ofEnableSmoothing();

  initTimeline();
#ifndef TARGET_RASPBERRY_PI
  initVideo();
#endif
  addColorTracks();
  mute = false;

  initRings();
  opcClient.setup("127.0.0.1", 7890);

  outputRectangle.set(500, 25, 250*16/9., 250);
  timeline.setCurrentTimeSeconds(10);
  timeline.play();
}

//--------------------------------------------------------------
void ofApp::initVideo(){
  loaded = false;
  if(settings.loadFile("settings.xml")){
    string videoPath = settings.getValue("videoPath", "");
    if(videoPath != ""){
      loadVideo(videoPath);
      addVideoTracksInPages(videoPath);
    }
  }
}

//--------------------------------------------------------------
void ofApp::initTimeline(){
  ofxTimeline::removeCocoaMenusFromGlut("nike-chasing-light");
  timeline.setup();
  timeline.setFrameRate(30);

  //timeline.addVideoTrack("video", "rodarte-fw13-480p.mp4");

  timeline.setPageName("Inner Circle 1");
  timeline.addPage("Outer Circle 1");
  timeline.setPageName("Outer Circle 1");
  timeline.addPage("Inner Circle 2");
  timeline.setPageName("Inner Circle 2");
  timeline.addPage("Outer Circle 2");
  timeline.setPageName("Outer Circle 2");
  timeline.addPage("Inner Circle 3");
  timeline.setPageName("Inner Circle 3");
  timeline.addPage("Outer Circle 3");
  timeline.setPageName("Outer Circle 3");

  timeline.enableSnapToOtherKeyframes(false);
  timeline.setLoopType(OF_LOOP_NORMAL);

  ofAddListener(timeline.events().bangFired, this, &ofApp::bangFired);


  timeline.setOffset(ofVec2f(0,300));
  timeline.setMinimalHeaders(true);
  timeline.setShowTimeControls(true);
  timeline.setCurrentPage(0);
}

//--------------------------------------------------------------
void ofApp::addColorTracks(){
  for (unsigned int j = 0; j < 1; j++){
    timeline.setCurrentPage(j);
    for (unsigned int i = 0; i < 16; i++){
      //timeline.addColors("color" + ofToString(j) + "_" + ofToString(i));
      timeline.addColors("color" + ofToString(j*16+i));
    }
  }
  timeline.setCurrentPage(0);
}

//--------------------------------------------------------------
void ofApp::addVideoTracksInPages(string videoPath){
  for (unsigned int j = 1; j < 2; j++){
    timeline.setCurrentPage(j);
    ofxTLVideoTrack* videoTrack = timeline.getVideoTrack("Video"+ofToString(j));

    if(videoTrack == NULL){
      timeline.addVideoTrack("Video"+ofToString(j), videoPath);
    }
  }
  timeline.setCurrentPage(0);
}

//--------------------------------------------------------------
void ofApp::loadVideo(string videoPath){
  ofxTLVideoTrack* videoTrack = timeline.getVideoTrack("Video");

  if(videoTrack == NULL){
    videoTrack = timeline.addVideoTrack("Video", videoPath);
    loaded = (videoTrack != NULL);
  }
  else{
    loaded = videoTrack->load(videoPath);
  }

  if(loaded){
    contentRectangle = ofRectangle(0,0, videoTrack->getPlayer()->getWidth(), videoTrack->getPlayer()->getHeight());

    //timeline.clear();
    //At the moment with video and audio tracks
    //ofxTimeline only works correctly if the duration of the track == the duration of the timeline
    //plan is to be able to fix this but for now...
    timeline.setFrameRate(videoTrack->getPlayer()->getTotalNumFrames()/videoTrack->getPlayer()->getDuration());
    timeline.setDurationInFrames(videoTrack->getPlayer()->getTotalNumFrames());
    timeline.setTimecontrolTrack(videoTrack); //video playback will control the time
    timeline.bringTrackToTop(videoTrack);
  }
  else{
    videoPath = "";
  }
  settings.setValue("videoPath", videoPath);
  settings.saveFile();        
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
  /*
  //calculate a the view for the movie, scaled into the center between the timeline and the buttons
  float availableHeight = ofGetHeight() - timeline.getBottomLeft().y;
  if(ofGetWidth() / availableHeight > contentRectangle.width/contentRectangle.height){
    outputRectangle.height = availableHeight;
    outputRectangle.width = contentRectangle.width * availableHeight / contentRectangle.height;
  }
  else {
    outputRectangle.width  = ofGetWidth();
    outputRectangle.height = contentRectangle.height * ofGetWidth()/contentRectangle.width;
  }
  outputRectangle.x =  ofGetWidth()/2 - outputRectangle.width/2;
  outputRectangle.y = timeline.getBottomLeft().y;
  */

  for (int i = 0; i < 16; i++) {
    leds[i].c = timeline.getColor("color"+ofToString(i));
  }

  // If the client is not connected do not try and send information
  if (!opcClient.isConnected())
  {
      // Will continue to try and reconnect to the Pixel Server
      opcClient.tryConnecting();
  }
  else
  {
      // Write out the first set of data
      vector <ofColor> colors;
      for (int i = 0; i < 64; i++) {
        colors.push_back(leds[i].c);
      }
      opcClient.writeChannelOne(colors);
      vector <ofColor> colors2;
      for (int i = 32; i < leds.size(); i++) {
        colors2.push_back(leds[i].c);
      }
      opcClient.writeChannelTwo(colors2);
      vector <ofColor> colors3;
      for (int i = 64; i < leds.size(); i++) {
        colors3.push_back(leds[i].c);
      }
      opcClient.writeChannelThree(colors3);
  }
  opcClient.update();
}

//--------------------------------------------------------------
void ofApp::initRings(){
  ofVec2f previewPos = ofVec2f(50, 50);

  // left ring, inner leds
  float size = 16;
  float x = previewPos.x + 50; // Offset Value for grabber
  float y = previewPos.y + 50; // Offset Value for grabber
  float radius = 50;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  
  // left ring, outer leds
  radius = 60;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // middle ring, inner leds
  x = x + radius + 15; // Offset Value for grabber
  y = y + radius + 30; // Offset Value for grabber
  radius = 30;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // middle ring, outer leds
  radius = 40;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }

  // right ring, inner leds
  x = previewPos.x + 250; // Offset Value for grabber
  y = previewPos.y + 50; // Offset Value for grabber
  radius = 35;
  for (int i = 0; i < size; i++)
  {
    float angle = (1.0 * i) * (2.0 * PI)/(1.0 * size);

    // Generate the position of the grabber points
    float rx = x + (radius * cos(angle));
    float ry = y + (radius * sin(angle));

    leds.push_back(Led(ofVec2f(rx,ry), i));
  }
  // right ring, outer leds
  radius = 45;
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
    if (mute){
      timeline.getVideoPlayer("Video")->setVolume(1);
      mute = false;
    } else {
      timeline.getVideoPlayer("Video")->setVolume(0);
      mute = true;
    }
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
