#include "engine.h"

engine::engine()
{

    width = 1024;
    height = 768;
    density = 10;
    noise = 0;
    prob = 0.5;
    lineWidth = 1;
    pointSize = 3;
    min = 0;
    max = 255;


}

void engine::setup()
{

    colorOne = ofColor(0);
    colorTwo = ofColor(255);
    colorTriangle = ofColor(255);

    setResolution(width, height);
    needsUpdateGrid = true;
    needsUpdatePoints = true;
    showInput = true;
    showBackground = false;
    showBackgroundFile = false;
    showTextures = false;
    shapeDrawing = 1;
    updateBackground();
    shaderAlpha.load("shadersGL2/shaderAlpha");

    svgTextures.clear();

    //some path, may be absolute or relative to bin/data
    string path = "Textures/";
    ofDirectory dir(path);
    //only show png files
    dir.allowExt("svg");
    //populate the directory object
    dir.listDir();

    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        ofxSVG file;
        file.load(dir.getPath(i));
        svgTextures.push_back(file);
    }



}

void engine::updateBackground()
{

    background.begin();
    ofClear(0);
    if (showBackground) backgroundGradient(colorOne, colorTwo);
    else if (showBackgroundFile) backgroundInput.draw(0,0);
    background.end();

}

void engine::update()
{

    if (needsUpdateGrid) updateGrid();
    if (needsUpdatePoints) drawPoints();

    canvas.begin();
    ofClear(ofFloatColor(0));

    if (showBackground || showBackgroundFile) background.draw(0,0);

    float ratioInput = input.getWidth()/input.getHeight();
    float ratioCanvas = canvas.getWidth()/canvas.getHeight();

    if ((showInput) && (input.isAllocated())) {

        if (maskInput.isAllocated()) {

            shaderAlpha.begin();
            shaderAlpha.setUniformTexture("imageMask", maskInput.getTextureReference(), 1);
            input.draw(0,0);
            shaderAlpha.end();

        }

        else input.draw(0,0);

    }

    grid.draw(0,0);

    if (definingMaskImg) {

        ofSetColor(255,0,0);


        if (pathInput.getNumVertices() > 0) {

            ofEllipse(pathInput.getVertex(0), 10, 10);

            for (int i = 1; i < pathInput.getNumVertices(); i++) {

                ofEllipse(pathInput.getVertex(i), 2, 2);
                ofLine(pathInput.getVertex(i-1), pathInput.getVertex(i));

            }
        }


        ofSetColor(255);
        pathInput.draw();

    }

    if (definingMaskGrid) {

        ofSetColor(0, 255, 0);


        if (pathGrid.getNumVertices() > 0) {

            ofEllipse(pathGrid.getVertex(0), 10, 10);

            for (int i = 1; i < pathGrid.getNumVertices(); i++) {

                ofEllipse(pathGrid.getVertex(i), 2, 2);
                ofLine(pathGrid.getVertex(i-1), pathGrid.getVertex(i));

            }
        }


        ofSetColor(255);

        pathGrid.draw();

    }

    if (definingMaskPoints) {

        ofSetColor(0,0,255);


        if (pathPoints.getNumVertices() > 0) {

            ofEllipse(pathPoints.getVertex(0), 10, 10);

            for (int i = 1; i < pathPoints.getNumVertices(); i++) {

                ofEllipse(pathPoints.getVertex(i), 2, 2);
                ofLine(pathPoints.getVertex(i-1), pathPoints.getVertex(i));

            }
        }


        ofSetColor(255);
        pathPoints.draw();

    }

    if (showTextures) {

        for (int i = 0; i < 1; i++) {

            svgTextures[0].draw();

        }
    }

    canvas.end();

}

void engine::draw(int x, int y, int w, int h)
{

    canvas.draw(x, y, w, h);

}

void engine::setResolution(int width_, int height_)
{
    width = width_;
    height = height_;

    canvas.allocate(width, height);
    grid.allocate(width, height);
    background.allocate(width, height);

    fboInput.allocate(width,height,GL_RGB);
    fboGrid.allocate(width,height,GL_RGB);
    fboPoints.allocate(width,height,GL_RGB);

    if (input.isAllocated()) {

        input.clone(origInput);
        float ratioInput = float(input.getWidth())/input.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioInput) {

            input.resize(width, width / ratioInput);
        }

        else {

            input.resize(height * ratioInput, height);


        }
    }

    if (backgroundInput.isAllocated()) {

        backgroundInput.clone(origBackgroundInput);

        float ratioBackground = float(backgroundInput.getWidth())/backgroundInput.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioBackground) {

            backgroundInput.resize(width, width / ratioBackground);
        }

        else {

            backgroundInput.resize(height * ratioBackground, height);


        }

    }

    if (maskInput.isAllocated()) {

        maskInput.clone(origMaskInput);

        float ratioMask = float(maskInput.getWidth())/maskInput.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioMask) {

            maskInput.resize(width, width / ratioMask);
        }

        else {

            maskInput.resize(height * ratioMask, height);


        }

    }

    if (maskGrid.isAllocated()) {

        maskGrid.clone(origMaskGrid);

        float ratioMask = float(maskGrid.getWidth())/maskGrid.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioMask) {

            maskGrid.resize(width, width / ratioMask);
        }

        else {

            maskGrid.resize(height * ratioMask, height);


        }

    }

    if (maskPoints.isAllocated()) {

        maskPoints.clone(origMaskPoints);

        float ratioMask = float(maskPoints.getWidth())/maskPoints.getHeight();
        float ratioCanvas = float(width)/height;

        if (ratioCanvas >= ratioMask) {

            maskPoints.resize(width, width / ratioMask);
        }

        else {

            maskPoints.resize(height * ratioMask, height);


        }

    }

}

void engine::setInput(string file_)
{
    origInput.load(file_);
    input.clone(origInput);
    setResolution(input.getWidth(), input.getHeight());

}

void engine::setMask(string file_)
{
    origMaskInput.load(file_);
    maskInput.clone(origMaskInput);
    setResolution(width, height);
    input.getTexture().setAlphaMask(maskInput.getTexture());
}

void engine::setMaskGrid(string file_)
{
    origMaskGrid.load(file_);
    maskGrid.clone(origMaskGrid);
    setResolution(width, height);
}

void engine::setMaskPoints(string file_)
{
    origMaskPoints.load(file_);
    maskPoints.clone(origMaskPoints);
    setResolution(width, height);
}

void engine::deleteImg()
{

    origInput.clear();
    input.clear();

}

void engine::deleteMask()
{
    origMaskInput.clear();
    maskInput.clear();
    pathInput.clear();
}

void engine::deleteMaskGrid()
{
    origMaskGrid.clear();
    maskGrid.clear();
}

void engine::deleteMaskPoints()
{
    origMaskPoints.clear();
    maskPoints.clear();
}

void engine::setBackground(string file)
{
    origBackgroundInput.load(file);
    backgroundInput.clone(origBackgroundInput);
    setResolution(width, height);

}

void engine::updateGrid()
{

    triangles.clear();
    triangulation.reset(input, colorTriangle);

    for (int i = 0; i < input.getWidth(); i+=density) {

        for(int j = 0; j < input.getHeight(); j += density) {

            ofPoint punto(i + ofRandom(-noise, noise), j + ofRandom(-noise, noise));
            if ((punto.x < 0) || (punto.x > input.getWidth())) punto.x = i;
            if ((punto.y < 0) || (punto.y > input.getHeight())) punto.y = j;

            ofColor colorPoint = input.getColor(punto.x, punto.y);
            float lightnessPoint = input.getColor(punto.x, punto.y).getBrightness();

            float pointMask, gridMask;

            if (maskGrid.isAllocated()) gridMask = maskGrid.getColor(punto.x, punto.y).getLightness();
            else gridMask = 255;

            if (maskPoints.isAllocated()) pointMask = maskPoints.getColor(punto.x, punto.y).getLightness();
            else pointMask = 255;

            if ((lightnessPoint > min) && (lightnessPoint < max))
            {
                    // cout << "Low Lightness: " << low.getLightness() << endl;
                    if (ofRandom(255) < lightnessPoint)
                    {
                        if  ( pointMask > 100) triangles.push_back(punto);
                        if  ( gridMask > 100) triangulation.addPoint(punto);
                    }
            }
        }
    }

    triangulation.triangulate();

    needsUpdateGrid = false;

}


void engine::drawPoints() {

    grid.begin();
    ofClear(ofColor(0, 0));

    triangulation.setHue(colorTriangle);

    // Dibujamos los triángulos

    ofNoFill();
    ofSetLineWidth(lineWidth);

    if (lineWidth > 0) triangulation.draw();

    // Dibujamos puntos
    ofFill();

    for (int i = 0; i < triangles.size(); i++ ) {

        ofPoint centro = ofPoint(triangles[i].x, triangles[i].y);
        ofColor colorCentro;
        colorCentro.set(colorTriangle);

        colorCentro.a = input.getColor(centro.x, centro.y).getLightness();

        ofSetColor(colorCentro);


        if (shapeDrawing == 1) {

            float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, 0, pointSize);
            ofDrawEllipse(centro, radio, radio);

        }

        else if (shapeDrawing == 2) {

            float lado = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, 0, pointSize);
            float angle = ofRandom(0, PI);
            ofDrawRectangle(centro.x - lado/2, centro.y - lado/2, lado, lado);
        }

        else if (shapeDrawing == 3) {

            float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, 0, pointSize);
            float angle = 0; //ofRandom(0, PI);
            ofPoint punto1(centro.x + radio * cos(angle), centro.y + radio * sin(angle));
            ofPoint punto2(centro.x + radio * cos(angle + 2*PI/3), centro.y + radio * sin(angle + 2*PI/3));
            ofPoint punto3(centro.x + radio * cos(angle + 4*PI/3), centro.y + radio * sin(angle + 4*PI/3));
            ofDrawTriangle(punto1, punto2, punto3);

        }

    }


    ofNoFill();

    grid.end();

    needsUpdatePoints = false;

}


void engine::backgroundGradient(const ofColor& start, const ofColor& end) {
    float w = background.getWidth(), h = background.getHeight();
    ofMesh gradientMesh;
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);


    gradientMesh.addVertex({0.f, 0.f, 0.f});
    gradientMesh.addVertex({w, 0.f, 0.f});
    gradientMesh.addVertex({w, h, 0.f});
    gradientMesh.addVertex({0.f, h, 0.f});
    gradientMesh.addColor(start);
    gradientMesh.addColor(start);
    gradientMesh.addColor(end);
    gradientMesh.addColor(end);

    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if(depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}
