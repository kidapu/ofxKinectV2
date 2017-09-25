#pragma once

#include "ofMain.h"

// lib ( addons )
#include "ofxKinectV2.h"
#include "ofxGui.h"


class Const
{
public:
  
  // layout
  const static int WIDTH = 1200;
  const static int HEIGHT = 600;
  
  const static int DEPTH_WIDTH = 512;
  const static int DEPTH_HEIGHT = 424;
  
  const static int COLOR_WIDTH = 1920;
  const static int COLOR_HEIGHT = 1080;
  
  const static int W_MARGIN = 20;
  const static int H_MARGIN = 20;
  
  constexpr static float DEPTH_SCALE = 1.0;
  constexpr static float COLOR_SCALE = 0.3;
};


class ofApp : public ofBaseApp
{
private:
  // vars
  ofxKinectV2 * _kinect;
  ofTexture _texDepth;
  ofTexture _texRGB;
  ofTexture _texBigDepth;
  ofShader _depthShader;
  
  // vars for gui
  ofxPanel _gui;
  ofParameter <float> _pKinectMinDist;
  ofParameter <float> _pKinectMaxDist;
  
  // vars for click
  ofPoint clickPoint;
  
public:
  void setup()
  {
    // env
    ofBackground(30, 30, 30);
    
    // kinect
    ofxKinectV2 tmp;
    vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
    _kinect = new ofxKinectV2();
    _kinect->open(deviceList[0].serial);
  
    // gui
    _gui.setup("", "settings.xml", Const::W_MARGIN, Const::DEPTH_HEIGHT + Const::H_MARGIN*2);
    _gui.add( _pKinectMinDist.set("KinectMinDist", 300, 0, 6000) );
    _gui.add( _pKinectMaxDist.set("KinectMaxDist", 5000, 0, 6000) );
    _gui.loadFromFile("settings.xml");
    
    
    // shader
    setupShader();
  }
  
  void setupShader()
  {
    string depthFragmentShader = R"(
    uniform float KinectMinDist;
    uniform float KinectMaxDist;
    uniform sampler2DRect tex;
    void main()
    {
      vec4  col = texture2DRect(tex, gl_TexCoord[0].xy);
      float value = col.r;
      float low1 = KinectMinDist;
      float high1 = KinectMaxDist;
      float low2 = 1.0;
      float high2 = 0.0;
      float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
      
      if (d == 1.0) {
        d = 0.0;
      }
      gl_FragColor = vec4(vec3(d), 1.0);
    }
    )";
    _depthShader.setupShaderFromSource(GL_FRAGMENT_SHADER, depthFragmentShader);
    _depthShader.linkProgram();
    return;
  }
  
  void update()
  {
    _kinect->update();
    if( _kinect->isFrameNew() )
    {
//      _texDepth.loadData( _kinect->getRawDepthPixels() );
      _texRGB.loadData( _kinect->getRgbPixels() );
      _texBigDepth.loadData( _kinect->getRawBigDepthPixels() );
    }
  }
  
  void draw()
  {
    // env
    ofClear(100);
    ofSetColor(255);
    
    
    // RGB
    ofPushMatrix();
    {
      ofTranslate(Const::W_MARGIN, Const::H_MARGIN);
      ofScale(Const::COLOR_SCALE,Const::COLOR_SCALE);
      _texRGB.draw(0, 0, Const::COLOR_WIDTH, Const::COLOR_HEIGHT);
    }
    ofPopMatrix();
    

    // big depth
    ofPushMatrix();
    {
      ofTranslate(Const::W_MARGIN * 2 + Const::COLOR_WIDTH * Const::COLOR_SCALE, Const::H_MARGIN);
      ofScale(Const::COLOR_SCALE,Const::COLOR_SCALE);
      _depthShader.begin();
      {
        _depthShader.setUniform1f("KinectMinDist", _pKinectMinDist);
        _depthShader.setUniform1f("KinectMaxDist", _pKinectMaxDist);
        
        _texBigDepth.draw(0, 0, Const::COLOR_WIDTH, Const::COLOR_HEIGHT);
      }
      _depthShader.end();
    }
    ofPopMatrix();
    
    
    // draw click point
    if( clickPoint.x != 0 && clickPoint.y != 0 )
    {
      ofPushMatrix();
      {
        ofSetColor(255,0,0);
        ofTranslate(Const::W_MARGIN, Const::H_MARGIN);
        ofScale(Const::COLOR_SCALE,Const::COLOR_SCALE);
        ofDrawCircle(clickPoint.x, clickPoint.y, 10);
        ofSetColor(255);
      }
      ofPopMatrix();

    }
    
    
    // gui
    _gui.draw();
    
    
    // fps
    stringstream ss;
    ss << "framerate: " << ofToString(ofGetFrameRate(), 0);
    ofDrawBitmapString(ss.str(), 10, 20);
  }

  
  void mousePressed(int x, int y, int button)
  {
    clickPoint = ofPoint( int((x - Const::W_MARGIN)/Const::COLOR_SCALE), int((y - Const::H_MARGIN)/Const::COLOR_SCALE) );
    float depth = _kinect->getDistanceInRgbCoord(clickPoint.x, clickPoint.y);
    
    float wx,wy,wz;
    _kinect->getWorldCoordinateByRgb(clickPoint.x, clickPoint.y, wx, wy, wz);
    
    
//    getWorldCoordinateByRgb(int u, int v, float & x, float & y, float & z)
    
    // get rgb_to_depth clicked
    printf("- - - - - \n");
    printf("RGB: %f %f => Depth: %f \n", clickPoint.x, clickPoint.y, depth);
    printf("WORLD : %f %f %f \n", wx, wy, wz);

  }
  
//  void keyPressed(int key);
//  void keyReleased(int key);
//  void mouseMoved(int x, int y );
//  void mouseDragged(int x, int y, int button);

//  void mouseReleased(int x, int y, int button);
//  void mouseEntered(int x, int y);
//  void mouseExited(int x, int y);
//  void windowResized(int w, int h);
//  void dragEvent(ofDragInfo dragInfo);
//  void gotMessage(ofMessage msg);
		
};
