/*==================================================================================================
 COURSE:                CSC 525/625
 ASSIGNMENT:            Project 2
 DATE:                  11/11/20
 TEAM MEMBERS (CONTRIBUTION PERCENTAGE; SUBTASKS):
 -David Engleman        (16.67%; )
 -Matthew Glenn         (16.67%; created code)
 -Benjamin Pottebaum    (16.67%; )
 -Niko Robbins          (16.67%; created code)
 -Tristan Tyler         (16.67%; created code)
 -Alicia Willard        (16.67%; )
 DESCRIPTION:           A text editor that renders text entered by a user using keyboard input, 
                        then uses menus to change the text's color or font, to open or close 
                        an info window, and to terminate the program.
 NOTES:                 Must use C:\Temp as the location of the files or the root of the file hierarchy.
 FILES:                 project2_team1.cpp, labProject.sln, freeglut.dll, glut.h, freeglut.lib
 IDE/COMPILER:          Microsoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
    1.      Double click on labProject.sln          to OPEN the project
    2.      Press Ctrl+F7                           to COMPILE
    3.      Press Ctrl+F5                           to EXECUTE
    4.      Type some text in the 'Editor' window   to DISPLAY the text in black
    5.      Right-click in the 'Editor' window      to OPEN the 'Editor' menu
            then select 'Info'                      to SHOW the 'Info' window
    OR      select 'Color'                          to CHANGE the text color
    OR      select 'Font'                           to CHANGE the text font
    OR      select 'Quit'                           to TERMINATE the program
    6.      Right-click in the 'Info' window        to OPEN the 'Info' menu
            then select 'Close'                     to HIDE the 'Info' window
==================================================================================================*/
#include <iostream>
#include <string>
#include <vector>
#include <GL/glut.h>

GLubyte text_color[3] = { 0, 0, 0 };
int colorValue;
int fontValue;
void* font = GLUT_BITMAP_TIMES_ROMAN_24;
void* helpFont = GLUT_BITMAP_HELVETICA_18;
void* axesFont = GLUT_BITMAP_TIMES_ROMAN_24;
int mainWindow, helpWindow;

// Line Variables
GLfloat lineScale = 1;
int blackColor[3] = { 0, 0, 0 };

// Window Variables
GLint windowPadding = 20;
GLint windowWidth = 550;
GLint windowHeight = 775;

// Text and Font Variables
std::string draw_text;
void* helvetica = GLUT_BITMAP_HELVETICA_18;
void* roman = GLUT_BITMAP_TIMES_ROMAN_24;

// Page Variables
std::vector<std::vector<std::string>> pages;
int currentPage = 0;
int currentLine = 0;
int currentChar = 0;
unsigned int maxCharacters = 30;
unsigned int maxLines = 30;

void mainInit();
void helpInit();
void mainDisplayCallback();
void helpDisplayCallback();
void keyboardCallback(unsigned char key, int cursorX, int cursorY);
void mouseCallback(int button, int state, int x, int y);
void mainMenuHandler(int num);
void drawMenu();
void helpMenuHandler(int num);
void drawHelpMenu();
void drawEditorText();
void drawText(std::string input, int x, int y);
void drawHelpText();
void setTextColor();
void setFont();
void drawLayout();
void drawTextLayout();
void drawCustomText(std::string text, int x, int y, int rgb[3], void* font);
void addPage();
int getNumberOfCharacters();


int main(int argc, char** argv) {
    addPage();
    glutInit(&argc, argv);                          // Initialization
    glutInitWindowSize(windowWidth, windowHeight);  // Specify a window size
    glutInitWindowPosition(300, 10);                // Specify a window position
    mainWindow = glutCreateWindow("Editor");        // Create a titled window
    drawMenu();
    mainInit();                                     // Specify some settings
    glutDisplayFunc(mainDisplayCallback);           // Register a callback
    glutKeyboardFunc(keyboardCallback);             // Register a keyboard callback
    glutMouseFunc(mouseCallback);
    glutInitWindowSize(500, 275);                   // Specify a window size
    glutInitWindowPosition(855, 250);               // Specify a window position
    helpWindow = glutCreateWindow("Info");
    drawHelpMenu();
    helpInit();
    glutDisplayFunc(helpDisplayCallback);           // Register a callback
    glutMainLoop();                                 // Get into an infinite loop

    return 0;                                       // Something wrong happened
}


void mainInit() {
    glClearColor(1, 1, 1, 0);                     // Specify a background color: white
	gluOrtho2D(0, windowWidth, windowHeight, 0);  // Specify a viewing area
}


void helpInit() {
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 500, 275, 0);
}


void mainDisplayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);  // Draw the background
    drawEditorText();
    drawLayout();
    drawTextLayout();
    glFlush();                     // Flush out the buffer contents
}


void helpDisplayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);  // Draw the background
    drawHelpText();
    glFlush();                     // Flush out the buffer contents
}


void mouseCallback(int button, int state, int x, int y) {
    if (button == 0 && state == 0) {
        int line = (y - (windowPadding + 6)) / 24;
        if (line > maxLines - 1)
            line = maxLines - 1;
        if (line < 0)
            line = 0;
        currentLine = line;
    }
}


void keyboardCallback(unsigned char key, int cursorX, int cursorY) {
    // Go to next line when over 30 characters
    if (pages[currentPage][currentLine].length() > (maxCharacters - 1))
        currentLine += 1;
    // Enter key pressed
    if (int(key) == 13)
        currentLine += 1;
    // If exceeds lines on page, it creates a new page
    if (currentLine == maxLines) {
        currentLine = 0;
        currentPage += 1;
        addPage();
    }
    // Backspace or delete pressed
    if (int(key) == 8 || int(key) == 127) {
        pages[currentPage][currentLine] = pages[currentPage][currentLine].substr(0, pages[currentPage][currentLine].length() - 1);
        if (pages[currentPage][currentLine].length() == 0) {
            currentLine -= 1;
            if (currentLine < 0)
                currentLine = 0;
            if (currentPage != 0 && currentLine == 0) {
                currentLine = maxLines - 1;
                currentPage -= 1;
            }
        }
    }
    if (int(key) != 8 && int(key) != 127 && int(key) != 13)
        pages[currentPage][currentLine] += key;
    
    mainDisplayCallback();
}


void setTextColor() {
    if (colorValue == 1) {
        // Black
        text_color[0] = 0;
        text_color[1] = 0;
        text_color[2] = 0;
    }
    if (colorValue == 2) {
        // Green
        text_color[0] = 25;
        text_color[1] = 200;
        text_color[2] = 25;
    }
    else if (colorValue == 3) {
        // Purple
        text_color[0] = 128;
        text_color[1] = 0;
        text_color[2] = 128;
    }
    else if (colorValue == 4) {
        // Red
        text_color[0] = 255;
        text_color[1] = 0;
        text_color[2] = 0;
    }
    else if (colorValue == 5) {
        // Random
        text_color[0] = rand() % 256;
        text_color[1] = rand() % 256;
        text_color[2] = rand() % 256;
    }
}


void setFont() {
    if (fontValue == 6)
        font = GLUT_BITMAP_9_BY_15;
    if (fontValue == 7)
        font = GLUT_BITMAP_HELVETICA_18;
    else if (fontValue == 8)
        font = GLUT_BITMAP_TIMES_ROMAN_24;
}


void mainMenuHandler(int num) {
    if (num == 0)
        exit(0);
    else if (num == 1 || num == 2 || num == 3 || num == 4 || num == 5) {
        colorValue = num;
        setTextColor();
    }
    else if (num == 6 || num == 7 || num == 8) {
        fontValue = num;
        setFont();
    }
    else if (num == 9) {
        glutSetWindow(helpWindow);
        glutShowWindow();
        glutSetWindow(mainWindow);
    }
    mainDisplayCallback();
}


void drawMenu() {
    int colorMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Black", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Purple", 3);
    glutAddMenuEntry("Red", 4);
    glutAddMenuEntry("Random", 5);
    int fontMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("9 By 15", 6);
    glutAddMenuEntry("Helvetica", 7);
    glutAddMenuEntry("Times New Roman", 8);
    int mainMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Info", 9);
    glutAddSubMenu("Color", colorMenuId);
    glutAddSubMenu("Font", fontMenuId);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void helpMenuHandler(int num) {
    if (num == 10) {
        glutSetWindow(helpWindow);
        glutHideWindow();
    }
    helpDisplayCallback();
}


void drawHelpMenu() {
    int mainMenuId = glutCreateMenu(helpMenuHandler);
    glutAddMenuEntry("Close", 10);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void drawHelpText() {
    glColor3ub(0, 0, 0);

    drawText("Type some text in the 'Editor' window to display it in black.", 5, 40);
    drawText("Press the backspace key to remove a character.", 5, 60);
    drawText("Press the 'Enter' key to start typing on the next row.", 5, 80);

    drawText("Right-clicking in the 'Editor' window opens this menu:", 5, 120);
    drawText("    'Info'  shows this 'Info' window", 5, 140);
    drawText("    'Color' opens a submenu to change text color", 5, 160);
    drawText("    'Font'  opens a submenu to change text font", 5, 180);
    drawText("    'Quit'  terminates the program", 5, 200);

    drawText("Right-clicking in this 'Info' window opens this menu:", 5, 240);
    drawText("    'Close' hides this 'Info' window", 5, 260);
}


void drawEditorText() {
    glColor3ubv(text_color);
    glRasterPos2i(5, windowPadding + 30 + (currentLine * 24));  // Draw line indicator
    glutBitmapCharacter(font, '|');

    for (size_t line = 0; line < pages[currentPage].size(); line++) {
        glRasterPos2i(windowPadding + 15, windowPadding + 30 + (line * 24));
        for (unsigned i = 0; i < pages[currentPage][line].length(); i++)
            glutBitmapCharacter(font, pages[currentPage][line][i]);
    }
}


void addPage() {
    std::vector<std::string> newPage;

    for (unsigned int i = 0; i < maxLines; i++)
        newPage.push_back("");

    pages.push_back(newPage);
}


void drawText(std::string input, int x, int y) {
    glRasterPos2i(x, y);

    for (unsigned i = 0; i < input.length(); i++)
        glutBitmapCharacter(helpFont, input[i]);
}


// Draws any text displayed on screen
void drawTextLayout() {
    std::string page = "Page " + std::to_string(currentPage + 1) + " of " + std::to_string(pages.size());
    drawCustomText(page, windowPadding + 5, windowHeight - 5, blackColor, helvetica); 
    drawCustomText("Line: " + std::to_string(currentLine + 1), windowPadding + 215, windowHeight - 5, blackColor, helvetica);
    drawCustomText("Characters: " + std::to_string(getNumberOfCharacters()), windowPadding + 375, windowHeight - 5, blackColor, helvetica);
}


int getNumberOfCharacters() {
    int chars = 0;

    for (size_t page = 0; page < pages.size(); page++) {
        for (size_t line = 0; line < pages[page].size(); line++)
            chars += pages[page][line].length();
    }
    return chars;
}


// Texts
void drawCustomText(std::string text, int x, int y, int rgb[3], void* font) {
	draw_text = text;
	glColor3ub(rgb[0], rgb[1], rgb[2]);
	glRasterPos2i(x, y);

	for (size_t i = 0; i < draw_text.length(); i++)
		glutBitmapCharacter(font, draw_text[i]);
}


// Line segments
void drawLine(int x1, int y1, int x2, int y2, int rgb[3]) {
    glEnable(GL_LINE_STIPPLE);
    glLineWidth(1);
    glBegin(GL_LINES);
    glColor3ub(rgb[0], rgb[1], rgb[2]);
    glVertex2f(x1 * lineScale, y1 * lineScale);
    glVertex2f(x2 * lineScale, y2 * lineScale);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
}


// Draws the surrounding border and individual section lines
void drawLayout() {
    drawLine(windowPadding, windowPadding, windowWidth - windowPadding, windowPadding, blackColor);  // Top line
    drawLine(windowPadding, windowHeight - windowPadding, windowWidth - windowPadding, windowHeight - windowPadding, blackColor);  // Bottom line
    drawLine(windowPadding, windowPadding, windowPadding, windowHeight - windowPadding, blackColor);  // Left line
    drawLine(windowWidth - windowPadding, windowPadding, windowWidth - windowPadding, windowHeight - windowPadding, blackColor);  // Right line
}