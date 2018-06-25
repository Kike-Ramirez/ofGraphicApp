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
    ofSetWindowTitle("ofGraphicApp v1.50");
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

    myengine.update();

}

void ofApp::draw()
{
    //ofBackgroundGradient(ofColor(0), ofColor(255), OF_GRADIENT_LINEAR);
	ofBackground(backgrounds[tIndex]);
	fitCanvas();



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

	if (myengine.showTextures && myengine.definingSvgCenter) {

		if ((mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

			ofSetColor(255, 0, 0);
			ofLine(mouseX - 5, mouseY, mouseX + 5, mouseY);
			ofLine(mouseX, mouseY - 5, mouseX, mouseY + 5);

		}

		ofDrawBitmapString("Mover Elementos Graficos", xCanvas + 10, yCanvas + 20);
		ofSetColor(255);

	}
}

void ofApp::loadGui() {

    int x = dWidth;
    int y = dHeight;
    int p = 0;
    int x11 = 38 * dWidth;

	// Cargar Temas del GUI
	themes = { new ofxDatGuiTheme(true),
		new ofxDatGuiThemeSmoke(),
		new ofxDatGuiThemeWireframe(),
		new ofxDatGuiThemeMidnight(),
		new ofxDatGuiThemeAqua(),
		new ofxDatGuiThemeCharcoal(),
		new ofxDatGuiThemeAutumn(),
		new ofxDatGuiThemeCandy() };

	// Cargar fondos
	backgrounds = {
		ofColor(150),
		ofColor(0x343B41),
		ofColor(0xFCFAFD),
		ofColor(0x011726),
		ofColor(0x445966),
		ofColor(0x28292E),
		ofColor(0x4C4743),
		ofColor(0xFF4081)
	};

	// Cargar colores corporativos
	corporativeColors = {

		ofColor(199, 91, 18),
		ofColor(77, 83, 87),
		ofColor(177, 228, 227),
		ofColor(107, 164, 184),
		ofColor(144, 40, 130),
		ofColor(255, 177, 187),
		ofColor(220, 134, 153),
		ofColor(255, 141, 109),
		ofColor(99, 198, 191),
		ofColor(0, 45, 66),
		ofColor(65, 12, 55),
		ofColor(238, 33, 87),
		ofColor(166, 9, 61),
		ofColor(238, 39, 55),
		ofColor(248, 224, 142),
		ofColor(80, 166, 132),
		ofColor(215, 210, 203),
		ofColor(255, 205, 0),
		ofColor(0, 104, 94),
		ofColor(39, 37, 31)

	};

	corporativeColorNames = {

		"Pantone 159C",
		"Pantone 445C",
		"Pantone 317C",
		"Pantone 549C",
		"Pantone 249C",
		"Pantone 176C",
		"Pantone 493C",
		"Pantone 1635C",
		"Pantone 570C",
		"Pantone 534C",
		"Pantone 7449C",
		"Pantone 1925C",
		"Pantone 1945C",
		"Pantone 1788C",
		"Pantone 1205C",
		"Pantone 7723C",
		"Warm Gray 1C",
		"Pantone 116C",
		"Pantone 329C",
		"Process Black EC"

	};

	// launch the app //
	mFullscreen = false;
	tIndex = 0;
	// instantiate and position the gui //
	gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);

	// Area de Trabajo
	ofxDatGuiFolder* folderArea = gui->addFolder("RESOLUCION DE TRABAJO", ofColor::red);
	folderArea->addTextInput("width", xmlParameters.getValue("settings:width", "1024"))->setLabel("-> Definir Ancho");
	folderArea->addTextInput("height", xmlParameters.getValue("settings:height", "762"))->setLabel("-> Definir Alto");
	folderArea->addFRM()->setLabel("-> FrameRate");

	folderArea->expand();

	gui->addBreak();

	// Imagen de entrada
	ofxDatGuiFolder* folderInput = gui->addFolder("IMAGEN DE ENTRADA", ofColor::blue);
	folderInput->addToggle("showInput", xmlParameters.getValue("settings:showInput", true))->setLabel("-> Mostrar Imagen");
	folderInput->addButton("uploadImg")->setLabel("-> Archivo");
	folderInput->addButton("deleteImg")->setLabel("-> Borrar");
	folderInput->addSlider("opacityImg", 0, 100, xmlParameters.getValue("settings:opacityImg", 100))->setLabel("-> Opacidad");
	folderInput->addButton("uploadMskImg")->setLabel("-> Mascara Imagen");
	folderInput->addButton("defineMskImg")->setLabel("-> Definir Forma");
	folderInput->addButton("colorMskImg")->setLabel("-> Definir Color");
	folderInput->addButton("deleteMskImg")->setLabel("-> Borrar Mascara");
	folderInput->addSlider("levelMsk", 0, 100, xmlParameters.getValue("settings:levelMsk", 100))->setLabel("-> Umbral Masc. Color");
	//folderInput->addToggle("Mostrar Imagen", xmlParameters.getValue("settings:showInput", true));
	folderInput->collapse();

	gui->addBreak();

	// Fondo de pantalla
	ofxDatGuiFolder* folderPantalla = gui->addFolder("FONDO", ofColor::blue);

	folderPantalla->addToggle("loadBackground", xmlParameters.getValue("settings:loadBackground", false))->setLabel("-> Archivo");
	folderPantalla->addToggle("colorBackground", xmlParameters.getValue("settings:colorBackground", true))->setLabel("-> Color");
	folderPantalla->addToggle("defineBackground", xmlParameters.getValue("settings:defineBackground", false))->setLabel("-> Degradado");
	folderPantalla->addSlider("angleBackground", 0, 180, xmlParameters.getValue("settings:angleBackground", 0))->setLabel("-> Angulo de degradado");
	folderPantalla->addColorPicker("colorOne", ofColor::fromHex(xmlParameters.getValue("settings:colorOne", 0x000000)))->setLabel("-> Color A");
	folderPantalla->addColorPicker("colorTwo", ofColor::fromHex(xmlParameters.getValue("settings:colorTwo", 0xFFFFFF)))->setLabel("-> Color B");
	folderPantalla->collapse();

	gui->addBreak();


    // Malla de lineas
	ofxDatGuiFolder* folderLineas = gui->addFolder("MALLA DE LINEAS", ofColor::green);
	folderLineas->addToggle("showGrid", xmlParameters.getValue("settings:showGrid", true))->setLabel("-> Mostrar malla");
	folderLineas->addSlider("opacityGrid", 0, 100, xmlParameters.getValue("settings:opacityGrid", 100))->setLabel("-> Opacidad");
	folderLineas->addSlider("max", 0, 255, xmlParameters.getValue("settings:mask", 255))->setLabel("-> Maximo");
	folderLineas->addSlider("min", 0, 255, xmlParameters.getValue("settings:min", 0))->setLabel("-> Minimo");
	folderLineas->addSlider("density", 4, 40, xmlParameters.getValue("settings:density", 10))->setLabel("-> Espaciado");
	folderLineas->addSlider("noise", 0, 100, xmlParameters.getValue("settings:noise", 0))->setLabel("-> Desplazamiento");
	folderLineas->addSlider("stroke", 0, 10, xmlParameters.getValue("settings:stroke", 1))->setLabel("-> Grosor Linea");
	folderLineas->addButton("uploadMskGrid")->setLabel("-> Archivo Mascara");
	folderLineas->addButton("defineMskGrid")->setLabel("-> Definir Forma");
	folderLineas->addButton("deleteMskGrid")->setLabel("-> Borrar Mascara");
	folderLineas->addColorPicker("colorGrid", ofColor::fromHex(xmlParameters.getValue("settings:colorGrid", 0xFFFFFF)))->setLabel("-> Color");
	folderLineas->collapse();

	gui->addBreak();

	// Malla de puntos
	ofxDatGuiFolder* folderPuntos = gui->addFolder("MALLA DE PUNTOS", ofColor::green);
	folderPuntos->addToggle("showPoints", xmlParameters.getValue("settings:showPoints", true))->setLabel("-> Mostrar puntos");
	folderPuntos->addSlider("opacityPoints", 0, 100, xmlParameters.getValue("settings:opacityPoints", 100))->setLabel("-> Opacidad");
	folderPuntos->addSlider("minP", 0, 255, xmlParameters.getValue("settings:maxP", 255))->setLabel("-> Maximo");
	folderPuntos->addSlider("maxP", 0, 255, xmlParameters.getValue("settings:minP", 0))->setLabel("-> Minimo");
	folderPuntos->addSlider("densityP", 4, 40, xmlParameters.getValue("settings:densityP", 10))->setLabel("-> Espaciado");
	folderPuntos->addSlider("noiseP", 0, 100, xmlParameters.getValue("settings:noiseP", 0))->setLabel("-> Desplazamiento");
	folderPuntos->addSlider("size", 0, 100, xmlParameters.getValue("settings:size", 5))->setLabel("-> Diametro");
	folderPuntos->addToggle("circulo", xmlParameters.getValue("settings:punto", true))->setLabel("-> Circulo");
	folderPuntos->addToggle("cuadrado", xmlParameters.getValue("settings:cuadrado", false))->setLabel("-> Cuadrado");
	folderPuntos->addToggle("triangulo", xmlParameters.getValue("settings:triangulo", false))->setLabel("-> Triangulo");
	folderPuntos->addToggle("archivo", xmlParameters.getValue("settings:archivo", false))->setLabel("-> Archivo");
	folderPuntos->addButton("uploadMskPoints")->setLabel("-> Archivo Mascara");
	folderPuntos->addButton("defineMskPoints")->setLabel("-> Definir Mascara");
	folderPuntos->addButton("deleteMskPoints")->setLabel("-> Borrar Mascara");
	folderPuntos->addColorPicker("color", ofColor::fromHex(xmlParameters.getValue("settings:color", 0x000000)))->setLabel("-> Color");
	folderPuntos->collapse();

	gui->addBreak();

	// Elementos Graficos
	ofxDatGuiFolder* folderGraficos = gui->addFolder("ELEMENTOS GRAFICOS", ofColor::yellow);
	folderGraficos->addToggle("graphicElements", xmlParameters.getValue("settings:graphicElements", false))->setLabel("-> Mostrar elementos graficos");
	folderGraficos->addButton("graphicRandom")->setLabel("-> Cambiar graficos");
	folderGraficos->addButton("svgCenter")->setLabel("-> Posicion");;
	folderGraficos->addSlider("svgSize", 0.1, 5, xmlParameters.getValue("settings:svgSize", 1))->setLabel("-> Escala");
	folderGraficos->addColorPicker("colorSVG", ofColor::fromHex(xmlParameters.getValue("settings:color", 0x000000)))->setLabel("-> Color");
	folderGraficos->collapse();

	gui->addBreak();

	// Colores corporativos
	vector<string> opts = { "Lineas", "Puntos", "Elem. Graficos", "Fondo A", "Fondo B" };
	gui->addLabel("COLORES CORPORATIVOS")->setStripeColor(ofColor::yellow);
	gui->addDropdown("-> Elemento", opts)->setStripeColor(ofColor::yellow);
	gui->addDropdown("-> Color", corporativeColorNames)->setStripeColor(ofColor::yellow);

	for (int i = 0; i < corporativeColors.size(); i++) {
		
		gui->getDropdown("-> Color")->getChildAt(i)->setBackgroundColor(corporativeColors[i]);
	}

	gui->addBreak();

	// Guardar y Exportar
	ofxDatGuiFolder* folderGuardar = gui->addFolder("GUARDAR - EXPORTAR", ofColor::red);
	folderGuardar->addButton("saveImg")->setLabel("-> Exportar Render");
	folderGuardar->addButton("saveVector")->setLabel("-> Exportar Vector");
	folderGuardar->addButton("loadProject")->setLabel("-> Abrir Proyecto");
	folderGuardar->addButton("saveProject")->setLabel("-> Guardar Proyecto");
	folderGuardar->addButton("reset")->setLabel("-> Borrar Proyecto");
	folderGuardar->collapse();

	gui->addBreak();

	// General
	ofxDatGuiFolder* folderGeneral = gui->addFolder("GENERAL", ofColor::red);
	folderGeneral->addToggle("toggle fullscreen", false)->setLabel("-> Pantalla Completa");
	folderGeneral->addButton("theme")->setLabel("-> Cambiar Tema");
	folderGeneral->addButton("exit")->setLabel("-> Salir");
	folderGeneral->expand();

	// Añadir Header y Footer
	gui->addHeader(":: REPOSICIONAR ::");
	gui->addFooter();
	gui->getFooter()->setLabelWhenExpanded("CERRAR MENU");
	gui->getFooter()->setLabelWhenCollapsed("ABRIR MENU");

	// Registrar Callbacks
	gui->onButtonEvent(this, &ofApp::onButtonEvent);
	gui->onToggleEvent(this, &ofApp::onToggleEvent);
	gui->onSliderEvent(this, &ofApp::onSliderEvent);
	gui->onTextInputEvent(this, &ofApp::onTextInputEvent);
	gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
	gui->onDropdownEvent(this, &ofApp::onDropdownEvent);



}

void ofApp::toggleFullscreen()
{
	mFullscreen = !mFullscreen;
	gui->getToggle("toggle fullscreen")->setChecked(mFullscreen);
	ofSetFullscreen(mFullscreen);
}

void ofApp::changeTheme() 
{
	tIndex = tIndex < themes.size() - 1 ? tIndex + 1 : 0;
	gui->setTheme(themes[tIndex]);
}

void ofApp::fitCanvas()
{

	float x11 = gui->getWidth() + dWidth;

	//	40 * dWidth;

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

    if (e.target->is("UploadImg")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          pathImg = result.getPath();
          myengine.setInput(pathImg);
          myengine.needsUpdateGrid = true;
		  myengine.needsUpdateDrawGrid = true;
          myengine.needsUpdateDrawPoints = true;
          myengine.needsDrawPoints = true;
          gui->getTextInput("width")->setText(std::to_string(myengine.input.getWidth()));
		  gui->getTextInput("height")->setText(std::to_string(myengine.input.getHeight()));
          myengine.updateBackground();

        }
    }

    if (e.target->is("deleteImg")) {

        myengine.deleteImg();
        myengine.needsUpdateGrid = true;
		myengine.needsUpdateDrawGrid = true;
		myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
    }

    else if (e.target->is("uploadMskImg")) {

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
          pathMskImg = result.getPath();
          myengine.setMask(pathMskImg);
          myengine.needsUpdateGrid = true;
		  myengine.needsUpdateDrawGrid = true;
		  myengine.needsUpdateDrawPoints = true;
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
		myengine.needsUpdateDrawGrid = true;
		myengine.needsUpdateDrawPoints = true;
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
		  myengine.needsUpdateDrawGrid = true;
		  myengine.needsUpdateDrawPoints = true;          
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
		  myengine.needsUpdateDrawGrid = true;
		  myengine.needsUpdateDrawPoints = true;          
		  myengine.needsDrawPoints = true;

        }
    }

    else if (e.target->is("defineMskPoints")) {

        myengine.definingMaskPoints = true;

    }

    else if (e.target->is("deleteMskPoints")) {

        myengine.deleteMaskPoints();
        myengine.needsUpdateGrid = true;
		myengine.needsUpdateDrawGrid = true;
		myengine.needsUpdateDrawPoints = true;
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

        }

    }
    else if (e.target->is("saveVector")) {
        
        ofFileDialogResult result = ofSystemSaveDialog("default.svg", "Save");
        
		// KIKE: Revisar qué es este path que estamos guardando aquí.
        if(result.bSuccess) {
            string path = result.getPath();
            
			// KIKE: Aquí se llama a drawVectors que guarda el vectorial
            myengine.drawVectors(path);
            
            
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

	else if (e.target->is("theme")) {
		
		changeTheme();

	}
	
	else if (e.target->is("svgCenter")) {

		myengine.definingSvgCenter = true;

	}
}

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)
{
	if (e.target->is("-> Color")) {

		if (gui->getDropdown("-> Elemento")->getSelected()->getIndex() == 0) 
		{
			gui->getColorPicker("colorGrid")->setColor(corporativeColors[e.target->getSelected()->getIndex()]);
			myengine.colorTriangle = gui->getColorPicker("colorGrid")->getColor();

		}

		else if (gui->getDropdown("-> Elemento")->getSelected()->getIndex() == 1)
		{
			gui->getColorPicker("color")->setColor(corporativeColors[e.target->getSelected()->getIndex()]);
			myengine.colorPoint = gui->getColorPicker("color")->getColor();
			myengine.needsDrawPoints = true;
		}

		else if (gui->getDropdown("-> Elemento")->getSelected()->getIndex() == 2)
		{
			gui->getColorPicker("colorSVG")->setColor(corporativeColors[e.target->getSelected()->getIndex()]);
			myengine.colorSVG = gui->getColorPicker("colorSVG")->getColor();

		}

		else if (gui->getDropdown("-> Elemento")->getSelected()->getIndex() == 3)
		{
			gui->getColorPicker("colorOne")->setColor(corporativeColors[e.target->getSelected()->getIndex()]);
			myengine.colorOne = gui->getColorPicker("colorOne")->getColor();
			myengine.updateBackground();
		}

		else if (gui->getDropdown("-> Elemento")->getSelected()->getIndex() == 4)
		{
			gui->getColorPicker("colorTwo")->setColor(corporativeColors[e.target->getSelected()->getIndex()]);
			myengine.colorTwo = gui->getColorPicker("colorTwo")->getColor();
			myengine.updateBackground();
		}
	}
};

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)
{

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

        if (gui->getToggle("loadBackground")->getChecked()) {
			gui->getToggle("loadBackground")->toggle();
            myengine.showBackgroundFile = gui->getToggle("loadBackground")->getChecked();
        }

        if (gui->getToggle("colorBackground")->getChecked()) {
			gui->getToggle("colorBackground")->toggle();
            myengine.showBackgroundColor = gui->getToggle("colorBackground")->getChecked();
        }
        
        myengine.updateBackground();


    }

    else if (e.target->is("colorBackground")) {
        
        myengine.showBackgroundColor = e.target->getChecked();
        
        if (gui->getToggle("loadBackground")->getChecked()) {
			gui->getToggle("loadBackground")->toggle();
            myengine.showBackgroundFile = gui->getToggle("loadBackground")->getChecked();
        }
        
        if (gui->getToggle("defineBackground")->getChecked()) {
			gui->getToggle("defineBackground")->toggle();
            myengine.showBackground = gui->getToggle("defineBackground")->getChecked();
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

        if (gui->getToggle("defineBackground")->getChecked()) {
			gui->getToggle("defineBackground")->toggle();
            myengine.showBackground = gui->getToggle("defineBackground")->getChecked();
        }
        
        if (gui->getToggle("colorBackground")->getChecked()) {
			gui->getToggle("colorBackground")->toggle();
            myengine.showBackgroundColor = gui->getToggle("colorBackground")->getChecked();
        }
        myengine.updateBackground();


    }

    else if (e.target->is("punto")) {

        myengine.shapeDrawing = 1;
        if (gui->getToggle("cuadrado")->getChecked()) gui->getToggle("cuadrado")->toggle();
        if (gui->getToggle("triangulo")->getChecked())gui->getToggle("triangulo")->toggle();
        if (gui->getToggle("archivo")->getChecked()) gui->getToggle("archivo")->toggle();
        myengine.needsDrawPoints = true;


    }

    else if (e.target->is("cuadrado")) {

        myengine.shapeDrawing = 2;
        if (gui->getToggle("circulo")->getChecked()) gui->getToggle("circulo")->toggle();
		if (gui->getToggle("triangulo")->getChecked())gui->getToggle("triangulo")->toggle();
		if (gui->getToggle("archivo")->getChecked()) gui->getToggle("archivo")->toggle();
		myengine.needsDrawPoints = true;

    }

    else if (e.target->is("triangulo")) {

        myengine.shapeDrawing = 3;
		if (gui->getToggle("circulo")->getChecked()) gui->getToggle("circulo")->toggle();
		if (gui->getToggle("cuadrado")->getChecked()) gui->getToggle("cuadrado")->toggle();
		if (gui->getToggle("archivo")->getChecked()) gui->getToggle("archivo")->toggle();
		myengine.needsDrawPoints = true;

    }

    else if (e.target->is("archivo")) {

		ofFileDialogResult result = ofSystemLoadDialog("Load file");

		if (result.bSuccess) {
			pathShape = result.getPath();
			myengine.setShape(pathShape);
		}

        myengine.shapeDrawing = 4;

		if (gui->getToggle("circulo")->getChecked()) gui->getToggle("circulo")->toggle();
		if (gui->getToggle("cuadrado")->getChecked()) gui->getToggle("cuadrado")->toggle();
		if (gui->getToggle("triangulo")->getChecked())gui->getToggle("triangulo")->toggle();
		myengine.needsDrawPoints = true;

    }

    
    else if (e.target->is("graphicElements")) {

        myengine.showTextures = e.target->getChecked();

    }

	else if (e.target->is("toggle fullscreen")) {

		toggleFullscreen();
	}
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)
{
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
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("NoiseP")) {
        
        myengine.noiseP = e.value;
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
        
    }

    else if (e.target->is("levelMsk")) {
        
        myengine.levelMsk = e.value;
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
		myengine.needsUpdateMask = true;
                
    }
    
    else if (e.target->is("minP")) {
        
        myengine.minP = e.value;
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("maxP")) {
        
        myengine.maxP = e.value;
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
        
    }
    
    else if (e.target->is("prob")) {

        myengine.prob = e.value;
        myengine.needsUpdateGrid = true;
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("size")) {

        myengine.pointSize = e.value;
        myengine.needsDrawPoints = true;

    }

    else if (e.target->is("stroke")) {

        myengine.lineWidth = e.value;
		myengine.needsUpdateDrawGrid = true;
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
		myengine.needsUpdateDrawGrid = true;
        myengine.needsDrawPoints = true;
    }
    
    else if (e.target->is("opacityPoints")) {
        
        myengine.opacityPoints = e.value;
        myengine.needsUpdateDrawPoints = true;
        myengine.needsDrawPoints = true;
        
    }

	else if (e.target->is("svgSize")) {

		myengine.svgSize = e.value;

	}
}




void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)
{

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

	else if ((key == 'p') || (key == 'P')) {

		// if (myengine.definingSvgCenter) myengine.definingSvgCenter = false;

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

	if (myengine.showTextures && myengine.definingSvgCenter) {
	
		float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
		float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
		myengine.centerSVG = ofVec2f(xPoint, yPoint);

	}

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	//colorSelectorGrid.mousePressed(x, y, button);
	//if (colorSelectorGrid.selectedColor != ofColor(0)) {
	//
	//	colorGrid->setColor(colorSelectorGrid.selectedColor);
	//	myengine.colorTriangle.set(colorSelectorGrid.selectedColor);

	//}


	//colorSelectorPoint.mousePressed(x, y, button);
	//if (colorSelectorPoint.selectedColor != ofColor(0)) {

	//	color->setColor(colorSelectorPoint.selectedColor);
	//	myengine.colorPoint.set(colorSelectorPoint.selectedColor);
	//	myengine.needsDrawPoints = true;

	//}

	//colorSelectorColorSVG.mousePressed(x, y, button);
	//if (colorSelectorColorSVG.selectedColor != ofColor(0)) {
	//
	//	colorSVG->setColor(colorSelectorColorSVG.selectedColor);
	//	myengine.colorSVG = colorSelectorColorSVG.selectedColor;
	//}

	//colorSelectorOne.mousePressed(x, y, button);
	//if (colorSelectorOne.selectedColor != ofColor(0)) {

	//	colorOne->setColor(colorSelectorOne.selectedColor);
	//	myengine.colorOne.set(colorSelectorOne.selectedColor);
	//	myengine.updateBackground();
	//}

	//colorSelectorTwo.mousePressed(x, y, button);
	//if (colorSelectorTwo.selectedColor != ofColor(0)) {

	//	colorTwo->setColor(colorSelectorTwo.selectedColor);
	//	myengine.colorTwo.set(colorSelectorTwo.selectedColor);
	//	myengine.updateBackground();
	//}

    if ((myengine.definingMaskImg) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {

        myengine.pathInput.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
        float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
        float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
        myengine.pathInput.addVertex(ofVec2f(xPoint, yPoint));

        ofPoint origin = myengine.pathInput.getVertex(0);
        ofPoint last = myengine.pathInput.getVertex(myengine.pathInput.getNumVertices() - 1);

        float dist = pow(origin.x - last.x, 2) + pow(origin.y - last.y, 2);

        if ((dist < myengine.canvas.getWidth() / 10.0) && (myengine.pathInput.getNumVertices() > 2)) {

            myengine.definingMaskImg = false;
            myengine.fboInput.begin();
            ofBackground(0);
            myengine.pathInput.draw();
            myengine.fboInput.end();

            ofPixels pixels;
            myengine.fboInput.readToPixels(pixels);
            myengine.maskInput.setFromPixels(pixels);
            myengine.maskInput = myengine.blur(myengine.maskInput, myengine.maskInput.getWidth()/10);

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

        if ((dist < myengine.canvas.getWidth() / 10.0) && (myengine.pathGrid.getNumVertices() > 2)) {

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

        if ((dist < myengine.canvas.getWidth() / 10.0) && (myengine.pathPoints.getNumVertices() > 2)) {

            myengine.definingMaskPoints = false;
            myengine.fboPoints.begin();
            ofBackground(0, 0, 0, 0);
            myengine.pathPoints.draw();
            myengine.fboPoints.end();

            ofPixels pixels;
            myengine.fboPoints.readToPixels(pixels);
            myengine.maskPoints.setFromPixels(pixels);
			myengine.maskPoints = myengine.blur(myengine.maskPoints, myengine.maskPoints.getWidth() / 10);


            myengine.needsUpdateGrid = true;
            myengine.needsDrawPoints = true;
        }

    }

    if ((myengine.coloringMaskImg) && (mouseX > xCanvas) && (mouseX < xCanvas + widthCanvas) && (mouseY > yCanvas) && (mouseY< yCanvas + heightCanvas)) {


        if (myengine.input.isAllocated()) {

            float xPoint = ofMap(mouseX - xCanvas, 0, widthCanvas, 0, myengine.canvas.getWidth());
            float yPoint = ofMap(mouseY - yCanvas, 0, heightCanvas, 0, myengine.canvas.getHeight());
            myengine.colorMaskPoint = myengine.input.getColor(xPoint, yPoint);

			myengine.needsUpdateMask = true;
        }

    }

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	myengine.definingSvgCenter = false;

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


void ofApp::resetSettings() {

	gui->getTextInput("width")->setText(ofToString(1024));
	gui->getTextInput("height")->setText(ofToString(768));
	gui->getToggle("showInput")->setChecked(true);
	gui->getSlider("opacityImg")->setValue(100);
	gui->getSlider("levelMsk")->setValue(25);
	gui->getToggle("showGrid")->setChecked(false);
	gui->getSlider("opacityGrid")->setValue(100);
	gui->getSlider("min")->setValue(0);
	gui->getSlider("max")->setValue(255);
	gui->getSlider("density")->setValue(20);
	gui->getSlider("noise")->setValue(5);
	gui->getSlider("stroke")->setValue(1);
	gui->getColorPicker("colorGrid")->setColor(0xC75B12);
	gui->getToggle("showPoints")->setChecked(false);
	gui->getSlider("opacityPoints")->setValue(100);
	gui->getSlider("minP")->setValue(0);
	gui->getSlider("maxP")->setValue(255);
	gui->getSlider("densityP")->setValue(20);
	gui->getSlider("noiseP")->setValue(5);
	gui->getSlider("size")->setValue(20);
	gui->getToggle("circulo")->setChecked(true);
	gui->getToggle("cuadrado")->setChecked(false);
	gui->getToggle("triangulo")->setChecked(false);
	gui->getToggle("archivo")->setChecked(false);
	gui->getColorPicker("color")->setColor(0x4D5357);
	gui->getToggle("graphicElements")->setChecked(false);
	numSVG = 0;
	gui->getColorPicker("colorSVG")->setColor(0xFFFFFF);
	gui->getToggle("loadBackground")->setChecked(false);
	gui->getToggle("colorBackground")->setChecked(true);
	gui->getToggle("defineBackground")->setChecked(false);
	gui->getSlider("angleBackground")->setValue(0);
	gui->getColorPicker("colorOne")->setColor(0x203244);
	gui->getColorPicker("colorTwo")->setColor(0xDDDBD6);

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

	// KIKE: Aquí falta actualizar la parte del reset para puntos y triangulaciones.

	updateValues();

	myengine.needsUpdateGrid = true;
	myengine.needsUpdateDrawGrid = true;
	myengine.needsUpdateDrawPoints = true;
	myengine.needsDrawPoints = true;
	myengine.setResolution(1024, 768);
	myengine.updateBackground();
	myengine.update();
;


}

void ofApp::loadSettings() {

	xmlParameters.loadFile(pathProject);


	gui->getTextInput("width")->setText(ofToString(xmlParameters.getValue("settings:width", 1024)));
	gui->getTextInput("height")->setText(ofToString(xmlParameters.getValue("settings:height", 768)));
	gui->getToggle("showInput")->setChecked(ofToBool(xmlParameters.getValue("settings:showInput", "true")));
	gui->getSlider("opacityImg")->setValue(xmlParameters.getValue("settings:opacityImg", 100));
	gui->getSlider("levelMsk")->setValue(xmlParameters.getValue("settings:levelMsk", 25));
	gui->getToggle("showGrid")->setChecked(ofToBool(xmlParameters.getValue("settings:showGrid", "true")));
	gui->getSlider("opacityGrid")->setValue(xmlParameters.getValue("settings:opacityGrid", 100));
	gui->getSlider("min")->setValue(xmlParameters.getValue("settings:min", 0));
	gui->getSlider("max")->setValue(xmlParameters.getValue("settings:max", 255));
	gui->getSlider("density")->setValue(xmlParameters.getValue("settings:density", 40));
	gui->getSlider("noise")->setValue(xmlParameters.getValue("settings:noise", 0));
	gui->getSlider("stroke")->setValue(xmlParameters.getValue("settings:stroke", 1));
	gui->getToggle("showPoints")->setChecked(ofToBool(xmlParameters.getValue("settings:showPoints", "true")));
	gui->getSlider("opacityPoints")->setValue(xmlParameters.getValue("settings:opacityPoints", 100));
	gui->getSlider("minP")->setValue(xmlParameters.getValue("settings:minP", 0));
	gui->getSlider("maxP")->setValue(xmlParameters.getValue("settings:maxP", 255));
	gui->getSlider("densityP")->setValue(xmlParameters.getValue("settings:densityP", 40));
	gui->getSlider("noiseP")->setValue(xmlParameters.getValue("settings:noiseP", 0));
	gui->getSlider("size")->setValue(xmlParameters.getValue("settings:size", 5));
	gui->getToggle("circulo")->setChecked(ofToBool(xmlParameters.getValue("settings:punto", "true")));
	gui->getToggle("cuadrado")->setChecked(ofToBool(xmlParameters.getValue("settings:cuadrado", "false")));
	gui->getToggle("triangulo")->setChecked(ofToBool(xmlParameters.getValue("settings:triangulo", "false")));
	gui->getToggle("archivo")->setChecked(ofToBool(xmlParameters.getValue("settings:archivo", "false")));
	gui->getToggle("graphicElements")->setChecked(ofToBool(xmlParameters.getValue("settings:graphicElements", "false")));
	numSVG = xmlParameters.getValue("settings:numSVG", 0);
	gui->getSlider("svgSize")->setValue(xmlParameters.getValue("settings:svgSize", 1.0));
	gui->getToggle("loadBackground")->setChecked(ofToBool(xmlParameters.getValue("settings:loadBackground", "false")));
	gui->getToggle("colorBackground")->setChecked(ofToBool(xmlParameters.getValue("settings:colorBackground", "true")));
	gui->getToggle("defineBackground")->setChecked(ofToBool(xmlParameters.getValue("settings:defineBackground", "false")));
	gui->getSlider("angleBackground")->setValue(xmlParameters.getValue("settings:angleBackground", 0));

	string strColorGrid = xmlParameters.getValue("settings:colorGrid", "0xFFFFFF");

	gui->getColorPicker("colorGrid")->setColor(ofHexToInt("0x" + strColorGrid));
	gui->getColorPicker("colorGrid")->setText(strColorGrid);

	string strColor = xmlParameters.getValue("settings:color", "0x000000");
	gui->getColorPicker("color")->setColor(ofHexToInt(strColor));
	gui->getColorPicker("color")->setText(strColor);

	string strColorOne = xmlParameters.getValue("settings:colorOne", "0x000000");
	gui->getColorPicker("colorOne")->setColor(ofHexToInt(strColorOne));
	gui->getColorPicker("colorOne")->setText(strColorOne);

	string strColorTwo = xmlParameters.getValue("settings:colorTwo", "0xFFFFFF");
	gui->getColorPicker("colorTwo")->setColor(ofHexToInt(strColorTwo));
	gui->getColorPicker("colorTwo")->setText(strColorTwo);

	string strColorSVG = xmlParameters.getValue("settings:colorSVG", "0xFFFFFF");
	gui->getColorPicker("colorSVG")->setColor(ofHexToInt(strColorSVG));
	gui->getColorPicker("colorSVG")->setText(strColorSVG);

	string strColorMaskPoint = xmlParameters.getValue("settings:colorMaskPoint", "NULL");


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


	if (strColorMaskPoint != "NULL") {

		myengine.colorMaskPoint.setHex(ofHexToInt(strColorMaskPoint));
		myengine.updateMask();

	}

	else myengine.colorMaskPoint = NULL;



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
		myengine.maskInput = myengine.blur(myengine.maskInput, myengine.input.getWidth() / 10);

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
		myengine.maskGrid = myengine.blur(myengine.maskGrid, myengine.maskGrid.getWidth() / 10);

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
		myengine.maskPoints = myengine.blur(myengine.maskPoints, myengine.maskPoints.getWidth() / 10);


	}

	myengine.triangulation.reset(myengine.colorTriangle);
	xmlParameters.pushTag("triangulationGrid");
	numberOfSavedPoints = xmlParameters.getNumTags("position");
	for (int i = 0; i < numberOfSavedPoints; i++) {
		xmlParameters.pushTag("position", i);

		ofPoint p;
		p.x = xmlParameters.getValue("X", 0);
		p.y = xmlParameters.getValue("Y", 0);

		myengine.triangulation.addPoint(p);
		xmlParameters.popTag();
	}
	xmlParameters.popTag(); //pop position

	myengine.triangulation.triangulate();

	myengine.triangles.clear();
	xmlParameters.pushTag("pointsGrid");
	numberOfSavedPoints = xmlParameters.getNumTags("position");
	for (int i = 0; i < numberOfSavedPoints; i++) {
		xmlParameters.pushTag("position", i);

		ofPoint p;
		p.x = xmlParameters.getValue("X", 0);
		p.y = xmlParameters.getValue("Y", 0);

		myengine.triangles.push_back(p);
		xmlParameters.popTag();
	}
	xmlParameters.popTag(); //pop position

	updateValues();

	myengine.setResolution(myengine.width, myengine.height);

	myengine.updateBackground();
	myengine.needsUpdateGrid = false;
	myengine.needsUpdateDrawPoints = false;
	myengine.needsUpdateDrawGrid = false;
	myengine.needsDrawPoints = true;
	myengine.needsUpdateMask = false;

	myengine.update();


}

void ofApp::saveSettings() {

	xmlParameters.setValue("settings:width", ofToInt(gui->getTextInput("width")->getText()));
	xmlParameters.setValue("settings:height", ofToInt(gui->getTextInput("height")->getText()));
	xmlParameters.setValue("settings:showInput", gui->getToggle("showInput")->getChecked());
	xmlParameters.setValue("settings:opacityImg", gui->getSlider("opacityImg")->getValue());

	if (myengine.colorMaskPoint != NULL) {

		xmlParameters.setValue("settings:colorMaskPoint", ofToHex(myengine.colorMaskPoint.getHex()));

	}

	else xmlParameters.setValue("settings:colorMaskPoint", "NULL");
	
	xmlParameters.setValue("settings:levelMsk", gui->getSlider("levelMsk")->getValue());
	xmlParameters.setValue("settings:showGrid", gui->getToggle("showGrid")->getChecked());
	xmlParameters.setValue("settings:opacityGrid", gui->getSlider("opacityGrid")->getValue());
	xmlParameters.setValue("settings:min", gui->getSlider("min")->getValue());
	xmlParameters.setValue("settings:max", gui->getSlider("max")->getValue());
	xmlParameters.setValue("settings:density", gui->getSlider("density")->getValue());
	xmlParameters.setValue("settings:noise", gui->getSlider("noise")->getValue());
	xmlParameters.setValue("settings:stroke", gui->getSlider("stroke")->getValue());
	xmlParameters.setValue("settings:colorGrid", gui->getTextInput("colorGrid")->getText());
	xmlParameters.setValue("settings:showPoints", gui->getToggle("showPoints")->getChecked());
	xmlParameters.setValue("settings:opacityPoints", gui->getSlider("opacityPoints")->getValue());
	xmlParameters.setValue("settings:minP", gui->getSlider("minP")->getValue());
	xmlParameters.setValue("settings:maxP", gui->getSlider("maxP")->getValue());
	xmlParameters.setValue("settings:densityP", gui->getSlider("densityP")->getValue());
	xmlParameters.setValue("settings:noiseP", gui->getSlider("noiseP")->getValue());
	xmlParameters.setValue("settings:size", gui->getSlider("size")->getValue());
	xmlParameters.setValue("settings:punto", gui->getToggle("circulo")->getChecked());
	xmlParameters.setValue("settings:cuadrado", gui->getToggle("cuadrado")->getChecked());
	xmlParameters.setValue("settings:triangulo", gui->getToggle("triangulo")->getChecked());
	xmlParameters.setValue("settings:archivo", gui->getToggle("archivo")->getChecked());
	xmlParameters.setValue("settings:color",  gui->getColorPicker("color")->getText());
	xmlParameters.setValue("settings:graphicElements", gui->getToggle("graphicElements")->getChecked());
	xmlParameters.setValue("settings:numSVG", numSVG);
	xmlParameters.setValue("settings:colorSVG", gui->getColorPicker("colorSVG")->getText());
	xmlParameters.setValue("settings:svgSize", gui->getSlider("svgSize")->getValue());
	xmlParameters.setValue("settings:loadBackground", gui->getToggle("loadBackground")->getChecked());
	xmlParameters.setValue("settings:colorBackground", gui->getToggle("colorBackground")->getChecked());
	xmlParameters.setValue("settings:defineBackground", gui->getToggle("defineBackground")->getChecked());
	xmlParameters.setValue("settings:angleBackground", gui->getSlider("angleBackground")->getValue());
	xmlParameters.setValue("settings:colorOne", gui->getColorPicker("colorOne")->getText() );
	xmlParameters.setValue("settings:colorTwo", gui->getColorPicker("colorTwo")->getText() );
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

	xmlParameters.addTag("triangulationGrid");

	xmlParameters.pushTag("triangulationGrid");
	//points is a vector<ofPoint> that we want to save to a file
	for (int i = 0; i < myengine.triangulation.triangleMesh.getNumVertices(); i++) {
		//each position tag represents one point
		xmlParameters.addTag("position");
		xmlParameters.pushTag("position", i);
		//so set the three values in the file
		xmlParameters.addValue("X", myengine.triangulation.triangleMesh.getVertex(i).x);
		xmlParameters.addValue("Y", myengine.triangulation.triangleMesh.getVertex(i).y);
		xmlParameters.popTag();//pop position
	}
	xmlParameters.popTag(); //pop position

	xmlParameters.addTag("pointsGrid");
	xmlParameters.pushTag("pointsGrid");
	//points is a vector<ofPoint> that we want to save to a file
	for (int i = 0; i < myengine.triangles.size(); i++) {
		//each position tag represents one point
		xmlParameters.addTag("position");
		xmlParameters.pushTag("position", i);
		//so set the three values in the file
		xmlParameters.addValue("X", myengine.triangles[i].x);
		xmlParameters.addValue("Y", myengine.triangles[i].y);
		xmlParameters.popTag();//pop position
	}
	xmlParameters.popTag(); //pop position

	xmlParameters.saveFile(pathProject);

}

void ofApp::updateValues() {

	myengine.width = ofToInt(gui->getTextInput("width")->getText());
	myengine.height = ofToInt(gui->getTextInput("height")->getText());
	myengine.showInput = gui->getToggle("showInput")->getChecked();
	myengine.opacityImg = gui->getSlider("opacityImg")->getValue();
	myengine.levelMsk = gui->getSlider("levelMsk")->getValue();
	myengine.showGrid = gui->getToggle("showGrid")->getChecked();
	myengine.opacityGrid = gui->getSlider("opacityGrid")->getValue();
	myengine.min = gui->getSlider("min")->getValue();
	myengine.max = gui->getSlider("max")->getValue();
	myengine.density = gui->getSlider("density")->getValue();
	myengine.noise = gui->getSlider("noise")->getValue();
	myengine.lineWidth = gui->getSlider("stroke")->getValue();
	myengine.colorTriangle = gui->getColorPicker("colorGrid")->getColor();
	myengine.showPoints = gui->getToggle("showPoints")->getChecked();
	myengine.opacityPoints = gui->getSlider("opacityPoints")->getValue();
	myengine.minP = gui->getSlider("minP")->getValue();
	myengine.maxP = gui->getSlider("maxP")->getValue();
	myengine.densityP = gui->getSlider("densityP")->getValue();
	myengine.noiseP = gui->getSlider("noiseP")->getValue();
	myengine.pointSize = gui->getSlider("size")->getValue();
	myengine.numSVG = numSVG;
	myengine.svgSize = gui->getSlider("svgSize")->getValue();

	if (gui->getToggle("circulo")->getChecked()) myengine.shapeDrawing = 1;
	else if (gui->getToggle("cuadrado")->getChecked()) myengine.shapeDrawing = 2;
	else if (gui->getToggle("triangulo")->getChecked()) myengine.shapeDrawing = 3;
	else if (gui->getToggle("archivo")->getChecked()) myengine.shapeDrawing = 4;


	myengine.colorPoint = gui->getColorPicker("color")->getColor();
	myengine.colorTriangle = gui->getColorPicker("colorGrid")->getColor();
	myengine.colorSVG = gui->getColorPicker("colorSVG")->getColor();
	myengine.showTextures = gui->getToggle("graphicElements")->getChecked();
	myengine.showBackground = gui->getToggle("defineBackground")->getChecked();
	myengine.showBackgroundColor = gui->getToggle("colorBackground")->getChecked();
	myengine.showBackgroundFile = gui->getToggle("loadBackground")->getChecked();
	myengine.angleBackground = gui->getSlider("angleBackground")->getValue();
	myengine.colorOne = gui->getColorPicker("colorOne")->getColor();
	myengine.colorTwo = gui->getColorPicker("colorTwo")->getColor();



}
