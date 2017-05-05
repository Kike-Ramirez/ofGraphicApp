#include "ofApp.h"

/*
    All components instantiated outside of a gui - 
    https://github.com/braitsch/ofxDatGui @braitsch
*/

void ofApp::setup()
{
    //ofSetFullscreen(true);
    ofSetWindowPosition(100, 100);
    ofSetWindowShape(1024, 768);
    ofSetWindowTitle("ofGraphicApp v1.10");
    ofSetFrameRate(60);
    ofSetEscapeQuitsApp(false);

    dWidth = int(ofGetWidth() / 100.0);
    dHeight = int(ofGetHeight() / 100.0);

	xmlParameters.loadFile("settings.xml");


    loadGui();
	myengine.setup();

	loadSettings();

    ofEnableSmoothing();




}

void ofApp::update()
{

    for(int i=0; i<components.size(); i++) components[i]->update();
    myengine.update();

}

void ofApp::draw()
{
    ofBackgroundGradient(ofColor(0), ofColor(255), OF_GRADIENT_LINEAR);
    fitCanvas();
    for(int i=components.size()-1; i>=0; i--) components[i]->draw();

	colorSelectorGrid.display();
	colorSelectorPoint.display();
	colorSelectorColorSVG.display();
	colorSelectorOne.display();
	colorSelectorTwo.display();



    if ((myengine.definingMaskImg) || (myengine.definingMaskGrid) || (myengine.definingMaskPoints)) {

        if ((mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {
            
            ofSetColor(255,0,0);
            ofLine(mouseX - 5, mouseY, mouseX + 5, mouseY);
            ofLine(mouseX, mouseY - 5, mouseX, mouseY + 5);
            
        }

        ofDrawBitmapString("Definiendo Mascara", xCanvas + 10, yCanvas + 20);
        ofDrawBitmapString("Cierra haciendo click en el primer punto para aplicar mascara", xCanvas + 10, yCanvas + 40);
        ofDrawBitmapString("Pulsa <Q> para salir sin aplicar", xCanvas + 10, yCanvas + 60);
        ofSetColor(255);
    }

    if (myengine.coloringMaskImg) {

        if ((mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {
            
            ofSetColor(255,0,0);
            ofLine(mouseX - 5, mouseY, mouseX + 5, mouseY);
            ofLine(mouseX, mouseY - 5, mouseX, mouseY + 5);
        
        }
        
        ofDrawBitmapString("Seleccion de Color", xCanvas + 10, yCanvas + 20);
        ofDrawBitmapString("Pulsa <A> para aceptar los cambios", xCanvas + 10, yCanvas + 40);
        ofSetColor(255);
        
    }

}

void ofApp::loadGui() {

    int x = dWidth;
    int y = dHeight;
    int p = 0;
    int x11 = 38 * dWidth;


//    component = new ofxDatGuiFRM();
//    component->setPosition(x, y);
//    component->setWidth(x11, 0.7);
//    components.push_back(component);

//    y += component->getHeight() + p + 3 * dHeight;
    component = new ofxDatGuiLabel("AREA DE TRABAJO");
    component->setPosition(x, y);
    component->setWidth(x11, 0.7);
    components.push_back(component);

    y += component->getHeight() + p;
    ancho = new ofxDatGuiTextInput("Definir Ancho", xmlParameters.getValue("settings:width", "1022"));
    ancho->setPosition(x, y);
    ancho->setWidth(x11, 0.7);
    ancho->onTextInputEvent(this, &ofApp::onTextInputEvent);
    components.push_back(ancho);

    y += ancho->getHeight() + p;
    alto = new ofxDatGuiTextInput("Definir Alto", xmlParameters.getValue("settings:height", "762"));
    alto->setPosition(x, y);
    alto->setWidth(x11, 0.7);
    alto->onTextInputEvent(this, &ofApp::onTextInputEvent);
    components.push_back(alto);

    y += alto->getHeight() + p + 4 * dHeight;
    showInput = new ofxDatGuiToggle("showInput", xmlParameters.getValue("settings:showInput", true));
	showInput->setPosition(x, y);
	showInput->setWidth(x11, 0.3);
	showInput->setLabel("MOSTRAR IMAGEN");
	showInput->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(showInput);
    
    y += showInput->getHeight() + p;
    component = new ofxDatGuiButton("uploadImg");
    component->setPosition(x, y);
    component->setWidth(x11 / 2, 0.7);
    component->setLabel("Archivo");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteImg");
    component->setPosition(x + x11 / 2, y);
    component->setWidth(x11 / 2, 0.7);
    component->setLabel("Borrar");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    opacityImg = new ofxDatGuiSlider("opacityImg", 0, 100, xmlParameters.getValue("settings:opacityImg", 100));
	opacityImg->setPosition(x, y);
	opacityImg->setWidth(x11, 0.3);
	opacityImg->setLabel("Opacidad");
	opacityImg->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(opacityImg);

    y += opacityImg->getHeight() + p;
    component = new ofxDatGuiButton("uploadMskImg");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Mascara Imagen");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("defineMskImg");
    component->setPosition(x + x11 * 0.25, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Def. Forma");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("colorMskImg");
    component->setPosition(x + x11 * 0.5, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Def. Color");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteMskImg");
    component->setPosition(x + x11 * 0.75, y);
    component->setWidth(x11 * 0.25, 0.7);
    component->setLabel("Borrar Mascara");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    y += component->getHeight() + p;
    levelMsk = new ofxDatGuiSlider("levelMsk", 0, 100, xmlParameters.getValue("settings:levelMsk", 100));
	levelMsk->setPosition(x, y);
	levelMsk->setWidth(x11, 0.3);
	levelMsk->setLabel("Umbral Masc. Color");
	levelMsk->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(levelMsk);
    
    
    y += levelMsk->getHeight() + p + 4 * dHeight;
    showGrid = new ofxDatGuiToggle("showGrid", xmlParameters.getValue("settings:showGrid", true));
	showGrid->setPosition(x, y);
	showGrid->setWidth(x11, 0.3);
	showGrid->setLabel("MOSTRAR MALLA");
	showGrid->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(showGrid);
    
    y += showGrid->getHeight() + p;
    opacityGrid = new ofxDatGuiSlider("opacityGrid", 0, 100, xmlParameters.getValue("settings:opacityGrid", 100));
	opacityGrid->setPosition(x, y);
	opacityGrid->setWidth(x11, 0.3);
	opacityGrid->setLabel("Opacidad");
	opacityGrid->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(opacityGrid);
    

    y += opacityGrid->getHeight() + p;
    min = new ofxDatGuiSlider("min", 0, 255, xmlParameters.getValue("settings:min", 0));
    min->setPosition(x, y);
	min->setWidth(x11, 0.3);
	min->setLabel("Minimo");
	min->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(min);

    y += min->getHeight() + p;
    max = new ofxDatGuiSlider("max", 0, 255, xmlParameters.getValue("settings:mask", 255));
	max->setPosition(x, y);
	max->setWidth(x11, 0.3);
	max->setLabel("Maximo");
	max->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(max);

    y += max->getHeight() + p;
    density = new ofxDatGuiSlider("density", 4, 40, xmlParameters.getValue("settings:density", 10));
	density->setPosition(x, y);
	density->setWidth(x11, 0.3);
	density->setLabel("Espaciado");
	density->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(density);

    y += density->getHeight() + p;
    noise = new ofxDatGuiSlider("noise", 0, 100, xmlParameters.getValue("settings:noise", 0));
	noise->setPosition(x, y);
	noise->setWidth(x11, 0.3);
	noise->setLabel("Desplazamiento");
	noise->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(noise);

    y += noise->getHeight() + p;
    stroke = new ofxDatGuiSlider("stroke", 0, 10, xmlParameters.getValue("settings:stroke", 1));
	stroke->setPosition(x, y);
	stroke->setWidth(x11, 0.3);
	stroke->setLabel("Grosor Linea");
	stroke->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(stroke);

    y += component->getHeight() + p;
    component = new ofxDatGuiButton("uploadMskGrid");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.4, 0.7);
    component->setLabel("Mascara Rejilla");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("defineMskGrid");
    component->setPosition(x + x11 * 0.4, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Definir Forma");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);
    
    component = new ofxDatGuiButton("deleteMskGrid");
    component->setPosition(x + x11 * 0.7, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Borrar Mascara");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

	y += component->getHeight() + p;
	colorSelectorGrid.setup(ofVec3f(x, y), ofVec3f(x11, 20));
    
    y += 20 + p;
    colorGrid = new ofxDatGuiColorPicker("colorGrid", ofColor::fromHex(xmlParameters.getValue("settings:colorGrid", 0xFFFFFF)));
	colorGrid->setPosition(x, y);
	colorGrid->setWidth(x11, 0.3);
	colorGrid->setLabel("Color");
	colorGrid->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(colorGrid);
    

    
    
    y += colorGrid->getHeight() + p + 4 * dHeight;
	showPoints = new ofxDatGuiToggle("showPoints", xmlParameters.getValue("settings:showPoints", true));
	showPoints->setPosition(x, y);
	showPoints->setWidth(x11, 0.3);
	showPoints->setLabel("MOSTRAR PUNTOS DE MALLA");
	showPoints->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(showPoints);
    
    y += showPoints->getHeight() + p;
    opacityPoints = new ofxDatGuiSlider("opacityPoints", 0, 100, xmlParameters.getValue("settings:opacityPoints", 100));
	opacityPoints->setPosition(x, y);
	opacityPoints->setWidth(x11, 0.3);
	opacityPoints->setLabel("Opacidad");
	opacityPoints->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(opacityPoints);

    y += opacityPoints->getHeight() + p;
    minP = new ofxDatGuiSlider("minP", 0, 255, xmlParameters.getValue("settings:minP", 0));
	minP->setPosition(x, y);
	minP->setWidth(x11, 0.3);
	minP->setLabel("Minimo");
	minP->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(minP);
    
    y += minP->getHeight() + p;
    maxP = new ofxDatGuiSlider("maxP", 0, 255, xmlParameters.getValue("settings:maxP", 255));
	maxP->setPosition(x, y);
	maxP->setWidth(x11, 0.3);
	maxP->setLabel("Maximo");
	maxP->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(maxP);
    
    y += maxP->getHeight() + p;
    densityP = new ofxDatGuiSlider("densityP", 4, 40, xmlParameters.getValue("settings:densityP", 10));
	densityP->setPosition(x, y);
	densityP->setWidth(x11, 0.3);
	densityP->setLabel("Espaciado");
	densityP->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(densityP);
    
    y += densityP->getHeight() + p;
    noiseP = new ofxDatGuiSlider("noiseP", 0, 100, xmlParameters.getValue("settings:noiseP", 0));
	noiseP->setPosition(x, y);
	noiseP->setWidth(x11, 0.3);
	noiseP->setLabel("Desplazamiento");
	noiseP->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(noiseP);
    
    y += noiseP->getHeight() + p;
    size = new ofxDatGuiSlider("size", 0, 100, xmlParameters.getValue("settings:size", 5));
	size->setPosition(x, y);
	size->setWidth(x11, 0.3);
	size->setLabel("Diametro");
	size->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(size);
    
    y += size->getHeight() + p;
    punto = new ofxDatGuiToggle("punto", xmlParameters.getValue("settings:punto", true));
	punto->setPosition(x, y);
	punto->setWidth(x11/4, 0.3);
	punto->setLabel("Circulo");
	punto->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(punto);

    cuadrado = new ofxDatGuiToggle("cuadrado", xmlParameters.getValue("settings:cuadrado", false));
	cuadrado->setPosition(x + x11/4, y);
	cuadrado->setWidth(x11/4, 0.3);
	cuadrado->setLabel("Cuadrado");
	cuadrado->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(cuadrado);

    triangulo = new ofxDatGuiToggle("triangulo", xmlParameters.getValue("settings:triangulo", false));
	triangulo->setPosition(x + 2 * x11/4, y);
	triangulo->setWidth(x11/4, 0.3);
	triangulo->setLabel("Triangulo");
	triangulo->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(triangulo);

    archivo = new ofxDatGuiToggle("archivo", xmlParameters.getValue("settings:archivo", false));
	archivo->setPosition(x + 3 * x11/4, y);
	archivo->setWidth(x11/4, 0.3);
	archivo->setLabel("Archivo");
	archivo->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(archivo);

    y += archivo->getHeight() + p;
    component = new ofxDatGuiButton("uploadMskPoints");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.4, 0.7);
    component->setLabel("Mascara Puntos");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("defineMskPoints");
    component->setPosition(x + x11 * 0.4, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Definir Forma");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("deleteMskPoints");
    component->setPosition(x + x11 * 0.7, y);
    component->setWidth(x11 * 0.3, 0.7);
    component->setLabel("Borrar Masc.");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

	y += component->getHeight() + p;
	colorSelectorPoint.setup(ofVec3f(x, y), ofVec3f(x11, 20));

	y += 20 + p;
    color = new ofxDatGuiColorPicker("color", ofColor::fromHex(xmlParameters.getValue("settings:color", 0x000000)));
	color->setPosition(x, y);
	color->setWidth(x11, 0.3);
	color->setLabel("Color");
	color->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(color);

    y += color->getHeight() + p + dHeight + 8*dHeight;
    graphicElements = new ofxDatGuiToggle("graphicElements", xmlParameters.getValue("settings:graficElements", false));
	graphicElements->setPosition(x, y);
	graphicElements->setWidth(x11 , 0.3);
	graphicElements->setLabel("MOSTRAR ELEMENTOS GRAFICOS");
	graphicElements->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(graphicElements);

    y += graphicElements->getHeight() + p;
    component = new ofxDatGuiButton("graphicRandom");
    component->setPosition(x, y);
    component->setWidth(x11, 1);
    component->setLabel("Generar graficos");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

	y += component->getHeight() + p;

	colorSelectorColorSVG.setup(ofVec3f(x, y), ofVec3f(x11, 20));

	y += 20 + p;

	colorSVG = new ofxDatGuiColorPicker("colorSVG", ofColor::fromHex(xmlParameters.getValue("settings:color", 0x000000)));
	colorSVG->setPosition(x, y);
	colorSVG->setWidth(x11, 0.3);
	colorSVG->setLabel("Color");
	colorSVG->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
	components.push_back(colorSVG);
    
    y += colorSVG->getHeight() + p + 8 * dHeight;
    loadBackground = new ofxDatGuiToggle("loadBackground", xmlParameters.getValue("settings:loadBackground", false));
    loadBackground->setPosition(x, y);
    loadBackground->setWidth(x11 * 0.4, 0.3);
    loadBackground->setLabel("ARCHIVO FONDO");
    loadBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(loadBackground);

    colorBackground = new ofxDatGuiToggle("colorBackground", xmlParameters.getValue("settings:colorBackground", true));
    colorBackground->setPosition(x + 0.4 * x11, y);
    colorBackground->setWidth(0.3 * x11, 0.3);
    colorBackground->setLabel("COLOR");
    colorBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(colorBackground);
    
    defineBackground = new ofxDatGuiToggle("defineBackground", xmlParameters.getValue("settings:defineBackground", false));
    defineBackground->setPosition(x + 0.7 * x11, y);
    defineBackground->setWidth(0.3 * x11, 0.3);
    defineBackground->setLabel("DEGRADADO");
    defineBackground->onToggleEvent(this, &ofApp::onToggleEvent);
    components.push_back(defineBackground);
    
    y += defineBackground->getHeight() + p;
    angleBackground = new ofxDatGuiSlider("angleBackground", 0, 180, xmlParameters.getValue("settings:angleBackground", 0));
	angleBackground->setPosition(x, y);
	angleBackground->setWidth(x11, 0.3);
	angleBackground->setLabel("Angulo Degradado");
	angleBackground->onSliderEvent(this, &ofApp::onSliderEvent);
    components.push_back(angleBackground);

	y += angleBackground->getHeight() + p;

	colorSelectorOne.setup(ofVec3f(x, y), ofVec3f(x11/2, 20));

	colorSelectorTwo.setup(ofVec3f(x + x11 / 2, y), ofVec3f(x11/2, 20));

	y += 20 + p;

    colorOne = new ofxDatGuiColorPicker("colorOne", ofColor::fromHex(xmlParameters.getValue("settings:colorOne", 0x000000)));
	colorOne->setPosition(x, y);
	colorOne->setWidth(x11 / 2, 0.6);
	colorOne->setLabel("Color A");
	colorOne->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(colorOne);

    colorTwo = new ofxDatGuiColorPicker("colorTwo", ofColor::fromHex(xmlParameters.getValue("settings:colorTwo", 0xFFFFFF)));
	colorTwo->setPosition(x + x11/2, y);
	colorTwo->setWidth(x11/2, 0.6);
	colorTwo->setLabel("Color B");
	colorTwo->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
    components.push_back(colorTwo);

    y += component->getHeight() + p + dHeight + 8*dHeight;
    component = new ofxDatGuiButton("saveImg");
    component->setPosition(x, y);
    component->setWidth(x11 * 0.5, 1);
    component->setLabel("GUARDAR ARCHIVO BITMAP");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

    component = new ofxDatGuiButton("saveVector");
    component->setPosition(x + 0.5 * x11, y);
    component->setWidth(x11 * 0.5, 1);
    component->setLabel("GUARDAR ARCHIVO VECTOR");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);

	y += component->getHeight() + p + dHeight;
	component = new ofxDatGuiButton("reset");
	component->setPosition(x, y);
	component->setWidth(x11 * 0.333, 1);
	component->setLabel("RESET");
	component->onButtonEvent(this, &ofApp::onButtonEvent);
	components.push_back(component);

	component = new ofxDatGuiButton("loadProject");
	component->setPosition(x + 0.333 * x11, y);
	component->setWidth(x11 * 0.333, 1);
	component->setLabel("CARGAR PROYECTO");
	component->onButtonEvent(this, &ofApp::onButtonEvent);
	components.push_back(component);

	component = new ofxDatGuiButton("saveProject");
	component->setPosition(x + 0.666 * x11, y);
	component->setWidth(x11 * 0.334, 1);
	component->setLabel("GUARDAR PROYECTO");
	component->onButtonEvent(this, &ofApp::onButtonEvent);
	components.push_back(component);

    y += component->getHeight() + p + 4 * dHeight;
    component = new ofxDatGuiButton("exit");
    component->setPosition(x, y);
    component->setWidth(x11, 1);
    component->setLabel("SALIR");
    component->onButtonEvent(this, &ofApp::onButtonEvent);
    components.push_back(component);


}

void ofApp::fitCanvas()
{

    float x11 = 40 * dWidth;

    float ratioCanvas = myengine.canvas.getWidth() / myengine.canvas.getHeight();
    float ratioDisplay = (ofGetWidth() - x11) / (ofGetHeight() - 2 * dHeight);

    if (ratioCanvas > ratioDisplay)
    {

        if (myengine.canvas.getWidth() < ofGetWidth() - x11)
        {
            xCanvas = x11 + (ofGetWidth() - x11 - 2 * dWidth - myengine.canvas.getWidth())/2;
            yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - myengine.canvas.getHeight())/2;
            widthCanvas = myengine.canvas.getWidth();
            heightCanvas = myengine.canvas.getHeight();
            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);


        }

        else
        {
            xCanvas = x11;
            yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - float(ofGetWidth() - x11) / float(ratioCanvas)) / 2;
            widthCanvas = ofGetWidth() - x11 - 2 * dWidth;
            heightCanvas = float(ofGetWidth() - x11) / float(ratioCanvas);

            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);

        }

    }

    else
    {

        if (myengine.canvas.getHeight() < ofGetHeight() - 2 * dHeight)
        {
            xCanvas = x11 - 2 * dWidth + (ofGetWidth() - x11 - myengine.canvas.getWidth())/2;
            yCanvas = dHeight + (ofGetHeight() - 2 * dHeight - myengine.canvas.getHeight())/2;
            widthCanvas = myengine.canvas.getWidth();
            heightCanvas = myengine.canvas.getHeight();

            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);

        }

        else
        {
            xCanvas = x11 - 2 * dWidth + (ofGetWidth() - x11 - (ofGetHeight() - 2 * dHeight) * ratioCanvas)/2;
            yCanvas = dHeight;
            widthCanvas = (ofGetHeight() - 2 * dHeight) * ratioCanvas;
            heightCanvas = ofGetHeight() - 2 * dHeight;

            myengine.draw(xCanvas, yCanvas, widthCanvas, heightCanvas);

        }
    }


}


/*
    event listeners
*/

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)
{
    cout << "onButtonEvent: " << e.target->getLabel() << endl;

    if (e.target->is("UploadImg")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          pathImg = result.getPath();
          myengine.setInput(pathImg);
          myengine.needsUpdateGrid = true;
          myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;
          ancho->setText(std::to_string(myengine.input.getWidth()));
          alto->setText(std::to_string(myengine.input.getHeight()));
          myengine.updateBackground();

        }
    }

    if (e.target->is("deleteImg")) {

        myengine.deleteImg();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
    }

    else if (e.target->is("uploadMskImg")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          pathMskImg = result.getPath();
          myengine.setMask(pathMskImg);
          myengine.needsUpdateGrid = true;
		  myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskImg")) {

        myengine.definingMaskImg = true;

    }

    else if (e.target->is("colorMskImg")) {

        myengine.coloringMaskImg = true;

    }

    else if (e.target->is("deleteMskImg")) {

        myengine.deleteMask();
        myengine.input.getTexture().disableAlphaMask();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

        while (myengine.pathInput.getNumVertices() > 0) {

            myengine.pathInput.removeVertex(0);

        }

    }

    else if (e.target->is("uploadMskGrid")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          pathMskGrid = result.getPath();
          myengine.setMaskGrid(pathMskGrid);
          myengine.needsUpdateGrid = true;
            myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskGrid")) {

        myengine.definingMaskGrid = true;

    }

    else if (e.target->is("deleteMskGrid")) {

        myengine.deleteMaskGrid();
        myengine.needsUpdateGrid = true;
        //myengine.needsUpdatePoints = true;
        //myengine.needsDrawPoints = true;

        while (myengine.pathGrid.getNumVertices() > 0) {

            myengine.pathGrid.removeVertex(0);

        }

    }

    else if (e.target->is("uploadMskPoints")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          pathMskPoints = result.getPath();
          myengine.setMaskPoints(pathMskPoints);
          // myengine.needsUpdateGrid = true;
          myengine.needsUpdatePoints = true;
          myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskPoints")) {

        myengine.definingMaskPoints = true;

    }

    else if (e.target->is("deleteMskPoints")) {

        myengine.deleteMaskPoints();
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

        while (myengine.pathPoints.getNumVertices() > 0) {

            myengine.pathPoints.removeVertex(0);

        }

    }

    else if (e.target->is("saveImg")) {

        ofFileDialogResult result = ofSystemSaveDialog("default.jpg", "Save");

        if(result.bSuccess) {
          string path = result.getPath();
            
            
          ofPixels pixels;
          myengine.canvas.readToPixels(pixels);
          pixels.setImageType(OF_IMAGE_COLOR);
            
          ofImage output;
          output.allocate(myengine.canvas.getWidth(), myengine.canvas.getHeight(), OF_IMAGE_COLOR);
          output.setFromPixels(pixels);
          output.save(path, OF_IMAGE_QUALITY_BEST);
          cout << "Saved in: " << path << endl;

        }

    }
    else if (e.target->is("saveVector")) {
        
        ofFileDialogResult result = ofSystemSaveDialog("default.svg", "Save");
        
        if(result.bSuccess) {
            string path = result.getPath();
            
            myengine.drawVectors(path);
            
            cout << "Saved in: " << path << endl;
            
        }
        
    }
    
    else if (e.target->is("graphicRandom")) {
        
        myengine.numSVG++;
        if (myengine.numSVG == myengine.svgTextures.size()) myengine.numSVG = 0;
		numSVG = myengine.numSVG;

        //myengine.numSVG = abs(ofRandom(myengine.svgTextures.size()));
        myengine.centerSVG = ofPoint(ofRandom(myengine.width), ofRandom(myengine.height));
        
    }

	else if (e.target->is("loadProject")) {

		ofFileDialogResult result = ofSystemLoadDialog("Load project file");

		if (result.bSuccess) {
			pathProject = result.getPath();

			loadSettings();

		}
	}
    
	else if (e.target->is("saveProject")) {

		ofFileDialogResult result = ofSystemSaveDialog("project.xml", "Save project file");

		if (result.bSuccess) {
			pathProject = result.getPath();

			saveSettings();

		}
	}

	else if (e.target->is("reset")) {

		resetSettings();

	}
    
    else if (e.target->is("exit")) {

        ofExit();

    }

}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{
    cout << "onToggleEvent: " << e.target->getLabel() << "::" <<  e.target->getChecked() << endl;

    if (e.target->is("showInput")) {

        myengine.showInput = e.target->getChecked();


    }
    
    else if (e.target->is("showGrid")) {
        
        myengine.showGrid = e.target->getChecked();
        myengine.needsDrawPoints = true;
        
        
    }

    else if (e.target->is("showPoints")) {
        
        myengine.showPoints = e.target->getChecked();
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("defineBackground")) {

        myengine.showBackground = e.target->getChecked();

        if (loadBackground->getChecked()) {
            loadBackground->toggle();
            myengine.showBackgroundFile = loadBackground->getChecked();
        }

        if (colorBackground->getChecked()) {
            colorBackground->toggle();
            myengine.showBackgroundColor = colorBackground->getChecked();
        }
        
        myengine.updateBackground();


    }

    else if (e.target->is("colorBackground")) {
        
        myengine.showBackgroundColor = e.target->getChecked();
        
        if (loadBackground->getChecked()) {
            loadBackground->toggle();
            myengine.showBackgroundFile = loadBackground->getChecked();
        }
        
        if (defineBackground->getChecked()) {
            defineBackground->toggle();
            myengine.showBackground = defineBackground->getChecked();
        }
        
        myengine.updateBackground();
        
        
    }
    
    else if (e.target->is("loadBackground")) {

        myengine.showBackgroundFile = e.target->getChecked();

        if (myengine.showBackgroundFile) {

            ofFileDialogResult result = ofSystemLoadDialog("Load file");

            if(result.bSuccess) {

              pathBackground = result.getPath();
              myengine.setBackground(pathBackground);

              // load your file at `path`
            }
        }

        if (defineBackground->getChecked()) {
            defineBackground->toggle();
            myengine.showBackground = defineBackground->getChecked();
        }
        
        if (colorBackground->getChecked()) {
            colorBackground->toggle();
            myengine.showBackgroundColor = colorBackground->getChecked();
        }
        myengine.updateBackground();


    }

    else if (e.target->is("punto")) {

        myengine.shapeDrawing = 1;
        if (cuadrado->getChecked()) cuadrado->toggle();
        if (triangulo->getChecked()) triangulo->toggle();
        if (archivo->getChecked()) archivo->toggle();
        myengine.needsDrawPoints = true;


    }

    else if (e.target->is("cuadrado")) {

        myengine.shapeDrawing = 2;
        if (punto->getChecked()) punto->toggle();
        if (triangulo->getChecked()) triangulo->toggle();
        if (archivo->getChecked()) archivo->toggle();
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("triangulo")) {

        myengine.shapeDrawing = 3;
        if (punto->getChecked()) punto->toggle();
        if (cuadrado->getChecked()) cuadrado->toggle();
        if (archivo->getChecked()) archivo->toggle();
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("archivo")) {

		ofFileDialogResult result = ofSystemLoadDialog("Load file");

		if (result.bSuccess) {
			pathShape = result.getPath();
			myengine.setShape(pathShape);
		}

        myengine.shapeDrawing = 4;

        if (punto->getChecked()) punto->toggle();
        if (cuadrado->getChecked()) cuadrado->toggle();
        if (triangulo->getChecked()) triangulo->toggle();
        myengine.needsDrawPoints = true;

    }

    
    else if (e.target->is("graphicElements")) {

        myengine.showTextures = e.target->getChecked();

    }
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
    cout << "onSliderEvent: " << e.value << "::" << e.scale << endl;
    if (e.target->is("Density")) {

        myengine.density = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("Noise")) {

        myengine.noise = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("min")) {

        myengine.min = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("max")) {

        myengine.max = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;

    }

    if (e.target->is("DensityP")) {
        
        myengine.densityP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("NoiseP")) {
        
        myengine.noiseP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }

    else if (e.target->is("levelMsk")) {
        
        myengine.levelMsk = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
        
        
        if ((myengine.coloringMaskImg)) {
            
            if (myengine.input.isAllocated()) {
                
                float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
                float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
                ofColor colorSelected = myengine.input.getColor(xPoint, yPoint);
                
                myengine.fboInput.begin();
                
                // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
                ofClear(0, 0, 0, 0);
                
                ofMesh points;
                points.setMode(OF_PRIMITIVE_POINTS);
                
                for (int i = 0; i < myengine.input.getWidth(); i++) {
                    
                    for (int j = 0; j < myengine.input.getHeight(); j++) {
                        
                        ofColor colorPoint = myengine.input.getColor(i,j);
                        
                        float distanciaColor = sqrt( pow(colorSelected.r - colorPoint.r, 2) + pow(colorSelected.g - colorPoint.g, 2) + pow(colorSelected.b - colorPoint.b, 2));
                        
                        if (distanciaColor > myengine.levelMsk) points.addVertex(ofPoint(i,j,0));
                        
                    }
                    
                }
                
                ofSetColor(255);
                points.draw();
                
                myengine.fboInput.end();
                
                ofPixels pixels;
                myengine.fboInput.readToPixels(pixels);
                myengine.maskInput.setFromPixels(pixels);
                myengine.maskInput = blur(myengine.maskInput, 5);
                
                myengine.input.getTexture().setAlphaMask(myengine.maskInput.getTexture());
                
                
                
            }
            
            // myengine.coloringMaskImg = false;
            
        }
        
        
        
        
        
    }
    
    else if (e.target->is("minP")) {
        
        myengine.minP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("maxP")) {
        
        myengine.maxP = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("prob")) {

        myengine.prob = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("size")) {

        myengine.pointSize = e.value;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("stroke")) {

        myengine.lineWidth = e.value;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("angleBackground")) {

        myengine.angleBackground = e.value;
        myengine.updateBackground();

    }

    else if (e.target->is("opacityImg")) {
        
        myengine.opacityImg = e.value;
        myengine.updateBackground();
        
    }

    else if (e.target->is("opacityGrid")) {
        
        myengine.opacityGrid = e.value;
        myengine.needsDrawPoints = true;
    }
    
    else if (e.target->is("opacityPoints")) {
        
        myengine.opacityPoints = e.value;
        myengine.needsUpdatePoints = true;
        myengine.needsDrawPoints = true;
        
    }
}


void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
    cout << "onDropdownEvent: " << e.child << endl;
}


void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{
    cout << "onColorPickerEvent: " << e.color << endl;

    if (e.target->is("color")) {
        myengine.colorPoint = e.color;
        myengine.needsDrawPoints = true;

    }
    else if (e.target->is("colorOne")) {
        myengine.colorOne = ofColor(e.color);
        myengine.updateBackground();
    }
    else if (e.target->is("colorTwo")) {
        myengine.colorTwo = ofColor(e.color);
        myengine.updateBackground();
    }
    else if (e.target->is("colorGrid")) {
        myengine.colorTriangle = ofColor(e.color);
        // myengine.updateBackground();
    }

	else if (e.target->is("colorSVG")) {
		myengine.colorSVG = ofColor(e.color);
		// myengine.updateBackground();
	}
}


void ofApp::onTextInputEvent(ofxDatGuiTextInputEvent e)
{
    cout << "onButtonEvent: " << e.text << endl;
    if (e.target->is("Definir Ancho")) {

        myengine.setResolution(std::stoi(e.text), myengine.height);
        myengine.updateBackground();
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;



    }

    else if (e.target->is("Definir Alto")) {

        myengine.setResolution(myengine.width, std::stoi(e.text));
        myengine.updateBackground();
        myengine.needsUpdateGrid = true;
        myengine.needsDrawPoints = true;


    }

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if ((key == 'q') || (key == 'Q')) {

        if (myengine.definingMaskImg) myengine.definingMaskImg = false;
        if (myengine.definingMaskGrid) myengine.definingMaskGrid = false;
        if (myengine.definingMaskPoints) myengine.definingMaskPoints = false;

    }

    else if ((key == 'a') || (key == 'A')) {
        
        if (myengine.coloringMaskImg) myengine.coloringMaskImg = false;
        
    }

	else if ((key == 'l') || (key == 'L')) {

		loadSettings();

	}

	else if ((key == 's') || (key == 'S')) {

		saveSettings();

	}

	else if ((key == 'r') || (key == 'R')) {

		resetSettings();

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	colorSelectorGrid.mousePressed(x, y, button);
	if (colorSelectorGrid.selectedColor != ofColor(0)) {
	
		colorGrid->setColor(colorSelectorGrid.selectedColor);
		myengine.colorTriangle.set(colorSelectorGrid.selectedColor);

	}


	colorSelectorPoint.mousePressed(x, y, button);
	if (colorSelectorPoint.selectedColor != ofColor(0)) {

		color->setColor(colorSelectorPoint.selectedColor);
		myengine.colorPoint.set(colorSelectorPoint.selectedColor);
		myengine.needsDrawPoints = true;

	}

	colorSelectorColorSVG.mousePressed(x, y, button);
	if (colorSelectorColorSVG.selectedColor != ofColor(0)) {
	
		colorSVG->setColor(colorSelectorColorSVG.selectedColor);
		myengine.colorSVG = colorSelectorColorSVG.selectedColor;
	}

	colorSelectorOne.mousePressed(x, y, button);
	if (colorSelectorOne.selectedColor != ofColor(0)) {

		colorOne->setColor(colorSelectorOne.selectedColor);
		myengine.colorOne.set(colorSelectorOne.selectedColor);
		myengine.updateBackground();
	}

	colorSelectorTwo.mousePressed(x, y, button);
	if (colorSelectorTwo.selectedColor != ofColor(0)) {

		colorTwo->setColor(colorSelectorTwo.selectedColor);
		myengine.colorTwo.set(colorSelectorTwo.selectedColor);
		myengine.updateBackground();
	}

    if ((myengine.definingMaskImg) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathInput.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathInput.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathInput.getVertex(0);
        ofPoint last = myengine.pathInput.getVertex(myengine.pathInput.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < 100) && (myengine.pathInput.getNumVertices() > 2)) {

            myengine.definingMaskImg = false;
            myengine.fboInput.begin();
            ofBackground(0);
            myengine.pathInput.draw();
            myengine.fboInput.end();

            ofPixels pixels;
            myengine.fboInput.readToPixels(pixels);
            myengine.maskInput.setFromPixels(pixels);
            myengine.maskInput = blur(myengine.maskInput, myengine.input.getWidth()/10);

            //myengine.input.getTexture().setAlphaMask(myengine.maskInput.getTexture());

        }

    }

    if ((myengine.definingMaskGrid) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathGrid.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathGrid.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathGrid.getVertex(0);
        ofPoint last = myengine.pathGrid.getVertex(myengine.pathGrid.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < 100) && (myengine.pathGrid.getNumVertices() > 2)) {

            myengine.definingMaskGrid = false;
            myengine.fboGrid.begin();
            ofBackground(0, 0, 0, 0);
            myengine.pathGrid.draw();
            myengine.fboGrid.end();

            ofPixels pixels;
            myengine.fboGrid.readToPixels(pixels);
            myengine.maskGrid.setFromPixels(pixels);

            myengine.needsUpdateGrid = true;
            myengine.needsDrawPoints = true;

        }

    }

    if ((myengine.definingMaskPoints) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathPoints.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathPoints.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathPoints.getVertex(0);
        ofPoint last = myengine.pathPoints.getVertex(myengine.pathPoints.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < 100) && (myengine.pathPoints.getNumVertices() > 2)) {

            myengine.definingMaskPoints = false;
            myengine.fboPoints.begin();
            ofBackground(0, 0, 0, 0);
            myengine.pathPoints.draw();
            myengine.fboPoints.end();

            ofPixels pixels;
            myengine.fboPoints.readToPixels(pixels);
            myengine.maskPoints.setFromPixels(pixels);
			myengine.maskPoints = blur(myengine.maskPoints, myengine.maskPoints.getWidth() / 10);


            myengine.needsUpdateGrid = true;
            myengine.needsDrawPoints = true;
        }

    }

    if ((myengine.coloringMaskImg) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        if (myengine.input.isAllocated()) {

            float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
            float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
            ofColor colorSelected = myengine.input.getColor(xPoint, yPoint);

            myengine.fboInput.begin();

            // Cleaning everthing with alpha mask on 0 in order to make it transparent by default
            ofClear(0, 0, 0, 0);

            ofMesh points;
            points.setMode(OF_PRIMITIVE_POINTS);

            for (int i = 0; i < myengine.input.getWidth(); i++) {

                for (int j = 0; j < myengine.input.getHeight(); j++) {
                    
                    ofColor colorPoint = myengine.input.getColor(i,j);
                    
                    float distanciaColor = sqrt( pow(colorSelected.r - colorPoint.r, 2) + pow(colorSelected.g - colorPoint.g, 2) + pow(colorSelected.b - colorPoint.b, 2));
                    
                    if (distanciaColor > myengine.levelMsk) points.addVertex(ofPoint(i,j,0));

                }

            }

            ofSetColor(255);
            points.draw();

            myengine.fboInput.end();

            ofPixels pixels;
            myengine.fboInput.readToPixels(pixels);
            myengine.maskInput.setFromPixels(pixels);
            myengine.maskInput = blur(myengine.maskInput, 5);

            myengine.input.getTexture().setAlphaMask(myengine.maskInput.getTexture());



        }

    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


ofImage ofApp::blur(ofImage img, int radio) {

    ofImage imgReturn;

    ofxCv::copy(img, imgReturn);
    ofxCv::blur(imgReturn, radio);
    imgReturn.update();
    
    return imgReturn;

}

void ofApp::resetSettings() {

	ancho->setText(ofToString(1024));
	alto->setText(ofToString(768));
	showInput->setChecked(true);
	opacityImg->setValue(100);
	levelMsk->setValue(100);
	showGrid->setChecked(false);
	opacityGrid->setValue(100);
	min->setValue(0);
	max->setValue(255);
	density->setValue(20);
	noise->setValue(5);
	stroke->setValue(1);
	colorGrid->setColor(0xC75B12);
	showPoints->setChecked(false);
	opacityPoints->setValue(100);
	minP->setValue(0);
	maxP->setValue(255);
	densityP->setValue(20);
	noiseP->setValue(5);
	size->setValue(20);
	punto->setChecked(true);
	cuadrado->setChecked(false);
	triangulo->setChecked(false);
	archivo->setChecked(false);
	color->setColor(0x4D5357);
	graphicElements->setChecked(false);
	numSVG = 0;
	colorSVG->setColor(0xFFFFFF);
	loadBackground->setChecked(false);
	colorBackground->setChecked(true);
	defineBackground->setChecked(false);
	angleBackground->setValue(0);
	colorOne->setColor(0x203244);
	colorTwo->setColor(0xDDDBD6);

	pathImg = "";
	pathMskImg = "";
	pathMskGrid = "";
	pathMskPoints = "";
	pathShape = "";


	myengine.deleteImg();
	myengine.deleteMask();
	myengine.deleteMaskGrid();
	myengine.deleteMaskPoints();

	myengine.deleteMask();
	myengine.input.getTexture().disableAlphaMask();


	while (myengine.pathInput.getNumVertices() > 0) {

		myengine.pathInput.removeVertex(0);

	}

	myengine.deleteMaskGrid();
	while (myengine.pathGrid.getNumVertices() > 0) {

		myengine.pathGrid.removeVertex(0);

	}

	myengine.deleteMaskPoints();
	while (myengine.pathPoints.getNumVertices() > 0) {

		myengine.pathPoints.removeVertex(0);

	}

	updateValues();

	myengine.needsUpdateGrid = true;
	myengine.needsUpdatePoints = true;
	myengine.needsDrawPoints = true;
	myengine.setResolution(1024, 768);
	myengine.updateBackground();
	myengine.update();
;


}

void ofApp::loadSettings() {

	xmlParameters.loadFile(pathProject);


	ancho->setText(ofToString(xmlParameters.getValue("settings:width", 1024)));
	alto->setText(ofToString(xmlParameters.getValue("settings:height", 768)));
	showInput->setChecked(ofToBool(xmlParameters.getValue("settings:showInput", "true")));
	opacityImg->setValue(xmlParameters.getValue("settings:opacityImg", 100));
	levelMsk->setValue(xmlParameters.getValue("settings:levelMsk", 100));
	showGrid->setChecked(ofToBool(xmlParameters.getValue("settings:showGrid", "true")));
	opacityGrid->setValue(xmlParameters.getValue("settings:opacityGrid", 100));
	min->setValue(xmlParameters.getValue("settings:min", 0));
	max->setValue(xmlParameters.getValue("settings:max", 255));
	density->setValue(xmlParameters.getValue("settings:density", 40));
	noise->setValue(xmlParameters.getValue("settings:noise", 0));
	stroke->setValue(xmlParameters.getValue("settings:stroke", 1));
	showPoints->setChecked(ofToBool(xmlParameters.getValue("settings:showPoints", "true")));
	opacityPoints->setValue(xmlParameters.getValue("settings:opacityPoints", 100));
	minP->setValue(xmlParameters.getValue("settings:minP", 0));
	maxP->setValue(xmlParameters.getValue("settings:maxP", 255));
	densityP->setValue(xmlParameters.getValue("settings:densityP", 40));
	noiseP->setValue(xmlParameters.getValue("settings:noiseP", 0));
	size->setValue(xmlParameters.getValue("settings:size", 5));
	punto->setChecked(ofToBool(xmlParameters.getValue("settings:punto", "true")));
	cuadrado->setChecked(ofToBool(xmlParameters.getValue("settings:cuadrado", "false")));
	triangulo->setChecked(ofToBool(xmlParameters.getValue("settings:triangulo", "false")));
	archivo->setChecked(ofToBool(xmlParameters.getValue("settings:archivo", "false")));
	graphicElements->setChecked(ofToBool(xmlParameters.getValue("settings:graphicElements", "false")));
	numSVG = xmlParameters.getValue("settings:numSVG", 0);
	loadBackground->setChecked(ofToBool(xmlParameters.getValue("settings:loadBackground", "false")));
	colorBackground->setChecked(ofToBool(xmlParameters.getValue("settings:colorBackground", "true")));
	defineBackground->setChecked(ofToBool(xmlParameters.getValue("settings:defineBackground", "false")));
	angleBackground->setValue(xmlParameters.getValue("settings:angleBackground", 0));

	string strColorGrid = xmlParameters.getValue("settings:colorGrid", "0xFFFFFF");
	cout << "colorGrid: " << strColorGrid << endl;

	colorGrid->setColor(ofHexToInt("0x" + strColorGrid));
	colorGrid->setText(strColorGrid);

	string strColor = xmlParameters.getValue("settings:color", "0x000000");
	color->setColor(ofHexToInt(strColor));
	color->setText(strColor);

	string strColorOne = xmlParameters.getValue("settings:colorOne", "0x000000");
	colorOne->setColor(ofHexToInt(strColorOne));
	colorOne->setText(strColorOne);

	string strColorTwo = xmlParameters.getValue("settings:colorTwo", "0xFFFFFF");
	colorTwo->setColor(ofHexToInt(strColorTwo));
	colorTwo->setText(strColorTwo);

	string strColorSVG = xmlParameters.getValue("settings:colorSVG", "0xFFFFFF");
	colorSVG->setColor(ofHexToInt(strColorSVG));
	colorSVG->setText(strColorSVG);

	pathImg = xmlParameters.getValue("settings:pathImg", "");
	pathMskImg = xmlParameters.getValue("settings:pathMskImg", "");
	pathMskGrid = xmlParameters.getValue("settings:pathMskGrid", "");
	pathMskPoints = xmlParameters.getValue("settings:pathMskPoints", "");
	pathBackground = xmlParameters.getValue("settings:pathBackground", "");
	pathShape = xmlParameters.getValue("settings:pathShape", "");

	if (pathImg != "") myengine.setInput(pathImg);
	else myengine.deleteImg();

	if (pathMskImg != "") myengine.setMask(pathMskImg);
	else myengine.deleteMask();

	if (pathMskGrid != "") myengine.setMaskGrid(pathMskGrid);
	else myengine.deleteMaskGrid();

	if (pathMskPoints != "") myengine.setMaskPoints(pathMskPoints);
	else myengine.deleteMaskPoints();

	if (pathBackground != "") myengine.setBackground(pathBackground);	

	if (pathShape != "") myengine.setShape(pathShape);


	myengine.pathInput.clear();
	xmlParameters.pushTag("pathInput");
	int numberOfSavedPoints = xmlParameters.getNumTags("position");
	for (int i = 0; i < numberOfSavedPoints; i++) {
		xmlParameters.pushTag("position", i);

		ofPoint p;
		p.x = xmlParameters.getValue("X", 0);
		p.y = xmlParameters.getValue("Y", 0);

		myengine.pathInput.addVertex(p);
		xmlParameters.popTag();
	}
	xmlParameters.popTag(); //pop position

	if (myengine.pathInput.getNumVertices() > 2) {

		myengine.definingMaskImg = false;
		myengine.fboInput.begin();
		ofBackground(0);
		myengine.pathInput.draw();
		myengine.fboInput.end();

		ofPixels pixels;
		myengine.fboInput.readToPixels(pixels);
		myengine.maskInput.setFromPixels(pixels);
		myengine.maskInput = blur(myengine.maskInput, myengine.input.getWidth() / 10);

	}


	myengine.pathGrid.clear();
	xmlParameters.pushTag("pathGrid");
	numberOfSavedPoints = xmlParameters.getNumTags("position");
	for (int i = 0; i < numberOfSavedPoints; i++) {
		xmlParameters.pushTag("position", i);

		ofPoint p;
		p.x = xmlParameters.getValue("X", 0);
		p.y = xmlParameters.getValue("Y", 0);

		myengine.pathGrid.addVertex(p);
		xmlParameters.popTag();
	}
	xmlParameters.popTag(); //pop position

	if (myengine.pathGrid.getNumVertices() > 2) {

		myengine.definingMaskGrid = false;
		myengine.fboGrid.begin();
		ofBackground(0, 0, 0, 0);
		myengine.pathGrid.draw();
		myengine.fboGrid.end();

		ofPixels pixels;
		myengine.fboGrid.readToPixels(pixels);
		myengine.maskGrid.setFromPixels(pixels);

	}

	myengine.pathPoints.clear();
	xmlParameters.pushTag("pathPoints");
	numberOfSavedPoints = xmlParameters.getNumTags("position");
	for (int i = 0; i < numberOfSavedPoints; i++) {
		xmlParameters.pushTag("position", i);

		ofPoint p;
		p.x = xmlParameters.getValue("X", 0);
		p.y = xmlParameters.getValue("Y", 0);

		myengine.pathPoints.addVertex(p);
		xmlParameters.popTag();
	}
	xmlParameters.popTag(); //pop position

	if (myengine.pathPoints.getNumVertices() > 2) {

		myengine.definingMaskPoints = false;
		myengine.fboPoints.begin();
		ofBackground(0, 0, 0, 0);
		myengine.pathPoints.draw();
		myengine.fboPoints.end();

		ofPixels pixels;
		myengine.fboPoints.readToPixels(pixels);
		myengine.maskPoints.setFromPixels(pixels);

	}







	updateValues();

	myengine.setResolution(myengine.width, myengine.height);
	myengine.updateGrid();
	myengine.updatePoints();
	myengine.drawPoints();
	myengine.updateBackground();
	myengine.update();


}

void ofApp::saveSettings() {

	xmlParameters.setValue("settings:width", ofToInt(ancho->getText()));
	xmlParameters.setValue("settings:height", ofToInt(alto->getText()));
	xmlParameters.setValue("settings:showInput", showInput->getChecked());
	xmlParameters.setValue("settings:opacityImg", opacityImg->getValue());
	xmlParameters.setValue("settings:levelMsk", levelMsk->getValue());
	xmlParameters.setValue("settings:showGrid", showGrid->getChecked());
	xmlParameters.setValue("settings:opacityGrid", opacityGrid->getValue());
	xmlParameters.setValue("settings:min", min->getValue());
	xmlParameters.setValue("settings:max", max->getValue());
	xmlParameters.setValue("settings:density", density->getValue());
	xmlParameters.setValue("settings:noise", noise->getValue());
	xmlParameters.setValue("settings:stroke", stroke->getValue());
	xmlParameters.setValue("settings:colorGrid", colorGrid->getText());
	xmlParameters.setValue("settings:showPoints", showPoints->getChecked());
	xmlParameters.setValue("settings:opacityPoints", opacityPoints->getValue());
	xmlParameters.setValue("settings:minP", minP->getValue());
	xmlParameters.setValue("settings:maxP", maxP->getValue());
	xmlParameters.setValue("settings:densityP", densityP->getValue());
	xmlParameters.setValue("settings:noiseP", noiseP->getValue());
	xmlParameters.setValue("settings:size", size->getValue());
	xmlParameters.setValue("settings:punto", punto->getChecked());
	xmlParameters.setValue("settings:cuadrado", cuadrado->getChecked());
	xmlParameters.setValue("settings:triangulo", triangulo->getChecked());
	xmlParameters.setValue("settings:archivo", archivo->getChecked());
	xmlParameters.setValue("settings:color",  color->getText());
	xmlParameters.setValue("settings:graphicElements", graphicElements->getChecked());
	xmlParameters.setValue("settings:numSVG", numSVG);
	xmlParameters.setValue("settings:colorSVG", colorSVG->getText());
	xmlParameters.setValue("settings:loadBackground", loadBackground->getChecked());
	xmlParameters.setValue("settings:colorBackground", colorBackground->getChecked());
	xmlParameters.setValue("settings:defineBackground", defineBackground->getChecked());
	xmlParameters.setValue("settings:angleBackground", angleBackground->getValue());
	xmlParameters.setValue("settings:colorOne", colorOne->getText() );
	xmlParameters.setValue("settings:colorTwo", colorTwo->getText() );
	xmlParameters.setValue("settings:pathImg", pathImg);
	xmlParameters.setValue("settings:pathMskImg", pathMskImg);
	xmlParameters.setValue("settings:pathMskGrid", pathMskGrid);
	xmlParameters.setValue("settings:pathMskPoints", pathMskPoints);
	xmlParameters.setValue("settings:pathBackground", pathBackground);
	xmlParameters.setValue("settings:pathShape", pathShape);
	xmlParameters.addTag("pathInput");

	xmlParameters.pushTag("pathInput");
	//points is a vector<ofPoint> that we want to save to a file
	for (int i = 0; i < myengine.pathInput.getNumVertices(); i++) {
		//each position tag represents one point
		xmlParameters.addTag("position");
		xmlParameters.pushTag("position", i);
		//so set the three values in the file
		xmlParameters.addValue("X", myengine.pathInput.getVertex(i).x);
		xmlParameters.addValue("Y", myengine.pathInput.getVertex(i).y);
		xmlParameters.popTag();//pop position
	}
	xmlParameters.popTag(); //pop position

	xmlParameters.addTag("pathGrid");

	xmlParameters.pushTag("pathGrid");
	//points is a vector<ofPoint> that we want to save to a file
	for (int i = 0; i < myengine.pathGrid.getNumVertices(); i++) {
		//each position tag represents one point
		xmlParameters.addTag("position");
		xmlParameters.pushTag("position", i);
		//so set the three values in the file
		xmlParameters.addValue("X", myengine.pathGrid.getVertex(i).x);
		xmlParameters.addValue("Y", myengine.pathGrid.getVertex(i).y);
		xmlParameters.popTag();//pop position
	}
	xmlParameters.popTag(); //pop position

	xmlParameters.addTag("pathPoints");

	xmlParameters.pushTag("pathPoints");
	//points is a vector<ofPoint> that we want to save to a file
	for (int i = 0; i < myengine.pathPoints.getNumVertices(); i++) {
		//each position tag represents one point
		xmlParameters.addTag("position");
		xmlParameters.pushTag("position", i);
		//so set the three values in the file
		xmlParameters.addValue("X", myengine.pathPoints.getVertex(i).x);
		xmlParameters.addValue("Y", myengine.pathPoints.getVertex(i).y);
		xmlParameters.popTag();//pop position
	}
	xmlParameters.popTag(); //pop position

	xmlParameters.saveFile(pathProject);

}

void ofApp::updateValues() {

	myengine.width = ofToInt(ancho->getText());
	myengine.height = ofToInt(alto->getText());
	myengine.showInput = showInput->getChecked();
	myengine.opacityImg = opacityImg->getValue();
	myengine.levelMsk = levelMsk->getValue();
	myengine.showGrid = showGrid->getChecked();
	myengine.opacityGrid = opacityGrid->getValue();
	myengine.min = min->getValue();
	myengine.max = max->getValue();
	myengine.density = density->getValue();
	myengine.noise = noise->getValue();
	myengine.lineWidth = stroke->getValue();
	myengine.colorTriangle = colorGrid->getColor();
	myengine.showPoints = showPoints->getChecked();
	myengine.opacityPoints = opacityPoints->getValue();
	myengine.minP = minP->getValue();
	myengine.maxP = maxP->getValue();
	myengine.densityP = densityP->getValue();
	myengine.noiseP = noiseP->getValue();
	myengine.pointSize = size->getValue();
	myengine.numSVG = numSVG;

	if (punto->getChecked()) myengine.shapeDrawing = 1;
	else if (cuadrado->getChecked()) myengine.shapeDrawing = 2;
	else if (triangulo->getChecked()) myengine.shapeDrawing = 3;
	else if (archivo->getChecked()) myengine.shapeDrawing = 4;


	myengine.colorPoint = color->getColor();
	myengine.colorTriangle = colorGrid->getColor();
	myengine.colorSVG = colorSVG->getColor();
	myengine.showTextures = graphicElements->getChecked();
	myengine.showBackground = defineBackground->getChecked();
	myengine.showBackgroundColor = colorBackground->getChecked();
	myengine.showBackgroundFile = loadBackground->getChecked();
	myengine.angleBackground = angleBackground->getValue();
	myengine.colorOne = colorOne->getColor();
	myengine.colorTwo = colorTwo->getColor();



}
