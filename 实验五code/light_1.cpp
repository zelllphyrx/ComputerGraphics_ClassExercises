#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>

// 初始化场景和光照设置
void init(void)
{
    glClearColor(0, 0, 0, 0); // 设置清屏颜色为黑色
    glEnable(GL_CULL_FACE); // 启用面剔除操作
    glCullFace(GL_BACK);    // 设置剔除背面多边形，减少渲染负担

    // 光源0：方向光设置
    GLfloat light0_ambient[] = { 0.0, 0.0, 0.0, 1.0 };    // 环境光：无色（黑色）
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };    // 漫反射光：白光
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };   // 镜面反射光：白光
    GLfloat light0_position[] = { 1.0, 1.0, 1.0, 0.0 };   // 位置：方向光（最后一个分量为0表示无穷远方向）

    // 设置光源0的各项参数
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

    // 光源1：点光源设置
    GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };    // 环境光：浅灰
    GLfloat light1_diffuse[] = { 1.0, 0.5, 0.0, 1.0 };    // 漫反射光：橙色
    GLfloat light1_specular[] = { 1.0, 0.5, 0.5, 1.0 };   // 镜面反射光：淡红色
    GLfloat light1_position[] = { -1.0, 0.5, 2.0, 1.0 };  // 位置：点光源（最后一个分量为1表示点光源）

    // 设置光源1的各项参数
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    // 光源2：点光源设置
    GLfloat light2_ambient[] = { 0.0, 0.0, 0.2, 1.0 };    // 环境光：蓝色
    GLfloat light2_diffuse[] = { 0.0, 0.0, 1.0, 1.0 };    // 漫反射光：纯蓝色
    GLfloat light2_specular[] = { 0.5, 0.5, 1.0, 1.0 };   // 镜面反射光：淡蓝色
    GLfloat light2_position[] = { 1.0, -1.0, 2.0, 1.0 };  // 位置：点光源（最后一个分量为1表示点光源）

    // 设置光源2的各项参数
    glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light2_specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

    // 启用光照系统及深度测试
    glEnable(GL_LIGHTING);  // 开启光照
    glEnable(GL_LIGHT0);    // 启用光源0（方向光）
    glEnable(GL_LIGHT1);    // 启用光源1（点光源1）
    glEnable(GL_LIGHT2);    // 启用光源2（点光源2）
    glEnable(GL_DEPTH_TEST); // 启用深度测试，确保正确的物体遮挡关系
}

// 调整窗口大小时的回调函数
void resize(int width, int height)
{
    float ar = (float)width / (float)height; // 计算窗口宽高比

    glViewport(0, 0, width, height); // 设置视口大小
    glMatrixMode(GL_PROJECTION);    // 切换到投影矩阵
    glLoadIdentity();               // 重置投影矩阵
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0); // 设置透视投影

    glMatrixMode(GL_MODELVIEW);     // 切换回模型视图矩阵
    glLoadIdentity();               // 重置模型视图矩阵
}

// 渲染场景的回调函数
void display()
{
    double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // 获取程序运行时间，单位为秒
    double a = time * 90.0;                            // 旋转角度，随时间线性增加
    glClearColor(0.0f, 0.34f, 0.57f, 1.0f);           // 设置背景颜色为蓝色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓存

    // 绘制线框球体
    glPushMatrix();
    glTranslatef(-0.3, 0.0, 0.0);  // 平移球体位置
    glTranslated(-0.5, 0.5, -3);   // 进一步平移
    glRotatef(285.0, 1.0, 0.0, 0.0); // 旋转角度
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotated(a, 0, 0, 1);         // 随时间自转
    glutWireSphere(0.5, 20, 16);   // 渲染线框球体，半径0.5
    glPopMatrix();

    // 绘制实心球体
    glPushMatrix();
    glTranslatef(0.7, 0.0, 0.0);   // 平移球体位置
    glTranslated(-0.5, -0.5, -3);  // 进一步平移
    glRotatef(90.0, 1.0, 0.0, 0.0); // 旋转角度
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glRotated(a, 0, 0, 1);         // 随时间自转
    glutSolidSphere(0.5, 30, 30);  // 渲染实心球体，半径0.5
    glPopMatrix();

    glutSwapBuffers();             // 交换前后台缓冲，显示当前帧
}

// 空闲时的回调函数，用于不断刷新屏幕
void idle(void)
{
    glutPostRedisplay(); // 标记当前窗口需要重新绘制
}

// 主函数
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);                          // 初始化GLUT库
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // 设置显示模式：RGB颜色、双缓冲、深度测试
    glutInitWindowPosition(100, 100);               // 设置窗口初始位置
    glutInitWindowSize(500, 500);                   // 设置窗口初始大小
    glutCreateWindow("方向光与点光源");               // 创建窗口并命名

    glutReshapeFunc(resize);                        // 注册窗口大小调整回调函数
    glutDisplayFunc(display);                       // 注册显示回调函数
    glutIdleFunc(idle);                             // 注册空闲回调函数
    init();                                         // 初始化场景和光照设置

    glutMainLoop();                                 // 进入GLUT事件处理循环
    return 0;
}
