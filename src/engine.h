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
    ofColor colorTriangle, colorOne, colorTwo, colorPoint;
    float min, max, density, noise, prob, lineWidth, rotation, pointSize, opacityImg, opacityGrid, opacityPoints;
    float minP, maxP, densityP, noiseP, levelMsk, angleBackground;
    bool needsUpdateGrid;
    bool needsUpdatePoints;
    bool needsDrawPoints;
    bool showInput;
    bool showGrid;
    bool showPoints;
    bool showBackgroundFile;
    bool showBackground;
    bool showBackgroundColor;
    bool showTextures;
    bool saveVector;
    bool definingMaskImg, definingMaskGrid, definingMaskPoints;
    bool coloringMaskImg, coloringMaskGrid, coloringMaskPoints;
    int shapeDrawing;
    ofxDelaunay triangulation;
    ofShader shaderAlpha;
    vector<ofxEditableSVG> svgTextures;
    int numSVG;
    ofPoint centerSVG;
    ofPath pathSvgGrid, pathSvgPoints;


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
    void updatePoints();
    void drawPoints();
    void drawVectors(string path);
    void updateBackground();
    void backgroundGradient(const ofColor& start, const ofColor& end);


};

#endif // ENGINE_H
