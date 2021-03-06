//
//  ofxKinectV2.cpp
//  kinectExample
//
//  Created by Theodore Watson on 6/23/14.
//
//

#include "ofxKinectV2.h"

//--------------------------------------------------------------------------------
ofxKinectV2::ofxKinectV2(){
    bNewFrame  = false;
    bNewBuffer = false;
    bOpened    = false;
    lastFrameNo = -1;
    
    //set default distance range to 50cm - 600cm
    
    params.add(minDistance.set("minDistance", 500, 0, 12000));
    params.add(maxDistance.set("maxDistance", 6000, 0, 12000));
}

//--------------------------------------------------------------------------------
ofxKinectV2::~ofxKinectV2(){
    close();
}

//--------------------------------------------------------------------------------
static bool sortBySerialName( const ofxKinectV2::KinectDeviceInfo & A, const ofxKinectV2::KinectDeviceInfo & B ){
    return A.serial < B.serial;
}

//--------------------------------------------------------------------------------
vector <ofxKinectV2::KinectDeviceInfo> ofxKinectV2::getDeviceList(){
    vector <KinectDeviceInfo> devices;
    
    int num = protonect.getFreenect2Instance().enumerateDevices();
    for (int i = 0; i < num; i++){
        KinectDeviceInfo kdi;
        kdi.serial = protonect.getFreenect2Instance().getDeviceSerialNumber(i);
        kdi.freenectId = i; 
        devices.push_back(kdi);
    }
    
    ofSort(devices, sortBySerialName);
    for (int i = 0; i < num; i++){
        devices[i].deviceId = i;
    }
    
    return devices;
}

//--------------------------------------------------------------------------------
unsigned int ofxKinectV2::getNumDevices(){
   return getDeviceList().size(); 
}

//--------------------------------------------------------------------------------
bool ofxKinectV2::open(unsigned int deviceId){
    
    vector <KinectDeviceInfo> devices = getDeviceList();
    
    if( devices.size() == 0 ){
        ofLogError("ofxKinectV2::open") << "no devices connected!";
        return false;
    }
    
    if( deviceId >= devices.size() ){
        ofLogError("ofxKinectV2::open") << " deviceId " << deviceId << " is bigger or equal to the number of connected devices " << devices.size() << endl;
        return false;
    }

    string serial = devices[deviceId].serial;
    return open(serial);
}

//--------------------------------------------------------------------------------
bool ofxKinectV2::open(string serial){
    close(); 
    
    params.setName("kinectV2 " + serial);
    
    bNewFrame  = false;
    bNewBuffer = false;
    bOpened    = false;
    
    int retVal = protonect.openKinect(serial);
    
    if(retVal==0){
        lastFrameNo = -1;
        startThread(true);
//        pointCloudFront.resize(512*424);
//        pointCloud = pointCloudBack = pointCloudFront;
//        pointCloudColor.resize(512*424);
    }else{
        return false;
    }
  
    
    bOpened = true;
    return true;
}

//--------------------------------------------------------------------------------
void ofxKinectV2::threadedFunction(){

    while(isThreadRunning()){
        protonect.updateKinect(rgbPixelsBack, depthPixelsBack, bigdepthPixelsBack);
        rgbPixelsFront.swap(rgbPixelsBack);
        depthPixelsFront.swap(depthPixelsBack);
        bigdepthPixelsFront.swap(bigdepthPixelsBack);
      
                
        lock();
        {
          bNewBuffer = true;
        }
        unlock();
    }
}

//--------------------------------------------------------------------------------
//ofPoint ofxKinectV2::getWorldCoordinateAt(int x, int y){
//    int index = x + y * 512;
//    if( index >= 0 && index < pointCloud.size() ){
//        return pointCloud[index];
//    }
//}

////--------------------------------------------------------------------------------
//vector <ofPoint> ofxKinectV2::getWorldCoordinates(){
//    return pointCloud;
//}
//

//--------------------------------------------------------------------------------
void ofxKinectV2::update(){
    if( ofGetFrameNum() != lastFrameNo ){
        bNewFrame = false;
        lastFrameNo = ofGetFrameNum();
    }
    if( bNewBuffer ){
    
        lock();
            rgbPix = rgbPixelsFront;
            rawDepthPixels = depthPixelsFront;
            rawBigDepthPixels = bigdepthPixelsFront;
//            pointCloud = pointCloudFront;
            bNewBuffer = false;
        unlock();
//        
//        if( rawDepthPixels.size() > 0 )
//        {
//            if( depthPix.getWidth() != rawDepthPixels.getWidth() )
//            {
//                depthPix.allocate(rawDepthPixels.getWidth(), rawDepthPixels.getHeight(), 1);
//            }
//        
//            float * pixelsF         = rawDepthPixels.getData();
//            unsigned char * pixels  = depthPix.getData();
//                
//            for(int i = 0; i < depthPix.size(); i++)
//            {
//                pixels[i] = ofMap(rawDepthPixels[i], minDistance, maxDistance, 255, 0, true);
//                if( pixels[i] == 255 )
//                {
//                    pixels[i] = 0;
//                }
//            }
//
//        }
        
        
        bNewFrame = true; 
    }
}


//--------------------------------------------------------------------------------
bool ofxKinectV2::isFrameNew(){
    return bNewFrame; 
}

//--------------------------------------------------------------------------------
ofPixels ofxKinectV2::getDepthPixels(){
    return depthPix;
}

//--------------------------------------------------------------------------------
ofFloatPixels ofxKinectV2::getRawDepthPixels(){
    return rawDepthPixels;
}

//--------------------------------------------------------------------------------
ofFloatPixels ofxKinectV2::getRawBigDepthPixels(){
  return rawBigDepthPixels;
}

//--------------------------------------------------------------------------------
ofPixels ofxKinectV2::getRgbPixels(){
    return rgbPix; 
}

//--------------------------------------------------------------------------------
void ofxKinectV2::close(){
    if( bOpened ){
        waitForThread(true);
        protonect.closeKinect();
        bOpened = false;
    }
}




//--------------------------------------------------------------------------------
// Added by kidapu
//--------------------------------------------------------------------------------
float ofxKinectV2::getDistanceInDepthCoord(int x, int y)
{
    if (!rawDepthPixels.isAllocated())
    {
        return 0.0f;
    }
    return rawDepthPixels[x + y * rawDepthPixels.getWidth()]; // mm
}

float ofxKinectV2::getDistanceInRgbCoord(int x, int y)
{
  if(!rawBigDepthPixels.isAllocated())
  {
    return 0.0f;
  }
  return rawBigDepthPixels[x + (y+1) * rawBigDepthPixels.getWidth()]; //mm
}

void ofxKinectV2::getWorldCoordinateByRgb(int cx, int cy, float & x, float & y, float & z)
{
  const float badPoint = std::numeric_limits<float>::quiet_NaN();
  if(!rawBigDepthPixels.isAllocated())
  {
    x = y = z = badPoint;
    return;
  }
  float depthVal = rawBigDepthPixels[cx + (cy+1) * rawBigDepthPixels.getWidth()] / 1000.0f; // mm -> m
  if (isnan(depthVal) || depthVal <= 0.001)
  {
    x = y = z = badPoint;
    return;
  }

  libfreenect2::Freenect2Device::ColorCameraParams colorParam = protonect.getColorCameraParams();
  
  // printf("%f %f %f %f \n", colorParam.cx, colorParam.cy, colorParam.fx, colorParam.fy);
  // x,y,z
  x = (cx + 0.5 - colorParam.cx) * (1/colorParam.fx) * depthVal;
  y = (cy + 0.5 - colorParam.cy) * (1/colorParam.fy) * depthVal;
  z = depthVal;
  
}

void ofxKinectV2::getBoxInWorldCoordinate(int cx, int cy, int cw, int ch, float z, float & x, float & y, float & w, float & h)
{
    libfreenect2::Freenect2Device::ColorCameraParams colorParam = protonect.getColorCameraParams();

    float depthVal = z; 
    x = (cx + 0.5 - colorParam.cx) * (1/colorParam.fx) * depthVal;
    y = (cy + 0.5 - colorParam.cy) * (1/colorParam.fy) * depthVal;
    z = depthVal;
    w = cw * (1/colorParam.fx) * depthVal;
    h = ch * (1/colorParam.fy) * depthVal;
}

void ofxKinectV2::depthToColor(ofPoint depthPoint, ofPoint & colorPoint)
{
    float z = getDistanceInDepthCoord(depthPoint.x, depthPoint.y) ;
    protonect.apply(depthPoint.x, depthPoint.y, z, colorPoint.x, colorPoint.y);
    
    //    cout << "- - - - -  " << endl;
    //    printf("x:%f,y:%f,z:%f \n",depthPoint.x, depthPoint.y, z);
    //    printf("x:%f,y:%f \n",colorPoint.x, colorPoint.y);
}



