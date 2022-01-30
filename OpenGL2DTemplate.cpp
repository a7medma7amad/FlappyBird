//  Includes
#include <stdio.h>
#include <math.h>
#include <random>
#include <glut.h>
#include <time.h>
//-----------------

//  Methods Signatures
void drawRect(int x, int y, int w, int h);
void drawCircle(int x, int y, float r);
void Key(unsigned char key, int x, int y);
void KeyUp(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);
void Display();
//-----------------

//  Global Variables
int windowWidth = 800;
int windowHeight = 800;
int pipeWidth = 50;
int pipeHeight = 350;
int vGapBetweenPipes = 200;
int hGapBetweenPipes = 100;
int xPipeTrans = 0;
bool hasRun = false;
//3 sets so ther can be 6 different sequences
int senarioNumber = -1;
int setsOrder[] = { -1, -1, -1 };
int gravity = 0;
int score = 0;
int lives = 4;
int powerUpSetNum = 1;
bool powerUpActivated = false;
bool checkEnd = false;
bool gameWon = false;
//-----------------
struct pixel {
    GLint x;
    GLint y;
};

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
};
Color getPixelColor(GLint x, GLint y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    return color;
}
void main(int argc, char** argr) {
    glutInit(&argc, argr);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(150, 150);

    glutCreateWindow("Flappy Bird");
    glutDisplayFunc(Display);
    glutKeyboardFunc(Key);      // sets the Keyboard handler function; called when a key is pressed
    glutKeyboardUpFunc(KeyUp);  // sets the KeyboardUp handler function; called when a key is released
    glutMouseFunc(Mouse);       // sets the Mouse handler function; called when a mouse button is clicked
    glutTimerFunc(0, Timer, 0); // sets the Timer handler function; which runs every `Threshold` milliseconds (1st argument)

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);

    glutMainLoop();
}

void print(int x, int y, char* string)
{
    int len, i;

    //set the position of the text in the window using the x and y coordinates
    glRasterPos2f(x, y);

    //get the length of the string to display
    len = (int)strlen(string);

    //loop to display character by character
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
    }
}
// draws rectangles using the (x,y) of the bottom left vertex, width (w) and height (h)
void drawRect(int x, int y, int w, int h) {

    glBegin(GL_QUADS);
    //glColor3f(1.0, 1.0, 1.0);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);

    glEnd();
}
void drawTriBaseDown(int x, int y, int base, int height) {

    glBegin(GL_TRIANGLES);
    //glColor3f(1.0, 0.0, 0.0);
    glVertex2f((x + base), y);
    glVertex2f(x, y);
    glVertex2f((x + (base / 2.0)), y + height);



    glEnd();

}
void drawTriBaseUp(int x, int y, int base, int height) {

    glBegin(GL_TRIANGLES);
    //glColor3f(1.0, 0.0, 0.0);
    glVertex2f((x + base), y);
    glVertex2f(x, y);
    glVertex2f((x + (base / 2.0)), y - height);



    glEnd();

}
void drawTriBaseleft(int x, int y, int base, int height) {

    glBegin(GL_TRIANGLES);

    glVertex2f(x, y);
    glVertex2f(x, y + base);
    glVertex2f((x + height), (y + base / 2.0));



    glEnd();

}

// draws a circle using OpenGL's gluDisk, given (x,y) of its center and tis radius
void drawCircle(int x, int y, float r) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    GLUquadric* quadObj = gluNewQuadric();
    gluDisk(quadObj, 0, r, 50, 50);
    glPopMatrix();
}

// Keyboard handler function

void Key(unsigned char key, int x, int y) {

    glutPostRedisplay();
}

// KeyboardUp handler function
//   similar to the Keyboard handler function except that it is called only when the key is released
void KeyUp(unsigned char key, int x, int y) {
    if (key == 'w') {
        gravity += 50;
    }
    glutPostRedisplay();
}

// Mouse handler function
//   button: the mouse button clicked: left, right or middle
//   state:  the state of the button: clicked (down) or released (up)
//   x  : the X coordinate of the mouse cursor at the moment of clicking the mouse button
//   y  : the Y coordinate of the mouse cursor at the moment of clicking the mouse button
void Mouse(int button, int state, int x, int y) {

    glutPostRedisplay();
}

void Timer(int value) {

    //check for collision

    int x[] = { 0,25,46,0,25,46 ,25,46 };
    int y[] = { (windowHeight / 2.0) + gravity - 1,(windowHeight / 2.0) + gravity - 1,(windowHeight / 2.0) + gravity - 1 ,(windowHeight / 2.0) + gravity + 1 + 45 ,(windowHeight / 2.0) + gravity + 1 + 45 ,(windowHeight / 2.0) + gravity + 1 + 45 ,(windowHeight / 2.0) + gravity + 25,(windowHeight / 2.0) + gravity + 25 };
    bool hasDeducted = false;
    checkEnd = false;

    if (gravity >= (windowHeight / 2.0) || gravity <= ((windowHeight / 2.0) + 45) * -1) {
        lives = 0;
    }


    for (size_t i = 0; i < 6; i++)
    {
        if ((getPixelColor(x[i], y[i]).r == 0.0) && (getPixelColor(x[i], y[i]).g == 0.0) && (getPixelColor(x[i], y[i]).b == 0.0)) {

            checkEnd = true;

            break;
        }
        if ((getPixelColor(x[i], y[i]).r == 0.0) && (getPixelColor(x[i], y[i]).g == 0.0) && (getPixelColor(x[i], y[i]).b == 1.0)) {

            powerUpActivated = true;
            printf("powering up ");
            break;
        }
        else if ((getPixelColor(x[i], y[i]).r == 1.0 && getPixelColor(x[i], y[i]).g == 1.0 && getPixelColor(x[i], y[i]).b == 1.0)
            || (getPixelColor(x[i], y[i]).r == 1.0 && getPixelColor(x[i], y[i]).g == 0.0 && getPixelColor(x[i], y[i]).b == 0.0)) {
            if (gravity < 0) {
                gravity += 100;
            }
            else {
                gravity -= 100;
            }
            if (!hasDeducted) {
                lives -= 1;
                hasDeducted = true;
                break;
            }
        }
    }




    gravity -= 7;
    xPipeTrans = xPipeTrans - 20;
    if (lives > 0) {
        score += 10;
    }

    // ask OpenGL to recall the display function to reflect the changes on the window
    glutPostRedisplay();

    // recall the Timer function after 20 seconds (20,000 milliseconds)
    glutTimerFunc(100, Timer, 0);
}
void drawPairOfPipes(int x, int heightShift, float rR, float rG, float rB, float tR, float tG, float tB) {
    //lower pipe
    glColor3f(rR, rG, rB);
    drawRect(x, 0, pipeWidth, pipeHeight + heightShift);
    glColor3f(tR, tG, tB);
    drawTriBaseUp(x, pipeHeight + heightShift, pipeWidth, pipeWidth);

    //upper pipee
    glColor3f(rR, rG, rB);
    drawRect(x, (pipeHeight + vGapBetweenPipes + heightShift), pipeWidth, pipeHeight - heightShift);
    glColor3f(tR, tG, tB);
    drawTriBaseDown(x, (pipeHeight + vGapBetweenPipes + heightShift), pipeWidth, pipeWidth);


}
void drawAset(int x, int numOfPipes, bool hasPowerUp) {

    for (size_t i = 0; i < numOfPipes; i++)
    {
        int randHeightShift = rand() % 150;
        int randSign = rand() % 2 + 1;
        if (randSign <= 1) {
            randHeightShift = randHeightShift * -1;
        }
        //drawPairOfPipes(x,0);
        x = x + hGapBetweenPipes + pipeWidth;
    }

}
void drawSetOne(int x, int PowerUpSet, float rR, float rG, float rB, float tR, float tG, float tB) {
    glBegin(GL_QUADS);

    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(x, 0);
    glVertex2f(x, windowHeight);
    glVertex2f(x + windowWidth + 500, windowHeight);
    glVertex2f(x + windowWidth + 500, 0);
    glEnd();

    drawPairOfPipes(x + hGapBetweenPipes, 20, rR, rG, rB, tR, tG, tB);

    if (PowerUpSet == 1) {
        //draw powerUp
        glColor3f(0.0, 0.0, 1.0);
        drawCircle(x + hGapBetweenPipes + 30, (windowHeight / 2.0) + 40, 20);
    }

    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 40, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 50, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 80, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 100, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;


}
void drawSetTwo(int x, int PowerUpSet, float rR, float rG, float rB, float tR, float tG, float tB) {
    glBegin(GL_QUADS);

    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(x, 0);
    glVertex2f(x, windowHeight);
    glVertex2f(x + windowWidth + 500, windowHeight);
    glVertex2f(x + windowWidth + 500, 0);
    glEnd();

    drawPairOfPipes(x + hGapBetweenPipes, -20, rR, rG, rB, tR, tG, tB);
    if (PowerUpSet == 2) {
        //draw powerUp
        glColor3f(0.0, 0.0, 1.0);
        drawCircle(x + hGapBetweenPipes + 30, (windowHeight / 2.0) + 40, 20);
    }
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, -40, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, -80, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, -100, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, -50, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 10, rR, rG, rB, tR, tG, tB);



}
void drawSetThree(int x, int PowerUpSet, float rR, float rG, float rB, float tR, float tG, float tB) {
    glBegin(GL_QUADS);

    glColor3f(0.0, 1.0, 1.0);
    glVertex2f(x, 0);
    glVertex2f(x, windowHeight);
    glVertex2f(x + windowWidth + 500, windowHeight);
    glVertex2f(x + windowWidth + 500, 0);
    glEnd();

    drawPairOfPipes(x + hGapBetweenPipes, 10, rR, rG, rB, tR, tG, tB);
    if (PowerUpSet == 3) {
        //draw powerUp
        glColor3f(0.0, 0.0, 1.0);
        drawCircle(x + hGapBetweenPipes + 30, (windowHeight / 2.0) + 40, 20);
    }
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 20, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 50, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 100, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 50, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;
    drawPairOfPipes(x + hGapBetweenPipes, 60, rR, rG, rB, tR, tG, tB);
    x = x + hGapBetweenPipes + pipeWidth;

}
void drawChosenSet(int setNum, int x, int powerUpSet, float rR, float rG, float rB, float tR, float tG, float tB) {
    switch (setNum)
    {
    case 1:
        drawSetOne(x, powerUpSet, rR, rG, rB, tR, tG, tB);
        break;
    case 2:
        drawSetTwo(x, powerUpSet, rR, rG, rB, tR, tG, tB);
        break;
    case 3:
        drawSetThree(x, powerUpSet, rR, rG, rB, tR, tG, tB);
        break;
    default:
        break;
    }
}

void drawBird() {
    glColor3f(1.0, 0.5, 0.5);
    drawRect(0, (windowWidth / 2.0), 45, 45);
    glColor3f(0.0, 1.0, 1.0);
    drawCircle(10, (windowWidth / 2.0) + 30, 7);
    glColor3f(0.0, 0.0, 0.0);
    drawTriBaseleft(25, (windowWidth / 2.0) + 10, 20, 20);

}
void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    //-------------------
    int firstSetXcoordinates = 0;
    int secondSetXcoordinates = windowHeight + 100 + 100;
    int thirdSetXcoordinates = (windowHeight + 100) * 2 + 200;
    if (lives > 0 && ((!checkEnd) || (checkEnd && xPipeTrans > 100))) {
        if (!hasRun) {
            srand(static_cast<unsigned int>(time(nullptr)));
            senarioNumber = rand() % 6;
            srand(static_cast<unsigned int>(time(nullptr)));
            powerUpSetNum = rand() % 3 + 1;
            if (powerUpSetNum > 3) {
                powerUpSetNum = 3;
            }

            switch (senarioNumber)
            {
            case 0://1,2,3
                setsOrder[0] = 1;
                setsOrder[1] = 2;
                setsOrder[2] = 3;
                break;
            case 1://1,3,2
                setsOrder[0] = 1;
                setsOrder[1] = 3;
                setsOrder[2] = 2;
                break;
            case 2://2,1,3
                setsOrder[0] = 2;
                setsOrder[1] = 1;
                setsOrder[2] = 3;
                break;
            case 3://2,3,1
                setsOrder[0] = 2;
                setsOrder[1] = 3;
                setsOrder[2] = 1;
                break;
            case 4://3,1,2
                setsOrder[0] = 3;
                setsOrder[1] = 1;
                setsOrder[2] = 2;
                break;
            case 5://3,2,1
                setsOrder[0] = 3;
                setsOrder[1] = 2;
                setsOrder[2] = 1;
                break;

            }
            hasRun = true;
        }
        printf("%d", powerUpSetNum);

        //first set
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_QUADS);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 3000, 0.0);
        glVertex3f(3000, 3000, 0.0);
        glVertex3f(3000, 0.0, 0.0);
        glEnd();

        glPushMatrix();
        glTranslatef(xPipeTrans, 0, 0);
        if ((powerUpActivated) && setsOrder[0] == powerUpSetNum) {

            drawChosenSet(setsOrder[0], firstSetXcoordinates, powerUpSetNum, 0.7, 0.8, 0.2, 0.1, 0.8, 1.0);
            //printf(" if 1 ");
        }
        else {
            drawChosenSet(setsOrder[0], firstSetXcoordinates, powerUpSetNum, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
            //printf("else");
        }
        //second set
        glPushMatrix();
        if ((powerUpActivated) && setsOrder[1] == powerUpSetNum) {

            drawChosenSet(setsOrder[0], secondSetXcoordinates, powerUpSetNum, 0.7, 0.8, 0.2, 0.1, 0.8, 1.0);
            //printf(" if 2 ");
        }
        else {
            drawChosenSet(setsOrder[1], secondSetXcoordinates, powerUpSetNum, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
            //printf("else");
        }
        //third set
        glPushMatrix();
        if ((powerUpActivated) && setsOrder[2] == powerUpSetNum) {

            drawChosenSet(setsOrder[0], thirdSetXcoordinates, powerUpSetNum, 0.7, 0.8, 0.2, 0.1, 0.8, 1.0);
            //printf(" if 3 ");
        }
        else {
            drawChosenSet(setsOrder[2], thirdSetXcoordinates, powerUpSetNum, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
            //printf("else");
        }
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();

        //the bird

        glPushMatrix();
        glTranslatef(0, gravity, 0);
        drawBird();



        glPopMatrix();


        glColor3f(1, 0, 0);
        char* p0s[20];
        char* p1s[20];
        sprintf((char*)p1s, "Lives: %d", lives);
        print(355, 700, (char*)p1s);
        sprintf((char*)p0s, "Score: %d", score);
        print(345, 680, (char*)p0s);
    }
    else {
        if (lives <= 0) {
            //GameOver
            char* p0s[20];
            char* p1s[20];
            sprintf((char*)p0s, "Score: %d", score);
            print(345, 680, (char*)p0s);
            sprintf((char*)p0s, "Game Over");
            print(345, 700, (char*)p0s);
        }
    }

    //---------------
    glFlush();

}
