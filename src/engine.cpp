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
    numSVG = 0;
	svgSize = 1.0;
    centerSVG = ofPoint(0,0);
    opacityImg = 100;
    opacityGrid = 100;
    opacityPoints = 100;
    minP = 0;
    maxP = 255;
    densityP = 10;
    noiseP = 0;
    levelMsk = 100;
    angleBackground = 0;


}

void engine::setup()
{

    colorOne = ofColor(0);
    colorTwo = ofColor(255);
    colorTriangle = ofColor(255);
    colorPoint = ofColor(0);
	colorSVG = ofColor(255);
	colorMaskPoint = NULL;

    setResolution(width, height);
    needsCalculateGrid = true;
	needsUpdateGrid= true;
	needsCalculatePoints = true;
    needsUpdatePoints = true;
	needsUpdateMask = false;
    showInput = true;
    showGrid = true;
    showPoints = true;
    showBackground = false;
    showBackgroundFile = false;
    showBackgroundColor = true;
    showTextures = false;
    saveVector = false;
    shapeDrawing = 1;
    updateBackground();
    shaderAlpha.load("shadersGL2/shaderAlpha");
	pathInput.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	pathGrid.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
	pathPoints.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

	definingSvgCenter = false;

    svgTextures.clear();
    
    //some path,  be absolute or relative to bin/data
    string path = "\Textures";
    ofDirectory dir(path);
    //only show png files
    dir.allowExt("svg");
    //populate the directory object
    dir.listDir();
    
    //go through and print out all the paths
    for(int i = dir.size() - 1; i >= 0; i--){
        ofxEditableSVG file;
		file.load(dir.getPath(i));
        svgTextures.push_back(file);
    }
    
    

}

void engine::updateBackground()
{

    background.begin();
    ofClear(0);
    if (showBackground) backgroundGradient(colorOne, colorTwo);
    if (showBackgroundColor) ofBackground(colorOne);
    if (showBackgroundFile) backgroundInput.draw(0,0);
    background.end();

}


void engine::update()
{

    if (needsCalculateGrid) calculateGrid();
    if (needsCalculatePoints) calculatePoints();
	if (needsUpdateGrid) updateGrid();
    if (needsUpdatePoints) updatePoints();
	if (needsUpdateMask) updateMask();

    canvas.begin();
    ofClear(ofColor(0, 0));

    if (showBackground || showBackgroundFile || showBackgroundColor) background.draw(0,0);

    //float ratioInput = input.getWidth()/input.getHeight();
    //float ratioCanvas = canvas.getWidth()/canvas.getHeight();
    

    if ((showInput) && (input.isAllocated())) {
		
		if (maskInput.isAllocated()) {

			shaderAlpha.begin();
			shaderAlpha.setUniformTexture("maskTex", maskInput.getTexture(), 1);
			shaderAlpha.setUniform1f("opacityImg", opacityImg / 100.0);
			shaderAlpha.setUniform1i("mode", 1);
			input.draw(0, 0);
			shaderAlpha.end();
		}
            
		else {

			shaderAlpha.begin();
			shaderAlpha.setUniform1f("opacityImg", opacityImg / 100.0);
			shaderAlpha.setUniform1i("mode", 0);
			input.draw(0, 0);
			shaderAlpha.end();

		}


    }
    
    grid.draw(0,0);
	points.draw(0, 0);

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

		ofPushMatrix();
		svgTextures[numSVG].setColorEngine(colorSVG);
		ofTranslate(centerSVG.x, centerSVG.y);
		ofScale(svgSize, svgSize);
		svgTextures[numSVG].draw();
		ofPopMatrix();
        
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

    canvas.allocate(width, height, GL_RGBA);
    grid.allocate(width, height, GL_RGBA);
	points.allocate(width, height, GL_RGBA);
    background.allocate(width, height, GL_RGBA);

    fboInput.allocate(width,height,GL_RGBA);
    fboGrid.allocate(width,height,GL_RGBA);
    fboPoints.allocate(width,height,GL_RGBA);


    if (input.isAllocated()) {

        input.clone(origInput);
		input.setImageType(OF_IMAGE_COLOR_ALPHA);

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
		backgroundInput.setImageType(OF_IMAGE_COLOR);

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
		maskInput.setImageType(OF_IMAGE_COLOR);

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
		maskGrid.setImageType(OF_IMAGE_COLOR);

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
		maskPoints.setImageType(OF_IMAGE_COLOR);

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
	input.allocate(origInput.getWidth(), origInput.getHeight(), OF_IMAGE_COLOR_ALPHA);	
    setResolution(input.getWidth(), input.getHeight());

}

void engine::setMask(string file_)
{
    origMaskInput.load(file_);
	maskInput.allocate(width, height, OF_IMAGE_COLOR);
    setResolution(width, height);
    // input.getTexture().setAlphaMask(maskInput.getTexture());
}

void engine::setMaskGrid(string file_)
{
    origMaskGrid.load(file_);
    maskGrid.allocate(width, height, OF_IMAGE_COLOR);
    setResolution(width, height);
}

void engine::setMaskPoints(string file_)
{
    origMaskPoints.load(file_);
    maskPoints.allocate(width, height, OF_IMAGE_COLOR);
    setResolution(width, height);
}

void engine::setShape(string file_)
{

	shapefile.load(file_);

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
	colorMaskPoint = NULL;
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
    backgroundInput.allocate(width, height, OF_IMAGE_COLOR);
    setResolution(width, height);

}

void engine::calculateGrid()
{

    triangulation.reset(colorTriangle);

    for (int i = 0; i < input.getWidth(); i+=density) {

        for(int j = 0; j < input.getHeight(); j += density) {

            ofPoint punto(i + ofRandom(-noise, noise), j + ofRandom(-noise, noise));
            if ((punto.x < 0) || (punto.x > input.getWidth())) punto.x = i;
            if ((punto.y < 0) || (punto.y > input.getHeight())) punto.y = j;

            ofColor colorPoint = input.getColor(punto.x, punto.y);
            float lightnessPoint = input.getColor(punto.x, punto.y).getBrightness();

            float gridMask;

            if (maskGrid.isAllocated()) gridMask = maskGrid.getColor(punto.x, punto.y).getLightness();
            else gridMask = 255;

            if ((lightnessPoint > min) && (lightnessPoint < max))
            {
				if  ( gridMask > 1) triangulation.addPoint(punto);
            }
        }
    }

    triangulation.triangulate();
	triangulation.addColor(colorTriangle, opacityGrid, input, maskGrid);

    needsCalculateGrid = false;

}

void engine::calculatePoints()
{

    triangles.clear();
    
    for (int i = 0; i < input.getWidth(); i+=densityP) {
        
        for(int j = 0; j < input.getHeight(); j += densityP) {
            
            ofPoint punto(i + ofRandom(-noiseP, noiseP), j + ofRandom(-noiseP, noiseP));
            if ((punto.x < 0) || (punto.x > input.getWidth())) punto.x = i;
            if ((punto.y < 0) || (punto.y > input.getHeight())) punto.y = j;
            
            ofColor colorPoint = input.getColor(punto.x, punto.y);
            float lightnessPoint = input.getColor(punto.x, punto.y).getBrightness();
            
            float pointMask;
            
            if (maskPoints.isAllocated()) pointMask = maskPoints.getColor(punto.x, punto.y).getLightness();
            else pointMask = 255;
            
            if ((lightnessPoint > minP) && (lightnessPoint < maxP))
            {
                // if (ofRandom(255) < lightnessPoint)
                {
                    if  ( pointMask > 1) triangles.push_back(punto);
                }
            }
        }
    }
        
    needsCalculatePoints = false;
    
}

void engine::updateMask() {

	if ((colorMaskPoint != NULL) && (input.isAllocated())) {

		ofPixels pixels = origInput.getPixels();

		for (int pos = 0; pos < pixels.size(); pos = pos+3) {

			ofColor colorPoint;
			colorPoint.r= pixels[pos];
			colorPoint.g = pixels[pos + 1];
			colorPoint.b = pixels[pos + 2];


			float distanciaColor = sqrt(pow(colorMaskPoint.r - colorPoint.r, 2) + pow(colorMaskPoint.g - colorPoint.g, 2) + pow(colorMaskPoint.b - colorPoint.b, 2)) / (255.0 * 3.0);

			if (distanciaColor <= levelMsk / 100.0) {

				pixels[pos] = ofColor(0).r;
				pixels[pos + 1] = ofColor(0).g;
				pixels[pos + 2] = ofColor(0).b;
			}

		}


		maskInput.setFromPixels(pixels);
		maskInput = blur(maskInput, 5);

		// input.getTexture().setAlphaMask(maskInput.getTexture());


	}

	needsUpdateMask = false;
}

void engine::updateGrid() {
	grid.begin();
	ofClear(ofColor(0, 0));
	ofFill();
	// ofSetLineWidth(lineWidth);

	if (showGrid) {
        
        triangulation.setColor(colorTriangle, opacityGrid, input, maskGrid);
		triangulation.drawThickness(lineWidth);

	}

	// Dibujamos puntos
	ofFill();
	grid.end();
	needsUpdateGrid = false;

}
void engine::updatePoints() {

    points.begin();
    ofClear(ofColor(0, 0));

    pathSvgPoints.clear();

    if (showPoints) {
        for (int i = 0; i < triangles.size(); i++ ) {

            ofPoint centro = ofPoint(triangles[i].x, triangles[i].y);
            ofColor colorCentro;
            colorCentro.set(colorPoint);

            colorCentro.a = opacityPoints / 100 * 255;
            
            float radioLevel = ofMap(colorCentro.a, 0, 255, 0, pointSize);
            
            if (maskPoints.isAllocated()) colorCentro.a = colorCentro.a * maskPoints.getColor(centro.x, centro.y).getLightness() / 255;

            ofSetColor(colorCentro);


            if (shapeDrawing == 1) {

                float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);
                ofDrawEllipse(centro, radio, radio);
                ofPath ellipse;
                ellipse.ellipse(centro.x, centro.y, radio, radio);
                pathSvgPoints.append(ellipse);
                
            }

            else if (shapeDrawing == 2) {

                float lado = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);
                float angle = ofRandom(0, PI);
                ofDrawRectangle(centro.x - lado/2, centro.y - lado/2, lado, lado);
                ofPath rectangle;
                rectangle.rectangle(centro.x - lado/2, centro.y - lado/2, lado, lado);
                pathSvgPoints.append(rectangle);
            }

            else if (shapeDrawing == 3) {

                float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);
                float angle = 0; //ofRandom(0, PI);
                ofPoint punto1(centro.x + radio * cos(angle), centro.y + radio * sin(angle));
                ofPoint punto2(centro.x + radio * cos(angle + 2*PI/3), centro.y + radio * sin(angle + 2*PI/3));
                ofPoint punto3(centro.x + radio * cos(angle + 4*PI/3), centro.y + radio * sin(angle + 4*PI/3));
                ofDrawTriangle(punto1, punto2, punto3);
                ofPath triangle;
                triangle.triangle(punto1, punto2, punto3);
                pathSvgPoints.append(triangle);

            }

			else if (shapeDrawing == 4) {

				float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);
				float angle = 0; //ofRandom(0, PI);
				shapefile.setSize(radio, radio);
				shapefile.draw();

				//ofPath triangle;
				//triangle.triangle(punto1, punto2, punto3);
				//pathSvgPoints.append(triangle);

			}

            pathSvgPoints.close();
        
        }

    }


    ofNoFill();

    points.end();

    needsUpdatePoints = false;

}

void engine::drawVectors(string path) {
  
    
	// Read input image and apply filters in pix2
    ofFbo fboSvgInput;
    fboSvgInput.allocate(canvas.getWidth(), canvas.getHeight(), GL_RGBA);

    fboSvgInput.begin();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    ofClear(ofFloatColor(0, 0, 0, 0));
 
	// Read background info and apply it
	if (showBackground || showBackgroundFile || showBackgroundColor) {

		ofImage bk;
		ofPixels pix;

		bk.allocate(canvas.getWidth(), canvas.getHeight(), OF_IMAGE_COLOR_ALPHA);
		pix.allocate(canvas.getWidth(), canvas.getHeight(), OF_IMAGE_COLOR_ALPHA);


		background.readToPixels(pix);
		bk.setFromPixels(pix);
		bk.draw(0, 0);

	}

	if ((showInput) && (input.isAllocated())) {

		if (maskInput.isAllocated()) {

			shaderAlpha.begin();
			shaderAlpha.setUniformTexture("maskTex", maskInput.getTexture(), 1);
			shaderAlpha.setUniform1f("opacityImg", opacityImg / 100.0);
			shaderAlpha.setUniform1i("mode", 1);
			input.draw(0, 0);
			shaderAlpha.end();
		}

		else {

			shaderAlpha.begin();
			shaderAlpha.setUniform1f("opacityImg", opacityImg / 100.0);
			shaderAlpha.setUniform1i("mode", 0);
			input.draw(0, 0);
			shaderAlpha.end();

		}


	}
 
	glDisable(GL_BLEND);
	glPopAttrib();

    fboSvgInput.end();
   
    ofImage vectorInput;
    ofPixels pix2;

	vectorInput.allocate(canvas.getWidth(), canvas.getHeight(), OF_IMAGE_COLOR_ALPHA);
	pix2.allocate(canvas.getWidth(), canvas.getHeight(), OF_IMAGE_COLOR_ALPHA);

    fboSvgInput.readToPixels(pix2);
    vectorInput.setFromPixels(pix2);
	vectorInput.saveImage("output.png");

	// Output vector file definition
	ofCairoRenderer file;
	ofRectangle viewport;

	viewport.set(0, 0, canvas.getWidth(), canvas.getHeight()); // pdf dimensions
	ofViewport(viewport);

	file.setup(path, ofCairoRenderer::FROM_FILE_EXTENSION);
	file.viewport(viewport);
	file.setBlendMode(OF_BLENDMODE_ALPHA);

	file.background(ofFloatColor(0, 0, 0, 0));
	file.setFillMode(OF_FILLED);

	// Apply input & background information
	file.draw(vectorInput, 0, 0, 0, canvas.getWidth(), canvas.getHeight(), 0, 0, canvas.getWidth(), canvas.getHeight() );
    

	// Apply grid
    if (showGrid) {
    
        file.setColor(colorTriangle);
        file.setLineWidth(lineWidth);
        
        file.draw(triangulation.triangleMesh, OF_MESH_WIREFRAME , true, false, false);
        
        
    }
    
	// Apply points grid
    if (showPoints) {
        for (int i = 0; i < triangles.size(); i++ ) {
            
            ofPoint centro = ofPoint(triangles[i].x, triangles[i].y);
            ofColor colorCentro;
            colorCentro.set(colorPoint);
            
            colorCentro.a = opacityPoints / 100 * 255;
            
            float radioLevel = ofMap(colorCentro.a, 0, 255, 0, pointSize);
 
            
            if (maskPoints.isAllocated()) colorCentro.a = colorCentro.a * maskPoints.getColor(centro.x, centro.y).getLightness() / 255;
            
            file.setColor(colorCentro);
            
            
            if (shapeDrawing == 1) {
                
                float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);

                file.drawCircle(centro.x, centro.y, 0, radio/2);
                
            }
            
            else if (shapeDrawing == 2) {
                
                float lado = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);
                float angle = ofRandom(0, PI);
                
                file.drawRectangle(centro.x, centro.y, 0, lado, lado);
                
            }
            
            else if (shapeDrawing == 3) {
                
                float radio = ofMap(input.getColor(centro.x, centro.y).getLightness(), 0, 255, radioLevel, 0);
                float angle = 0; //ofRandom(0, PI);
                ofPoint punto1(centro.x + radio * cos(angle), centro.y + radio * sin(angle));
                ofPoint punto2(centro.x + radio * cos(angle + 2*PI/3), centro.y + radio * sin(angle + 2*PI/3));
                ofPoint punto3(centro.x + radio * cos(angle + 4*PI/3), centro.y + radio * sin(angle + 4*PI/3));
                file.drawTriangle(punto1.x, punto1.y, 0, punto2.x, punto2.y, 0, punto3.x, punto3.y, 0);
                
                
            }
            
            
        }
        
    }

	// Apply SVG Textures
	if (showTextures) {

		file.setColor(colorSVG);

		file.pushMatrix();
		file.translate(centerSVG.x, centerSVG.y);
		file.scale(svgSize, svgSize);

		int n = svgTextures[numSVG].getNumPath();

		for (int i = 0; i < n; i++) {

			file.draw(svgTextures[numSVG].getPathAt(i));
		}


		file.popMatrix();

	}

	// Close file
    file.close();

	// Set normal viewport
    ofViewport(ofRectangle(0,0,ofGetWidth(),ofGetHeight()));
    
    
}


void engine::backgroundGradient(const ofColor& start, const ofColor& end) {
    float w = background.getWidth(), h = background.getHeight();
    float angle = ofMap(angleBackground, 0, 180, 0, 2);
    ofMesh gradientMesh;
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

    ofColor a, b, c, d;
    
    if (angle <= 1) {
        a = start;
        a.lerp(end, angle);
        
        b = start;
        b.lerp(start, angle);
        
        c = end;
        c.lerp(start, angle);
        
        d = end;
        d.lerp(end, angle);
    }
    
    else {
        a = end;
        a.lerp(end, angle -1);
        
        b = start;
        b.lerp(end, angle -1);
        
        c = start;
        c.lerp(start, angle - 1);
        
        d = end;
        d.lerp(start, angle - 1);
    }
    
    gradientMesh.addVertex({0.f, 0.f, 0.f});
    gradientMesh.addVertex({w, 0.f, 0.f});
    gradientMesh.addVertex({w, h, 0.f});
    gradientMesh.addVertex({0.f, h, 0.f});
    gradientMesh.addColor(a);
    gradientMesh.addColor(b);
    gradientMesh.addColor(c);
    gradientMesh.addColor(d);

    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if(depthMaskEnabled){
        glDepthMask(GL_TRUE);
    }
}

ofImage engine::blur(ofImage img, int radio) {

	ofImage imgReturn;

	ofxCv::copy(img, imgReturn);
	ofxCv::blur(imgReturn, radio);
	imgReturn.update();

	return imgReturn;

}
