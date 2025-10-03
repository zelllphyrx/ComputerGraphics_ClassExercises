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

GLuint snowflake_display_list; // 用于保存雪花图形的显示列表
bool is_snowflake_created = false; // 标记雪花是否已生成

// 递归绘制Koch曲线
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

// 创建显示列表，仅绘制一次雪花图形
void create_snowflake()
{
    snowflake_display_list = glGenLists(1); // 生成一个显示列表ID
    glNewList(snowflake_display_list, GL_COMPILE); // 开始记录显示列表
    divide_line(start[0], start[1], N, theta);      // 底边
    divide_line(start[0], start[2], N, theta + 60); // 左边
    divide_line(start[2], start[1], N, theta - 60); // 右边
    glEndList(); // 结束显示列表记录
    is_snowflake_created = true; // 标记为已生成
}

void draw_snowflake()
{
    if (!is_snowflake_created) // 如果尚未生成显示列表，先生成
        create_snowflake();

    glPushMatrix();
    glTranslatef(snowflake_x, snowflake_y, 0.0f); // 平移雪花位置
    glCallList(snowflake_display_list); // 调用显示列表，绘制雪花
    glPopMatrix();
}

// 显示函数
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // 清空窗口
    draw_snowflake();             // 绘制雪花
    glFlush();
}

// 更新雪花位置
void update_snowflake(int value)
{
    snowflake_y -= snowflake_speed; // 雪花向下移动

    // 如果雪花超出底部，重置位置并随机生成新x坐标
    if (snowflake_y < -1.0f)
    {
        snowflake_y = 0.75f;
        snowflake_x = ((float)rand() / RAND_MAX) * 2.0f - 1.0f; // 随机x坐标
    }

    glutPostRedisplay(); // 请求重新绘制
    glutTimerFunc(16, update_snowflake, 0); // 16ms后调用更新
}

// 初始化函数
void myinit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // 背景色为白色
    glColor3f(0, 1.0, 0); // 图形颜色为绿色
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0); // 设置二维视口范围
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(NULL)); // 初始化随机数种子
    printf("Please input your iterative times:\n");
    scanf("%d", &N); // 输入迭代次数

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Falling Koch Snowflake");

    glutDisplayFunc(display); // 注册显示回调函数
    myinit();
    glutTimerFunc(16, update_snowflake, 0); // 启动动画更新
    glutMainLoop(); // 进入主循环
    return 0;
}
