/*==================================================================================================
 COURSE:                CSC 525/625
 ASSIGNMENT:            Project 2
 DATE:                  10/28/20
 TEAM MEMBERS (CONTRIBUTION PERCENTAGE; SUBTASKS):
 -David Engleman        (16.67%; )
 -Matthew Glenn         (16.67%; Created Code)
 -Benjamin Pottebaum    (16.67%; )
 -Niko Robbins          (16.67%; )
 -Tristan Tyler         (16.67%;)
 -Alicia Willard        (16.67%; )
 DESCRIPTION:           Text editor.....finsih later
 NOTES:                 Must use C:\Temp as the location of the files or the root of the file hierarchy.
 FILES:                 project2_team1.cpp, labProject.sln, freeglut.dll, glut.h, freeglut.lib
 IDE/COMPILER:          Microsoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
    1.      Double click on labProject.sln      to OPEN the project
    2.      Press Ctrl+F7                       to COMPILE
    3.      Press Ctrl+F5                       to EXECUTE
    4.      Type text                           to DISPLAY it in green
    5.      Press the 'Enter' key               to FINISH typing
    6.      Press the right mouse button        to OPEN the menu
            then select 'Color'                 to CHANGE text color
    OR      select 'Font'                       to CHANGE text font
    7.      Click text and hold mouse button    to MOVE text
    8.      Press the right mouse button        to OPEN the menu
            then select 'Quit'                  to TERMINATE the program
==================================================================================================*/
#include <iostream>
#include <string>
#include <GL/glut.h>

bool started_typing = false;
GLubyte text_color[3] = { 25, 200, 25 };
int cursor_x = 0;
int cursor_y = 0;
int colorValue;
int fontValue;
std::string userString;
void* font = GLUT_BITMAP_TIMES_ROMAN_24;
void* helpFont = GLUT_BITMAP_HELVETICA_18;
void* axesFont = GLUT_BITMAP_TIMES_ROMAN_24;
int mainWindow, helpWindow;

// Line Variables
GLfloat lineScale = 1;
int blackColor[3] = { 0, 0, 0 };

// Window Variables
GLint windowPadding = 20;
GLint windowWidth = 400 + (windowPadding * 2);
GLint windowHeight = 800 + (windowPadding * 2);

// Text and Font Variables
std::string draw_text;
void* helvetica = GLUT_BITMAP_HELVETICA_18;
void* roman = GLUT_BITMAP_TIMES_ROMAN_24;

void mainInit();
void helpInit();
void mainDisplayCallback();
void helpDisplayCallback();
void mouseMotionCallback(int mousex, int mousey);
void keyboardCallback(unsigned char key, int cursorX, int cursorY);
void mainMenuHandler(int num);
void drawMenu();
void helpMenuHandler(int num);
void drawHelpMenu();
void drawMainText();
void drawText(std::string input, int x, int y);
void drawHelpText();
void setTextColor();
void setFont();
void drawLayout();
void drawText();
void drawText(std::string text, int x, int y, int rgb[3], void* font);


int main(int argc, char** argv) {
    glutInit(&argc, argv);                // Initialization
    glutInitWindowSize(windowWidth, windowHeight);         // Specify a window size
    glutInitWindowPosition(400, 150);     // Specify a window position
    mainWindow = glutCreateWindow("Editor"); // Create a titled window
    drawMenu();
    mainInit();                             // Specify some settings
    glutDisplayFunc(mainDisplayCallback);   // Register a callback
    glutKeyboardFunc(keyboardCallback);   // Register a keyboard callback
    glutMotionFunc(mouseMotionCallback);  // register a mouse motion callback

    glutInitWindowSize(600, 400);         // Specify a window size
    glutInitWindowPosition(1045, 150);     // Specify a window position
    helpWindow = glutCreateWindow("Info");
    drawHelpMenu();
    helpInit();
    glutDisplayFunc(helpDisplayCallback);   // Register a callback

    glutMainLoop();                       // Get into an infinite loop

    return 0;                             // Something wrong happened
}


void mainInit() {
    glClearColor(1, 1, 1, 0);          // Specify a background color: white
	gluOrtho2D(0, windowWidth, windowHeight, 0);	// Specify a viewing area
}

void helpInit() {
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 600, 400, 0);
}


void mainDisplayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);  // Draw the background
    drawMainText();
    drawLayout();
    drawText();
    glFlush();                     // Flush out the buffer contents
}

void helpDisplayCallback() {
    glClear(GL_COLOR_BUFFER_BIT);  // Draw the background
    drawHelpText();
    glFlush();                     // Flush out the buffer contents
}

void keyboardCallback(unsigned char key, int cursorX, int cursorY) {
    //if (userString.length() != 30) {
        userString += key;

        if (!started_typing) {
            cursor_x = cursorX - 200;     // Adjust the x position for the width of the window
            cursor_y = -(cursorY - 200);  // Adjust the y position for the height of the window
            started_typing = true;
        }
        // Backspace or delete pressed
        if (int(key) == 8 || int(key) == 127)
            userString = userString.substr(0, userString.length() - 2);
    //}
    mainDisplayCallback();
}


void setTextColor() {
    if (colorValue == 1) {
        // Green
        text_color[0] = 25;
        text_color[1] = 200;
        text_color[2] = 25;
    }
    else if (colorValue == 2) {
        // Purple
        text_color[0] = 128;
        text_color[1] = 0;
        text_color[2] = 128;
    }
    else if (colorValue == 3) {
        // Red
        text_color[0] = 255;
        text_color[1] = 0;
        text_color[2] = 0;
    }
    else if (colorValue == 4) {
        //Random
        text_color[0] = rand() % 256;
        text_color[1] = rand() % 256;
        text_color[2] = rand() % 256;
    }
}


void setFont() {
    if (fontValue == 5)
        font = GLUT_BITMAP_9_BY_15;
    if (fontValue == 6)
        font = GLUT_BITMAP_HELVETICA_18;
    else if (fontValue == 7)
        font = GLUT_BITMAP_TIMES_ROMAN_24;
}


void mouseMotionCallback(int mousex, int mousey) {
    int line_length = userString.length() * 9;
    if (mousex <= 0)
        cursor_x = -200;
    else if (mousex >= 400 - line_length)
        cursor_x = 200 - line_length;
    else
        cursor_x = mousex - 200;    // Adjust the x position for the width of the window
    if (mousey <= 18)
        cursor_y = 182;
    else if (mousey >= 400)
        cursor_y = -200;
    else
        cursor_y = -(mousey - 200); // Adjust the y position for the height of the window

    mainDisplayCallback();
}


void mainMenuHandler(int num) {
    if (num == 0)
        exit(0);
    else if (num == 1 || num == 2 || num == 3 || num == 4) {
        colorValue = num;
        setTextColor();
    }
    else if (num == 5 || num == 6 || num == 7) {
        fontValue = num;
        setFont();
    }
    else if (num == 8) {
        glutSetWindow(helpWindow);
        glutShowWindow();
        glutSetWindow(mainWindow);
    }
    mainDisplayCallback();
}


void drawMenu() {

    int colorMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Green", 1);
    glutAddMenuEntry("Purple", 2);
    glutAddMenuEntry("Red", 3);
    glutAddMenuEntry("Random", 4);
    int fontMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("9 By 15", 5);
    glutAddMenuEntry("Helvetica", 6);
    glutAddMenuEntry("Times New Roman", 7);
    int mainMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Help", 8);
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


void drawMainText() {
    glColor3ubv(text_color);
    glRasterPos2i(windowPadding + 15, windowPadding + 30);

    for (unsigned i = 0; i < userString.length(); i++)
        glutBitmapCharacter(font, userString[i]);
    
}

void drawHelpText() {
    glColor3ub(0, 0, 0);

    drawText("Type some text to display it in green within the window.", 5, 40);
    drawText("Press 'Backspace' to remove a character", 5, 60);
    drawText("Press the Left Mouse Button and Hold to move the text", 5, 120);

    drawText("Right Mouse Button on the 'Lab 12' window opens this menu:", 5, 160);
    drawText("    'Help' option shows the 'Lab 12: Help' window", 5, 180);
    drawText("    'Color' option opens a submenu to change text color", 5, 200);
    drawText("    'Font' option opens a submenu to change text font", 5, 220);
    drawText("    'Quit' option quits the application", 5, 240);

    drawText("Right Mouse Button on the 'Lab 12: Help' window opens this menu:", 5, 280);
    drawText("    'Close' option hides the Lab 12: Help window", 5, 300);

}

void drawText(std::string input, int x, int y) {
    glRasterPos2i(x, y);

    for (unsigned i = 0; i < input.length(); i++)
        glutBitmapCharacter(helpFont, input[i]);
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

// Draws the border and individual section lines
void drawLayout() {
    // Draws surrounding border
    drawLine(windowPadding, windowPadding, windowWidth - windowPadding, windowPadding, blackColor);  // Top Line
    drawLine(windowPadding, windowHeight - windowPadding, windowWidth - windowPadding, windowHeight - windowPadding, blackColor);  // Bottom Line
    drawLine(windowPadding, windowPadding, windowPadding, windowHeight - windowPadding, blackColor);  // Left Line
    drawLine(windowWidth - windowPadding, windowPadding, windowWidth - windowPadding, windowHeight - windowPadding, blackColor); //Right Line
}

// Draws any text displayed on screen
void drawText() {
    drawText("Page 1 of 1 ", windowPadding + 5, windowHeight - 5, blackColor, helvetica); // Text for counting characters
    drawText("Characters: ", windowPadding + 150, windowHeight - 5, blackColor, helvetica); // Text for counting characters
}

// Texts
void drawText(std::string text, int x, int y, int rgb[3], void* font) {
	draw_text = text;
	glColor3ub(rgb[0], rgb[1], rgb[2]);
	glRasterPos2i(x, y);

	for (size_t i = 0; i < draw_text.length(); i++)
		glutBitmapCharacter(font, draw_text[i]);
}