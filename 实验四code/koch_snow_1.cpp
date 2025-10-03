#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define C 0.017453
typedef float point[2];
int N; // 迭代次数
point start[3] = { {-0.5,-0.25},{0.5,-0.25},{0,0.25} }; // 三角形左中右三个点坐标
float theta = 0;

// 雪花位置与速度
float snowflake_x = 0.0f;
float snowflake_y = 0.75f;
float snowflake_speed = 0.01f;

void draw_line(point p1, point p2)
{
    glBegin(GL_LINES);
    glVertex2fv(p1);
    glVertex2fv(p2);
    glEnd();
}

void divide_line(point p1, point p2, int n, float theta)
{
    if (n > 0)
    {
        point p[3];
        float dis_x, dis_y, dis;
        dis_x = p2[0] - p1[0];
        dis_y = p2[1] - p1[1];
        dis = sqrt(dis_x * dis_x + dis_y * dis_y) / 3;
        p[0][0] = p1[0] + dis_x / 3;
        p[0][1] = p1[1] + dis_y / 3;
        p[2][0] = p2[0] - dis_x / 3;
        p[2][1] = p2[1] - dis_y / 3;
        p[1][0] = p[0][0] + dis * cos((theta - 60.0) * C);
        p[1][1] = p[0][1] + dis * sin((theta - 60.0) * C);
        divide_line(p1, p[0], n - 1, theta);
        divide_line(p[0], p[1], n - 1, theta - 60);
        divide_line(p[1], p[2], n - 1, theta + 60);
        divide_line(p[2], p2, n - 1, theta);
    }
    else
        draw_line(p1, p2);
}

void draw_snowflake()
{
    glPushMatrix();
    glTranslatef(snowflake_x, snowflake_y, 0.0f); // 平移雪花
    divide_line(start[0], start[1], N, theta);
    divide_line(start[0], start[2], N, theta + 60);
    divide_line(start[2], start[1], N, theta - 60);
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    draw_snowflake();
    glFlush();
}

void update_snowflake(int value)
{
    snowflake_y -= snowflake_speed; // 更新雪花位置

    // 如果雪花到达底部，重置位置
    if (snowflake_y < -1.0f)
    {
        snowflake_y = 0.75f;
        snowflake_x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // 随机生成新的x坐标
    }

    glutPostRedisplay(); // 请求重新绘制
    glutTimerFunc(16, update_snowflake, 0); // 16ms后调用更新函数
}

void myinit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor3f(0, 1.0, 0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 设置视口为[-1,1]范围
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(NULL)); // 初始化随机数种子
    printf("Please input your iterative times:\n");
    scanf("%d", &N);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Falling Koch Snowflake");
    glutDisplayFunc(display);
    myinit();
    glutTimerFunc(16, update_snowflake, 0); // 启动动画更新
    glutMainLoop();
    return 0;
}
