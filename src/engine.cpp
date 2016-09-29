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
    showMasking = false;
    shapeDrawing = 1;
    updateBackground();

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

        ofEnableBlendMode(OF_BLENDMODE_ALPHA);

        input.draw(0,0);

    }

    if ((showInput) && (maskInput.isAllocated())) {

        ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

        if (showMasking) maskInput.draw(0, 0);


    }

    ofDisableBlendMode();

    grid.draw(0,0);

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

}

void engine::setInput(string file_)
{
    origInput.load(file_);
    input.clone(origInput);
    setResolution(input.getWidth(), input.getHeight());

}

void engine::setMask(string file_)
{
    showMasking = true;
    origMaskInput.load(file_);
    maskInput.clone(origMaskInput);
    setResolution(width, height);
}

void engine::deleteMask()
{
    showMasking = false;
    origMaskInput.clear();
    maskInput.clone(origMaskInput);
    setResolution(width, height);
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

            float maskPoint;

            if (maskInput.isAllocated()) maskPoint = maskInput.getColor(punto.x, punto.y).getLightness();
            else maskPoint = 255;

            if (!showMasking) maskPoint = 255;

            cout << "min: " << min << " max: " << max << " value: " << lightnessPoint << endl;

            if ((lightnessPoint > min) && (lightnessPoint < max))
            {
                    // cout << "Low Lightness: " << low.getLightness() << endl;
                    if ((ofRandom(255) < lightnessPoint) && ( maskPoint > 100))
                    {
                        triangles.push_back(punto);
                        triangulation.addPoint(punto);
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
