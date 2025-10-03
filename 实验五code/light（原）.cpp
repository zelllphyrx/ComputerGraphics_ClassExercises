#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>

void init(void)
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_CULL_FACE);//开启剔除操作效果
    glCullFace(GL_BACK);//禁用多边形背面光照

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

    //光照参数
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

}

void resize(int width, int height)
{
    float ar = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    double a = time * 90.0;
    glClearColor(0.0f, 0.34f, 0.57f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    glTranslatef(-0.3, 0.0, 0.0);
    glTranslated(-0.5, 0.5, -3);
    glRotatef(285.0, 1.0, 0.0, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotated(a, 0, 0, 1);
    glutWireSphere(0.5, 20, 16);   //渲染用线条构成的球体
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7, 0.0, 0.0);
    glTranslated(-0.5, -0.5, -3);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotated(a, 0, 0, 1);
    glutSolidSphere(0.5, 30, 30);//实心球
    glPopMatrix();

    //glFlush();
    glutSwapBuffers();
}
void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("旋转球");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    init();

    glutMainLoop();
    return 0;
}