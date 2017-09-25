//
//  ofxKinectV2.h
//  kinectExample
//
//  Created by Theodore Watson on 6/23/14.
//
//

#pragma once

#include "ofProtonect.h"
#include "ofMain.h"

class ofxKinectV2 : public ofThread{

    public:
    
        struct KinectDeviceInfo{
            string serial;
            int deviceId;   //if you have the same devices plugged in device 0 will always be the same Kinect
            int freenectId; //don't use this one - this is the index given by freenect2 - but this can change based on order device is plugged in
        };

        ofxKinectV2();
        ~ofxKinectV2(); 
        
        //for some reason these can't be static - so you need to make a tmp object to query them
        vector <KinectDeviceInfo> getDeviceList();
        unsigned int getNumDevices();
    
        bool open(string serial);
        bool open(unsigned int deviceId = 0);
        void update();
        void close();
    
        bool isFrameNew();
    
    
        // Added by kidapu
        float getDistanceInDepthCoord(int x, int y);
        float getDistanceInRgbCoord(int x, int y);
        void getWorldCoordinateByRgb(int cx, int cy, float & x, float & y, float & z);
        void getBoxInWorldCoordinate(int cx, int cy, int cw, int ch, float z, float & x, float & y, float & w, float & h);
        void depthToColor(ofPoint depthPoint, ofPoint & colorPoint);

        //    void colorToDepth(ofPoint colorPoint, ofPoint & depthPoint);
    
    
//        ofPoint getWorldCoordinateAt(int x, int y);
//        vector <ofPoint> getWorldCoordinates();
//        vector <ofColor> getWorldCoordinateColor(){
//            return pointCloudColor;
//        }
  
        ofPixels getDepthPixels();
        ofPixels getRgbPixels();
        ofFloatPixels getRawDepthPixels();
        ofFloatPixels getRawBigDepthPixels();
  
        ofParameterGroup params;
        ofParameter <float> minDistance;
        ofParameter <float> maxDistance;

    protected:
        void threadedFunction();

        ofPixels rgbPix;
        ofPixels depthPix;
        ofFloatPixels rawDepthPixels;
        ofFloatPixels rawBigDepthPixels;
    
        bool bNewBuffer;
        bool bNewFrame;
        bool bOpened;
    
        ofProtonect protonect; 
    
        ofPixels rgbPixelsBack;
        ofPixels rgbPixelsFront;
        ofFloatPixels depthPixelsBack;
        ofFloatPixels depthPixelsFront;
  
        // added by kidapu
        ofFloatPixels bigdepthPixelsBack;
        ofFloatPixels bigdepthPixelsFront;
    
//        vector <ofPoint> pointCloudFront;
//        vector <ofPoint> pointCloudBack;
//        vector <ofPoint> pointCloud;
//        vector <ofColor> pointCloudColor;

        int lastFrameNo;
    
    
        // Added by kidapu
        ofFloatPixels bigDepthPixelsBack;
        libfreenect2::Registration * registration;
};
