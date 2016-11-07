#ifndef ENGINE_H
#define ENGINE_H

#include "ofMain.h"
#include "ofxDelaunay.h"
#include "ofxEditableSVG.h"

class engine
{
public:
    engine();
    void setup();
    void update();
    void draw(int x, int y, int w, int h);

    ofImage input, origInput;
    ofImage maskInput, origMaskInput;
    ofImage maskGrid, origMaskGrid;
    ofImage maskPoints, origMaskPoints;
    ofImage backgroundInput, origBackgroundInput;
    ofMesh pathInput, pathGrid, pathPoints;
    ofFbo fboInput, fboGrid, fboPoints;
    ofFbo canvas;
    ofFbo grid;
    ofFbo background;
    vector<ofPoint> triangles;
    int width, height;
    ofColor colorTriangle, colorOne, colorTwo;
    float min, max, density, noise, prob, lineWidth, rotation, pointSize;
    bool needsUpdateGrid;
    bool needsUpdatePoints;
    bool showInput;
    bool showBackgroundFile;
    bool showBackground;
    bool showTextures;
    bool definingMaskImg, definingMaskGrid, definingMaskPoints;
    bool coloringMaskImg, coloringMaskGrid, coloringMaskPoints;
    int shapeDrawing;
    ofxDelaunay triangulation;
    ofShader shaderAlpha;
    vector<ofxEditableSVG> svgTextures;
    int numSVG;


    void setResolution(int width_, int height_);
    void setInput(string file);
    void setMask(string file);
    void setMaskGrid(string file);
    void setMaskPoints(string file);
    void deleteImg();
    void deleteMask();
    void deleteMaskGrid();
    void deleteMaskPoints();
    void setBackground(string file);
    void updateGrid();
    void drawPoints();
    void updateBackground();
    void backgroundGradient(const ofColor& start, const ofColor& end);


};

#endif // ENGINE_H
