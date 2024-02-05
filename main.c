#include<windows.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846


float _sun_angle = PI;
float _sun_diffuse = 0.3;

int manyPolygons = 1;
int spotlightOn = 1;

// Camera position
float camera_angle = 0.0f;
float camera_radius = 70.0f;
float camera_height = 40.0f;

GLfloat v[][3] = {
        {0.0, 0.0, 1.0},
        {0.0, 0.942809, -0.33333},
        {-0.816497, -0.471405, -0.333333},
        {0.816497, -0.471405, -0.333333} };

typedef struct _vector3 {
    GLfloat x, y, z;
} Vector3, Point3;

void menu(int id)
{

    switch (id)
    {
    case 1:
    {
        manyPolygons = 0;
        break;
    }
    case 2:
    {
        manyPolygons = 1;
        break;
    }
    case 3:
    {
        spotlightOn = 1;
        break;
    }
    case 4:
    {
        spotlightOn = 0;
        break;
    }
    case 5:
    {
        glShadeModel(GL_SMOOTH);
        break;
    }
    case 6:
    {
        glShadeModel(GL_FLAT);
        break;
    }
    case 7:
    {
        exit(0);
        break;
    }
    }
    glutPostRedisplay();
}


void createGLUTMenus() {
    int polygonMenuID = glutCreateMenu(menu);
    glutAddMenuEntry("Low", 1);
    glutAddMenuEntry("High", 2);
    int spotlightMenuID = glutCreateMenu(menu);
    glutAddMenuEntry("On", 3);
    glutAddMenuEntry("Off", 4);
    int shadeMenuID = glutCreateMenu(menu);
    glutAddMenuEntry("Smooth", 5);
    glutAddMenuEntry("Flat", 6);
    int menuID = glutCreateMenu(menu);
    glutAddSubMenu("Polygon", polygonMenuID);
    glutAddSubMenu("Spotlight", spotlightMenuID);
    glutAddSubMenu("Shade", shadeMenuID);
    glutAddMenuEntry("Exit", 7);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void updateScene() {
    _sun_angle -= 0.001;
    if (_sun_angle < 0) {
        _sun_angle = PI;
    }

    float slope = 1.4 / PI; // (0.3 - 1) / (PI/2)
    if (_sun_angle <= PI / 2.0) {
        _sun_diffuse = slope * _sun_angle + 0.3; // line equation
    }
    else {
        _sun_diffuse = -slope * _sun_angle + 1.7;
    }
    glutPostRedisplay();
}

void rotateScene(int value) {
    camera_angle = camera_angle + value;
    if (camera_angle > 360) {
        camera_angle -= 360;
    }
    glutPostRedisplay();
}
void keyPressed(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_RIGHT:
        rotateScene(10);
        break;
    case GLUT_KEY_LEFT:
        rotateScene(-10);
        break;
    }
}

void MyInit() {
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LIGHT0); // sun
    glEnable(GL_LIGHT1); // spotlight
}
void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

Vector3* calculateSurfaceNormal(Vector3* n, Point3* p0, Point3* p1, Point3* p2) {
    Vector3 v1[] = { p2->x - p1->x, p2->y - p1->y, p2->z - p1->z };
    Vector3 v2[] = { p0->x - p1->x, p0->y - p1->y, p0->z - p1->z };

    n->x = v1->y * v2->z - v1->z * v2->y;
    n->y = v1->z * v2->x - v1->x * v2->z;
    n->z = v1->x * v2->y - v1->y * v2->x;

    return n;
}

void drawHouse() {

    Vector3 normal = { 0, 0, 0 };

    /* walls */
    GLfloat wall_diffuse[] = { 0.8, 0.1, 0.1 };
    GLfloat wall_specular[] = { 0, 0, 0 };
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, wall_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, wall_specular);

    /*front rectangle*/
    Point3 front_rect[] = { {-5, 0, 10}, {5, 0, 10}, {5, 10, 10}, {-5, 10, 10} };
    glBegin(GL_POLYGON);
    glNormal3fv(calculateSurfaceNormal(&normal, &front_rect[0], &front_rect[1], &front_rect[2]));
    glVertex3fv(&front_rect[0]);
    glVertex3fv(&front_rect[1]);
    glVertex3fv(&front_rect[2]);
    glVertex3fv(&front_rect[3]);
    glEnd();

    /* back rectangle */
    Point3 back_rect[] = { {-5, 0, -10}, {-5, 10, -10}, {5, 10, -10}, {5, 0, -10} };
    glBegin(GL_POLYGON);
    glNormal3fv(calculateSurfaceNormal(&normal, &back_rect[0], &back_rect[1], &back_rect[2]));
    glVertex3fv(&back_rect[0]);
    glVertex3fv(&back_rect[1]);
    glVertex3fv(&back_rect[2]);
    glVertex3fv(&back_rect[3]);
    glEnd();

    /* left rectangle */
    Point3 left_rect[] = { {-5, 0, 10}, {-5, 10, 10}, {-5, 10, -10}, {-5, 0, -10} };
    glBegin(GL_POLYGON);
    glNormal3fv(calculateSurfaceNormal(&normal, &left_rect[0], &left_rect[1], &left_rect[2]));
    glVertex3fv(&left_rect[0]);
    glVertex3fv(&left_rect[1]);
    glVertex3fv(&left_rect[2]);
    glVertex3fv(&left_rect[3]);
    glEnd();

    /* right rectangle */
    Point3 right_rect[] = { {5, 0, 10}, {5, 0, -10}, {5, 10, -10}, {5, 10, 10} };
    glBegin(GL_POLYGON);
    glNormal3fv(calculateSurfaceNormal(&normal, &right_rect[0], &right_rect[1], &right_rect[2]));
    glVertex3fv(&right_rect[0]);
    glVertex3fv(&right_rect[1]);
    glVertex3fv(&right_rect[2]);
    glVertex3fv(&right_rect[3]);
    glEnd();

    /* roof */
    GLfloat roof_diffuse[] = { .5, .5, .5 };
    GLfloat roof_specular[] = { 0.5, 0.5, 0.5 };
    glMaterialf(GL_FRONT, GL_SHININESS, 100);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, roof_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, roof_specular);

    /* front triangle(roof) */
    Point3 front_tr[] = { {0, 18.66, 10}, { -5, 10, 10}, {5, 10, 10} };
    glBegin(GL_TRIANGLES);
    glNormal3fv(calculateSurfaceNormal(&normal, &front_tr[0], &front_tr[1], &front_tr[2]));
    glVertex3fv(&front_tr[0]); // (roof height is 5 * sqrt(3)) = 18.66, due to Pythagorean theorem in the roof triangle
    glVertex3fv(&front_tr[1]);
    glVertex3fv(&front_tr[2]);
    glEnd();

    /* back triangle(roof)  */
    Point3 back_tr[] = { {0, 18.66, -10}, {5, 10, -10}, { -5, 10, -10} };
    glBegin(GL_TRIANGLES);
    glNormal3fv(calculateSurfaceNormal(&normal, &back_tr[0], &back_tr[1], &back_tr[2]));
    glVertex3fv(&back_tr[0]);
    glVertex3fv(&back_tr[1]);
    glVertex3fv(&back_tr[2]);
    glEnd();

    /* left roof */
    Point3 left_roof[][4] = { {-5, 10, 10}, {0, 18.66, 10}, {0, 18.66, -10}, {-5, 10, -10} };
    glBegin(GL_POLYGON);
    glNormal3fv(calculateSurfaceNormal(&normal, &left_roof[0], &left_roof[1], &left_roof[2]));
    glVertex3fv(&left_roof[0]);
    glVertex3fv(&left_roof[1]);
    glVertex3fv(&left_roof[2]);
    glVertex3fv(&left_roof[3]);
    glEnd();

    /* right roof */
    Point3 right_roof[][4] = { {5, 10, 10}, {5, 10, -10}, {0, 18.66, -10}, {0, 18.66, 10} };
    glBegin(GL_POLYGON);
    glNormal3fv(calculateSurfaceNormal(&normal, &right_roof[0], &right_roof[1], &right_roof[2]));
    glVertex3fv(&right_roof[0]);
    glVertex3fv(&right_roof[1]);
    glVertex3fv(&right_roof[2]);
    glVertex3fv(&right_roof[3]);
    glEnd();

    /*grass */
    GLfloat grass_diffuse[] = { 0, 1, 0 };
    GLfloat grass_spec[] = { 0.0, 0.0, 0.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, grass_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, grass_spec);
    glMaterialf(GL_FRONT, GL_SHININESS, 0);
    Point3 grass[] = { {40, 0, 40}, {40, 0, -40}, {-40, 0, -40}, {-40, 0, 40} };
    calculateSurfaceNormal(&normal, &grass[0], &grass[1], &grass[2]); // it's the same for every vertex of grass

    if (manyPolygons == 0) {
        glBegin(GL_POLYGON);
        glNormal3fv(&normal);
        glVertex3fv(&grass[0]);
        glVertex3fv(&grass[1]);
        glVertex3fv(&grass[2]);
        glVertex3fv(&grass[3]);
        glEnd();
    }
    else {  // manyPolygons==1, many polygons(100)
        Point3 pivot = { grass[0].x, grass[0].y, grass[0].z }; // pivot is the lower right corner of every square

        // break the big 80x80 square to 100 smaller squares with 8x8 sides
        for (int i = 0; i < 100; i++) {
            Point3 small_grass[] = {
                { pivot.x, pivot.y, pivot.z },
                { pivot.x, pivot.y, pivot.z - 8}, // step = 80/10 because 80 is the length of the square
                { pivot.x - 8, pivot.y, pivot.z - 8 },
                { pivot.x - 8, pivot.y, pivot.z }
            };

            if (i % 10 == 9) { // pivot needs to go right and up
                pivot.x = 40;
                pivot.z -= 8;
            }
            else { // go left by 8 points
                pivot.x -= 8;
            }
            glBegin(GL_POLYGON);
            glNormal3fv(&normal);
            glVertex3fv(&small_grass[0]);
            glVertex3fv(&small_grass[1]);
            glVertex3fv(&small_grass[2]);
            glVertex3fv(&small_grass[3]);
            glEnd();
        }
    }
}
// normalize a vector to the unit length
void normalize(GLfloat v[3]) {
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (d == 0.0) return;
    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

void divideTriangle(GLfloat a[3], GLfloat b[3], GLfloat c[3], int iterations) {
    if (iterations == 0) {
        Vector3 normal = { 0.0, 0.0, 0.0 };
        glBegin(GL_TRIANGLES);
        //glNormal3fv(calculateSphereNormal(&normal, a));
        glNormal3fv(a);
        glVertex3fv(a);
        glNormal3fv(b);
        glVertex3fv(b);
        glNormal3fv(c);
        glVertex3fv(c);
        glEnd();
        return;
    }

    // calculate midpoints of the sides
    GLfloat ab[3] = { (a[0] + b[0]) / 2, (a[1] + b[1]) / 2, (a[2] + b[2]) / 2 };
    GLfloat ac[3] = { (a[0] + c[0]) / 2, (a[1] + c[1]) / 2, (a[2] + c[2]) / 2 };
    GLfloat bc[3] = { (b[0] + c[0]) / 2, (b[1] + c[1]) / 2, (b[2] + c[2]) / 2 };

    // normalize the midpoints to the unit sphere
    normalize(ab);
    normalize(ac);
    normalize(bc);

    // recurse on the four new triangles
    divideTriangle(a, ab, ac, iterations - 1);
    divideTriangle(c, ac, bc, iterations - 1);
    divideTriangle(b, bc, ab, iterations - 1);
    divideTriangle(ab, bc, ac, iterations - 1);
}

void drawSphere() {
    GLfloat sun_color[] = { 0.91, 0.57, 0.0 };
    GLfloat zero[] = { 0.0, 0.0, 0.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_color);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);  // zero shininess

    divideTriangle(v[0], v[1], v[2], 4);
    divideTriangle(v[3], v[2], v[1], 4);
    divideTriangle(v[0], v[3], v[1], 4);
    divideTriangle(v[0], v[2], v[3], 4);

    glMaterialfv(GL_FRONT, GL_EMISSION, zero);
}

void spotlight() {
    GLfloat light_position[] = { 0.0f, 19.0f, 10.0f, 1.0f }; // Position of the light
    GLfloat light_direction[] = { 0.0, -1.0, 0.0 };
    GLfloat light_cutoff = 30.0;
    GLfloat light_color[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white

    glLightfv(GL_LIGHT1, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, light_cutoff);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_color);
}
void drawAndSetLights() {
    GLfloat sun_pos[] = { 50 * cos(_sun_angle), 50 * sin(_sun_angle), 0.0, 1.0 };
    GLfloat sun_color[] = { _sun_diffuse, _sun_diffuse, _sun_diffuse };
    GLfloat sun_ambient[] = { 0.0, 0.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, sun_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);
    glPushMatrix();
    glTranslatef(sun_pos[0], sun_pos[1], sun_pos[2]);
    drawSphere();
    glPopMatrix();
    if (spotlightOn == 1) {
        glEnable(GL_LIGHT1);
        spotlight();
    }
    else {
        glDisable(GL_LIGHT1);
    }
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    
    float cameraX = camera_radius * sin(camera_angle * PI / 180.0);
    float cameraZ = camera_radius * cos(camera_angle * PI / 180.0);
    gluLookAt(cameraX, camera_height, cameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glScalef(0.6, 0.6, 0.6);
    glPushMatrix();

    drawAndSetLights();
    drawHouse();

    glFlush();
    glPopMatrix();
    glutSwapBuffers();

}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutCreateWindow("My House");
    MyInit();

    createGLUTMenus();

    glutDisplayFunc(drawScene);
    glutIdleFunc(updateScene);
    glutSpecialFunc(keyPressed);
    glutReshapeFunc(handleResize);
    glutMainLoop();
    return 0;
}
