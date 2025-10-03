#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>

void init(void)
{
    glClearColor(0, 0, 0, 0); // 背景颜色为黑色
    glEnable(GL_CULL_FACE);// 开启剔除操作效果
    glCullFace(GL_BACK);// 禁用多边形背面光照

    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 }; // {Red, Green, Blue, Alpha}环境光，其中{0.0, 0.0, 0.0}表示环境光被关闭，Alpha为透明度，这里Alpha=1表示完全不透明
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // {Red, Green, Blue, Alpha}散射光，其中{1.0, 1.0, 1.0}表示白光，Alpha为透明度
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 }; // {Red, Green, Blue, Alpha}镜面光，其中{1.0, 1.0, 1.0}表示白光，Alpha为透明度
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 }; // 光源位置，{ X, Y, Z, W }W = 0为方向光，W = 1.0为点光源

    // 光照参数
    // 通过glLightfv函数为光源GL_LIGHT0配置不同的光照属性
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient); // 设置光源的环境光属性
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse); // 设置光源的散射光属性
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); // 设置光源的镜面光属性
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); // 设置光源的位置或方向

    glEnable(GL_LIGHTING); // 全局启用OpenGL的光照模型，激活光照计算
    glEnable(GL_LIGHT0); // 启用光源0（只有启用的光源才会影响场景）
    glEnable(GL_DEPTH_TEST); // 启用深度探测，通过深度缓冲区（z-buffer）记录每个像素的深度值，确定物体的遮挡关系

}

void resize(int width, int height)
{
    float ar = (float)width / (float)height; // 计算窗口的宽高比（aspect ratio），用于保持图像在不同窗口尺寸下的正确比例

    glViewport(0, 0, width, height); // 定义窗口的绘图区域（视口），其中：(0, 0)：视口的左下角坐标；(width, height)：视口的宽度和高度
    glMatrixMode(GL_PROJECTION); // 切换到投影矩阵模式，定义观察空间如何投影到屏幕上
    glLoadIdentity(); // 重置投影矩阵
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0); // 定义一个透视投影矩阵（用于模拟人眼近大远小的观察效果）。left, right，bottom, top：分别为视景体的左右下上边界；near, far：视景体的近、远裁剪面

    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵模式，用于控制物体在场景中的位置、旋转和缩放。以便继续设置物体的变换
    glLoadIdentity(); // 重置模型视图矩阵
}

void display()
{
    // 获取时间信息，计算旋转角度
    double time = glutGet(GLUT_ELAPSED_TIME) / 1000.0; // 获取自程序启动以来的时间（单位为毫秒），并将时间转换为秒（/ 1000.0）
    double a = time * 90.0; // 计算当前时间对应的旋转角度a（每秒旋转 90 度）
    // 清除背景色并进行深度缓冲
    glClearColor(0.0f, 0.34f, 0.57f, 1.0f); // 设置背景颜色为蓝绿色（RGBA）
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色缓冲区（背景色）和深度缓冲区，重置深度信息，为新渲染做准备

    // 绘制线框球体
    glPushMatrix(); // 保存当前模型视图矩阵状态，绘制完成后恢复，以避免后续绘制受到影响
    glTranslatef(-0.3, 0.0, 0.0); // 将球体沿X轴平移-0.3
    glTranslated(-0.5, 0.5, -3); // 左移-0.5，上移0.5，远离视点-3
    glRotatef(285.0, 1.0, 0.0, 0.0); // 绕X轴旋转285度，调整球体的朝向。
    glRotatef(90.0, 1.0, 0.0, 0.0); // 再次绕X轴旋转90度，进一步调整方向
    glRotated(a, 0, 0, 1); // 动态绕Z轴旋转角度a，实现旋转动画
    glutWireSphere(0.5, 20, 16);   // 渲染用线条构成的球体，绘制一个半径为0.5，由20条经线和16条纬线组成的线框球体
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.7, 0.0, 0.0); // 将第二个球体沿X轴平移0.7，调整球体的初始位置
    glTranslated(-0.5, -0.5, -3); // 向左、向下和远离视点移动
    glRotatef(90.0, 1.0, 0.0, 0.0); // 绕X轴旋转90度
    glRotatef(90.0, 1.0, 0.0, 0.0); // 动态绕Z轴旋转角度a，实现旋转动画
    glRotated(a, 0, 0, 1); // 绘制一个半径为0.5，表面由30条经线和30条纬线细分的实心球体
    glutSolidSphere(0.5, 30, 30);// 实心球
    glPopMatrix();

    // glFlush();
    // glFlush()在单缓冲模式下强制完成渲染，但在双缓冲模式中无意义，因此被注释掉
    glutSwapBuffers(); // 双缓冲交换
    // 双缓冲交换：交换前后台缓冲区，显示最新的绘图内容。是双缓冲渲染的关键步骤，用于防止屏幕闪烁。

}

// 空状态回调函数：当程序处于空闲状态时（即没有用户输入如鼠标或键盘操作），idle函数会自动执行
void idle(void)
{
    glutPostRedisplay(); // glutPostRedisplay()通知GLUT框架，当前窗口需要重新绘制，接着会触发之前注册的display回调函数，更新窗口内容
}

// 主函数，初始化GLUT库
int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
    // 这里是设置窗口的显示模式，启用以下：
    // GLUT_RGB：使用 RGB 模式（红绿蓝）显示颜色。
    // GLUT_DOUBLE：启用双缓冲机制，避免屏幕闪烁。
    // GLUT_DEPTH：启用深度缓冲，用于处理3D场景中物体的遮挡关系
    glutInitWindowPosition(100, 100); // 设置窗口位置：左上角坐标为(100, 100)，相对屏幕左上角稍偏移
    glutInitWindowSize(500, 500); // 设置窗口大小：初始宽度和高度为500 x 500像素
    glutCreateWindow("旋转球"); // 创建一个标题为"旋转球"的窗口

    glutReshapeFunc(resize);
    glutDisplayFunc(display); // 用display函数绘制场景内容，需要重绘时（如最小化恢复、窗口切换），调用display
    glutIdleFunc(idle); // 当程序处于空闲状态时调用idle函数，实现持续更新场景（如动画效果），这个函数通常调用glutPostRedisplay触发窗口重新绘制
    init(); // 初始化

    glutMainLoop(); // 启动GLUT主循环
    return 0; // 返回状态码0，程序正常结束
}