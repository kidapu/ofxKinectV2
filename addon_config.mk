meta:
	ADDON_NAME = ofxKinectV2
	ADDON_DESCRIPTION = An addon for the new Kinect For Windows V2 sensor
	ADDON_AUTHOR = Theo Watson
	ADDON_TAGS = "kinect" "kinectv2" "libfreenect" "libfreenect2"
	ADDON_URL = https://github.com/ofTheo/ofxKinectV2

osx:
	ADDON_FRAMEWORKS = OpenCL

linux64:
    ADDON_PKG_CONFIG_LIBRARIES = libusb-1.0
    ADDON_LDFLAGS = -Llibfreenect2
    ADDON_LDFLAGS += -lOpenCL
