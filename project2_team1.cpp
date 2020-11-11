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
 -Alicia Willard        (16.67%; created code)
 DESCRIPTION:           A text editor that renders text entered by a user using keyboard input,
                        then uses menus to change the text's color or font, to open or close
                        an info window, to save the text to a file, and to terminate the program.
 NOTES:                 Must use C:\Temp as the location of the files or the root of the file hierarchy.
 FILES:                 project2_team1.cpp, labProject.sln, freeglut.dll, glut.h, freeglut.lib
 IDE/COMPILER:          Microsoft Visual Studio 2019
 INSTRUCTION FOR COMPILATION AND EXECUTION:
    1.      Double click on labProject.sln          to OPEN the project
    2.      Press Ctrl+F7                           to COMPILE
    3.      Press Ctrl+F5                           to EXECUTE
    4.      Type some text in the 'Editor' window   to DISPLAY the text in black
    5.      Right-click in the 'Editor' window      to OPEN the 'Editor' menu
            then select 'Info'                      to DISPLAY the 'Info' window
    OR      select 'Color'                          to CHANGE the text color
    OR      select 'Font'                           to CHANGE the text font
    OR      select 'Page Navigation'                to GO back or forward a page
    OR      select 'Save'                           to SAVE the text as a file
    OR      select 'Quit'                           to TERMINATE the program
    6.      Right-click in the 'Info' window        to OPEN the 'Info' menu
            then select 'Close'                     to HIDE the 'Info' window
==================================================================================================*/
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <GL/glut.h>

using namespace std;

// Misc Variables
int fontValue;
int colorValue;
int backgroundColorValue;
int entered = 0;
int start_x = 0;
int start_y = 0;
bool can_change_start = true;

// Line Variables
GLfloat lineScale = 1;
string draw_text;
int blackColor[3] = { 0, 0, 0 };

// Window Variables
int mainWindow, helpWindow;
GLint windowPadding = 20;
GLint windowWidth = 550;
GLint windowHeight = 775;

// Text and Font Variables
void* helvetica = GLUT_BITMAP_HELVETICA_18;
void* roman = GLUT_BITMAP_TIMES_ROMAN_24;
void* font = roman;
void* helpFont = helvetica;
void* axesFont = roman;
vector<int> text_color = { 0, 0, 0 };

// Page Variables
vector<string> strings;
vector<void*> fonts;
vector<vector<int>> colors;
int currentIndex = 0;
int currentPage = 0;
int totalPages = 1;
unsigned int maxCharacters = 30;
unsigned int maxLines = 30;

void mainInit();
void helpInit();
void mainDisplayCallback();
void helpDisplayCallback();
void specialCallback(int key, int x, int y);
void keyboardCallback(unsigned char key, int cursorX, int cursorY);
void mouseCallback(int button, int state, int x, int y);
void mainMenuHandler(int num);
void drawMenu();
void helpMenuHandler(int num);
void drawHelpMenu();
void drawEditorText();
void drawHelpText();
void setTextColor();
void setFont();
void setBackgroundColor();
void drawLayout();
void drawTextLayout();
void drawCustomText(string text, int x, int y, int rgb[3], void* font);
void saveFile();
int getTotalLines();
int getTotalCharacters();


int main(int argc, char** argv) {
    // Initialize text editor variables
    strings.push_back("");
    fonts.push_back(font);
    colors.push_back(text_color);

    std::cout << "Must use C:\\Temp as the location of the files or the root of the file hierarchy.";
    std::cout << "\n" << std::endl;

    glutInit(&argc, argv);                          // Initialization
    glutInitWindowSize(windowWidth, windowHeight);  // Specify a window size
    glutInitWindowPosition(300, 10);                // Specify a window position
    mainWindow = glutCreateWindow("Editor");        // Create a titled window
    drawMenu();
    mainInit();                                     // Specify some settings
    glutDisplayFunc(mainDisplayCallback);           // Register a callback
    glutKeyboardFunc(keyboardCallback);             // Register a keyboard callback
    glutSpecialFunc(specialCallback);
    glutMouseFunc(mouseCallback);
    glutInitWindowSize(575, 350);                   // Specify a window size
    glutInitWindowPosition(855, 213);               // Specify a window position
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
    gluOrtho2D(0, 575, 350, 0);
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
    if (button == 0 && state == 0) {  // Set the starting text position
        if (x > windowPadding + 15 && start_x == 0 && can_change_start) {
            if (y > windowPadding + 30 && start_y == 0 && can_change_start) {
                start_x = x;
                start_y = y;
                can_change_start = false;
            }
        }
    }
}


void specialCallback(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT)  // Go forward a page
        currentPage += 1;
    else if (key == GLUT_KEY_LEFT) {  // Go backward a page
        currentPage -= 1;

        if (currentPage == -1)
            currentPage = 0;
    }
    mainDisplayCallback();
}


void keyboardCallback(unsigned char key, int cursorX, int cursorY) {
    if (int(key) == 8 || int(key) == 127) {  // Backspace or delete pressed
        strings[currentIndex] = strings[currentIndex].substr(0, strings[currentIndex].length() - 1);
        if (getTotalCharacters() == 0) {  // Reset Starting text position
            start_x = 0;
            start_y = 0;
            can_change_start = true;
        }
        if (strings[currentIndex].length() == 0 && currentIndex != 0) {  // Remove a styling entry if it's length is zero
            strings.pop_back();
            fonts.pop_back();
            colors.pop_back();
            currentIndex -= 1;
        }
    }
    if (int(key) != 8 && int(key) != 127) {
        strings[currentIndex] += key;
        currentPage = totalPages - 1;
        if (can_change_start) {
            can_change_start = false;
        }
    }
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
    strings.push_back("");
    fonts.push_back(font);
    colors.push_back(text_color);
    currentIndex += 1;
}


void setFont() {
    if (fontValue == 6)
        font = GLUT_BITMAP_8_BY_13;
    if (fontValue == 7)
        font = GLUT_BITMAP_9_BY_15;
    else if (fontValue == 8)
        font = GLUT_BITMAP_TIMES_ROMAN_10;
    else if (fontValue == 16)
        font = GLUT_BITMAP_TIMES_ROMAN_24;
    else if (fontValue == 17)
        font = GLUT_BITMAP_HELVETICA_10;
    else if (fontValue == 18)
        font = GLUT_BITMAP_HELVETICA_12;
    else if (fontValue == 19)
        font = GLUT_BITMAP_HELVETICA_18;

    strings.push_back("");
    fonts.push_back(font);
    colors.push_back(text_color);
    currentIndex += 1;
}

void setBackgroundColor() {
    if (backgroundColorValue == 9) {
        glClearColor(0.44f, 0.5f, 0.56f, 1);
    }
    else if (backgroundColorValue == 10) {
        glClearColor(0.29f, 0.61f, 0.83f, 1);
    }
    else if (backgroundColorValue == 11) {
        glClearColor(1, 1, 1, 0);
    }
}

void mainMenuHandler(int num) {
    if (num == 15)
        saveFile();
    else if (num == 0) {
        saveFile();
        exit(0);
    }
    else if (num == 1 || num == 2 || num == 3 || num == 4 || num == 5) {
        colorValue = num;
        setTextColor();
    }
    else if (num == 6 || num == 7 || num == 8 || num == 16 || num == 17 || num == 18 || num == 19) {
        fontValue = num;
        setFont();
    }
    else if (num == 9 || num == 10 || num == 11) {
        backgroundColorValue = num;
        setBackgroundColor();
    }
    else if (num == 12) {
        glutSetWindow(helpWindow);
        glutShowWindow();
        glutSetWindow(mainWindow);
    }
    else if (num == 13) {
        currentPage -= 1;

        if (currentPage == -1)
            currentPage = 0;
    }
    else if (num == 14)
        currentPage += 1;

    mainDisplayCallback();
}


void drawMenu() {
    int navMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Go back 1 page", 13);
    glutAddMenuEntry("Go forward 1 page", 14);
    int colorMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Black", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Purple", 3);
    glutAddMenuEntry("Red", 4);
    glutAddMenuEntry("Random", 5);
    int fontMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("8 by 13", 6);
    glutAddMenuEntry("9 by 15", 7);
    glutAddMenuEntry("Times New Roman", 8);
    glutAddMenuEntry("Times New Roman Large", 16);
    glutAddMenuEntry("Helvetica Small", 17);
    glutAddMenuEntry("Helvetica Medium", 18);
    glutAddMenuEntry("Helvetica Large", 19);
    int backgroundMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Grey", 9);
    glutAddMenuEntry("Blue", 10);
    glutAddMenuEntry("White", 11);
    int mainMenuId = glutCreateMenu(mainMenuHandler);
    glutAddMenuEntry("Info", 12);
    glutAddSubMenu("Font", fontMenuId);
    glutAddSubMenu("Text Color", colorMenuId);
    glutAddSubMenu("Background Color", backgroundMenuId);
    glutAddSubMenu("Page Navigation", navMenuId);
    glutAddMenuEntry("Save", 15);
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


void drawEditorText() {
    int line = 0;           // Tracks the local line in a page
    int overall_line = 0;   // Tracks the current overall line in document
    int page = 0;           // The local page
    totalPages = 1;
    int x, y;               // X and Y raster position coordinates

    if (start_x == 0 && start_y == 0) {
        x = windowPadding + 15;
        y = windowPadding + 30;
    }
    else {
        x = start_x;
        y = start_y;
    }

    for (size_t ind = 0; ind < strings.size(); ind++) {
        int r = colors[ind][0];
        int g = colors[ind][1];
        int b = colors[ind][2];
        void* f = fonts[ind];

        for (unsigned i = 0; i < strings[ind].length(); i++) {
            if (int(strings[ind][i]) == 13) {  // Enter key character found, then move to new line
                line += 1;
                overall_line += 1;
                x = windowPadding + 15;

                if (start_x == 0 && start_y == 0 || page != 0)
                    y = windowPadding + 30 + (line * 24);
                else
                    y = start_y + (line * 24);
                continue;
            }
            if (x >= windowWidth - 50) {  // X coordinate limit reached, move to a new line
                line += 1;
                overall_line += 1;
                x = windowPadding + 15;

                if (start_x == 0 && start_y == 0 || page != 0)
                    y = windowPadding + 30 + (line * 24);
                else
                    y = start_y + (line * 24);
            }
            if (y >= windowHeight - 20) {  // Y coordinate limit reached, move to a new page
                page += 1;
                totalPages += 1;
                overall_line += 1;
                line = 0;
                x = windowPadding + 15;
                y = windowPadding + 30 + (line * 24);
            }
            if (page == currentPage) {  // Only render text that belongs to the currentPage
                glColor3ub(r, g, b);
                glRasterPos2i(x, y);
                glutBitmapCharacter(f, strings[ind][i]);
            }
            x += glutBitmapWidth(f, strings[ind][i]);
        }
    }
}


// Gets the number of lines entered by the user
int getTotalLines() {
    int overall_lines = 0;
    int line = 0;
    int x = windowPadding + 15;
    int y = windowPadding + 30;

    for (size_t ind = 0; ind < strings.size(); ind++) {
        void* f = fonts[ind];

        for (unsigned i = 0; i < strings[ind].length(); i++) {
            if (int(strings[ind][i]) == 13) {  // Enter key
                line += 1;
                overall_lines += 1;
                x = windowPadding + 15;
                y = windowPadding + 30 + (line * 24);
                continue;
            }
            if (x > windowWidth - 50) {
                line += 1;
                overall_lines += 1;
                x = windowPadding + 15;
                y = windowPadding + 30 + (line * 24);
            }
            x += glutBitmapWidth(f, strings[ind][i]);
        }
    }
    return overall_lines;
}


// Gets the number of characters entered by the user
int getTotalCharacters() {
    int chars = 0;

    for (size_t ind = 0; ind < strings.size(); ind++)
        chars += strings[ind].length();

    return chars;
}


// Saves the contents of the what the user has entered to a file
void saveFile() {
    cout << "Saving file to C:\\Temp\\typed.txt\n";
    ofstream out("C:/Temp/typed.txt");

    for (size_t ind = 0; ind < strings.size(); ind++)
        out << strings[ind];

    out.close();
}


// Texts
void drawCustomText(string text, int x, int y, int rgb[3], void* font) {
    draw_text = text;
    glColor3ub(rgb[0], rgb[1], rgb[2]);
    glRasterPos2i(x, y);

    for (size_t i = 0; i < draw_text.length(); i++)
        glutBitmapCharacter(font, draw_text[i]);
}


void drawHelpText() {
    drawCustomText("Click somewhere in the 'Editor' window to start typing.", 5, 40, blackColor, helpFont);
    drawCustomText("Type some text to display it in black.", 5, 60, blackColor, helpFont);
    drawCustomText("Press the backspace key to remove a character.", 5, 80, blackColor, helpFont);
    drawCustomText("Press the 'Enter' key to start typing on the next row.", 5, 100, blackColor, helpFont);

    drawCustomText("Right-clicking in the 'Editor' window opens this menu:", 5, 140, blackColor, helpFont);
    drawCustomText("    'Info' displays the 'Info' window", 5, 160, blackColor, helpFont);
    drawCustomText("    'Font' opens a submenu to change text font", 5, 180, blackColor, helpFont);
    drawCustomText("    'Text Color' opens a submenu to change text color", 5, 200, blackColor, helpFont);
    drawCustomText("    'Background Color' opens a submenu to change background color", 5, 220, blackColor, helpFont);
    drawCustomText("    'Page Navigation' opens a submenu to go back or forward a page", 5, 240, blackColor, helpFont);
    drawCustomText("    'Save' saves the text as a file to C:\\Temp\\typed.txt", 5, 260, blackColor, helpFont);
    drawCustomText("    'Quit' terminates the program", 5, 280, blackColor, helpFont);

    drawCustomText("Right-clicking in the 'Info' window opens this menu:", 5, 320, blackColor, helpFont);
    drawCustomText("    'Close' hides the 'Info' window", 5, 340, blackColor, helpFont);
}


// Draws any text displayed on screen
void drawTextLayout() {
    string page = "Page " + to_string(currentPage + 1);
    drawCustomText(page, windowPadding + 5, windowHeight - 5, blackColor, helvetica);
    drawCustomText("Lines: " + to_string(getTotalLines() + 1), windowPadding + 215, windowHeight - 5, blackColor, helvetica);
    drawCustomText("Characters: " + to_string(getTotalCharacters()), windowPadding + 375, windowHeight - 5, blackColor, helvetica);
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