#pragma once

#include "ofMain.h"
//#include "pingPongBuffer.h"

//--------------------------------------------------------------
#ifndef GPUParticles_pingPongBuffer_h
#define GPUParticles_pingPongBuffer_h

// examples/gl/gpuParticleSystemExampleを元に、複数バッファ書き出しに対応

struct pingPongBuffer {
public:
    void allocate( int _width, int _height, int _internalformat = GL_RGBA, int _numColorBuffers = 1){
        // Allocate
        ofFbo::Settings fboSettings;
        fboSettings.width  = _width;
        fboSettings.height = _height;
        fboSettings.numColorbuffers = _numColorBuffers;
        fboSettings.useDepth = false;
        fboSettings.internalformat = _internalformat;    // Gotta store the data as floats, they won't be clamped to 0..1
        fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
        fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
        fboSettings.minFilter = GL_NEAREST; // No interpolation, that would mess up data reads later!
        fboSettings.maxFilter = GL_NEAREST;
        
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(fboSettings );
        }
        
        // Clean
        clear();
        
        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }
    
    void swap(){
        src = &(FBOs[(flag)%2]);
        dst = &(FBOs[++(flag)%2]);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    
    ofFbo   *src;       // Source       ->  Ping
    ofFbo   *dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s
    int     flag;       // Integer for making a quick swap
};

#endif

//--------------------------------------------------------------
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
    
    ofVboMesh particles;
    
    ofShader render;
    ofShader updatePos;
    
    ofEasyCam cam;
    
    pingPongBuffer pingPong;
    
    ofVec3f emitterPos;
    ofVec3f prevEmitterPos;
    
    int particleNum;
    int texRes;
    
    bool showTex;
		
};
