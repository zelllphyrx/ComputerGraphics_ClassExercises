#include <windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/glut.h>

#define PI 3.1415
#define MAX_POINT 21    //允许的最大控制点个数
#define SCREEN_WIDTH 640    //屏幕的尺寸
#define SCREEN_HEIGHT 480

//定义存储控制点坐标的结构体
class Point {
public:
    float x, y;
    void position(float x1, float y1) //设置控制点坐标
    {
        x = x1;
        y = y1;
    }
};

int POINT_NUM = 0; //绘制三角形的点的个数
Point triangles[3]; //存储三角形的三个顶点

//初始化
void myInit() {
    glClearColor(1.0, 1.0, 1.0, 1.0); //清空颜色缓存
    glColor3f(0.0, 0.0, 0.0); //设置颜色信息
    glPointSize(4.0); //设置点尺寸
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}

//画出控制点
void drawPoint(Point pt) {
    glBegin(GL_POINTS);    //绘制控制点
    glVertex2f(pt.x, pt.y);
    glEnd();
    glFlush();
}

//画出三角形
void drawTriangle(Point p1, Point p2, Point p3) {
    glBegin(GL_TRIANGLES); //绘制三角形
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glEnd();
    glFlush();
}

//平移和旋转三角形
void transformTriangle(Point& p1, Point& p2, Point& p3, float tx, float ty, float angle) {
    float rad = angle * PI / 180.0; //角度转弧度
    float cosA = cos(rad), sinA = sin(rad);

    //平移矩阵
    p1.x += tx; p1.y += ty;
    p2.x += tx; p2.y += ty;
    p3.x += tx; p3.y += ty;

    //旋转矩阵
    float x1 = p1.x * cosA - p1.y * sinA;
    float y1 = p1.x * sinA + p1.y * cosA;
    float x2 = p2.x * cosA - p2.y * sinA;
    float y2 = p2.x * sinA + p2.y * cosA;
    float x3 = p3.x * cosA - p3.y * sinA;
    float y3 = p3.x * sinA + p3.y * cosA;

    p1.position(x1, y1);
    p2.position(x2, y2);
    p3.position(x3, y3);
}

//鼠标控制
void myMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { //鼠标左键点击三次确定三个控制点
        if (POINT_NUM < 3) {
            triangles[POINT_NUM].position((float)x, (float)(SCREEN_HEIGHT - y));
            drawPoint(triangles[POINT_NUM]);
            POINT_NUM++;
            if (POINT_NUM == 3) { //当获得三个点后，绘制三角形
                drawTriangle(triangles[0], triangles[1], triangles[2]);
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { //鼠标右键完成对三角形的平移与旋转
        if (POINT_NUM == 3) { //确保三角形已经绘制
            glClear(GL_COLOR_BUFFER_BIT); //清空当前屏幕
            float tx = 50.0; //平移x方向距离
            float ty = 30.0; //平移y方向距离
            float angle = 30.0; //旋转角度
            transformTriangle(triangles[0], triangles[1], triangles[2], tx, ty, angle); //平移旋转三角形
            drawTriangle(triangles[0], triangles[1], triangles[2]); //绘制变换后的三角形
        }
    }
    if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) { //鼠标中键清空当前屏幕
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        POINT_NUM = 0; //清空点计数
    }
}

//显示
void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("三角形平移与旋转");
    glutMouseFunc(myMouse);
    glutDisplayFunc(myDisplay);
    myInit();
    glutMainLoop();
    return 0;
}

