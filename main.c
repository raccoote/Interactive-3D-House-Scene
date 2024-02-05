// AEM1 3874
// AEM2 3902

#include<windows.h>
#include<GL/glu.h>
#include<GL/glut.h>

GLfloat a = 4;
GLfloat b = 100;

GLfloat angle = 0.0;
GLfloat cubeSize = 4;
GLfloat vx = 1.0, vy = 1.0, vz = 1.0;

GLfloat stepSize = 0.1;
GLfloat stepSpin = 2;

int menuID = 1;

void MyInit()
{
    glClearColor(0.9, 0.9, 0.9, 0.9);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50, 50, -50, 50, 1, 300);

    glNewList(1, GL_COMPILE);
    glBegin(GL_POLYGON);
    glVertex3f(-1, 1, 1); // top left
    glVertex3f(1, 1, 1); // top right
    glVertex3f(1, -1, 1); // bottom right
    glVertex3f(-1, -1, 1); // bottom left
    glEnd();
    glEndList();
}


enum MENU_TYPE
{
    MENU_ONE,
    MENU_TWO,
    MENU_EXIT,
};


void createGLUTMenus() {
    glutAddMenuEntry("erwthma 1", MENU_ONE);
    glutAddMenuEntry("erwthma 2", MENU_TWO);
    glutAddMenuEntry("Exit", MENU_EXIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu(int);


void Spin()
{
    angle += stepSpin;
    if (angle >= 360)
        angle -= 360;

    cubeSize += stepSize;
    if (cubeSize >= 2 * a) {
        cubeSize = 2 * a;
        stepSize *= -1;
    }
    else if (cubeSize <= a) {
        cubeSize = a;
        stepSize *= -1;
    }
    glutPostRedisplay();
}


void DrawCube()
{
    // top
    glColor3f(0.99, 0.84, 0.36); // yelloow
    glPushMatrix();
    glTranslatef(0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, -1);
    glCallList(1);
    glPopMatrix();

    // bottom
    glColor3f(0.6, 0.66, 0.91);  // purple
    glPushMatrix();
    glTranslatef(0, -1, 0);
    glRotatef(90, 1, 0, 0);
    glTranslatef(0, 0, -1);
    glCallList(1);
    glPopMatrix();

    // right
    glColor3f(0.92, 0.96, 0.52); // lime
    glPushMatrix();
    glTranslatef(1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, -1);
    glCallList(1);
    glPopMatrix();

    // left
    glColor3f(0.76, 0.92, 0.6);  // green
    glPushMatrix();
    glTranslatef(-1, 0, 0);
    glRotatef(90, 0, 1, 0);
    glTranslatef(0, 0, -1);
    glCallList(1);
    glPopMatrix();

    // front
    glColor3f(0.96, 0.6, 0.5);   // red
    glPushMatrix();
    glTranslatef(0, 0, 1);
    glTranslatef(0, 0, -1);
    glCallList(1);
    glPopMatrix();

    // back
    glColor3f(0.55, 0.82, 0.8);  // blue
    glPushMatrix();
    glTranslatef(0, 0, -1);
    glTranslatef(0, 0, -1);
    glCallList(1);
    glPopMatrix();
}

void Draw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (menuID == 2) {
        glTranslatef(0, 0, (-8 * b) / 10);
        glRotatef(angle, vx, vy, vz);
        glTranslatef(0, 0, -(-8 * b) / 10);
    }

    glTranslatef(0, 0, -b); // position
    if (menuID == 1) {
        glRotatef(angle, vx, vy, vz);
    }
    glScalef(cubeSize, cubeSize, cubeSize);
    glPushMatrix();

    DrawCube();
    glPopMatrix();

    glutSwapBuffers();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(50, 150);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Cube");
    MyInit();

    glutCreateMenu(menu);
    createGLUTMenus();

    glutDisplayFunc(Draw);
    glutIdleFunc(Spin);

    glutMainLoop();
    return 0;
}



void menu(int id)
{
    switch (id)
    {
    case MENU_ONE:
    {
        menuID = 1;
        break;
    }
    case MENU_TWO:
    {
        menuID = 2;
        break;
    }
    case MENU_EXIT:
    {
        exit(0);
        break;
    }
    }
    return;
}