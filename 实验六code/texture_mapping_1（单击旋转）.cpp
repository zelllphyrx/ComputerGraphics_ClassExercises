#define _CRT_SECURE_NO_WARNINGS
#define BITMAP_ID 0x4D42     // 位图标识ID
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<gl/glut.h>

float angleX = 0.0; // 绕X轴旋转角度
float angleY = 0.0; // 绕Y轴旋转角度
float angleZ = 0.0; // 绕Z轴旋转角度
BITMAPINFOHEADER bitmapinfoHeader; // 位图头部
unsigned char* bitmapData;         // 纹理数据
unsigned int  texture;             // 纹理对象

// 加载位图文件的函数
unsigned char* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapinfoHeader)
{
    FILE* filePtr;   // 文件指针
    BITMAPFILEHEADER bitmapFileHeader; // 位图文件头
    unsigned char* bitmapImage;  // 位图图像数据
    int imageIdx = 0;   // 位图索引计数器
    unsigned char tempRGB;   // 转换变量

    filePtr = fopen(filename, "rb"); // 以“读二进制”模式打开文件
    if (filePtr == NULL)
        return NULL;

    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr); // 读位图文件头
    if (bitmapFileHeader.bfType != BITMAP_ID) // 不等于位图标识符
    {
        fclose(filePtr);
        return NULL;
    }

    // 通过检查位图标识ID，来验证这是否是一个BMP文件
    fread(bitmapinfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    // 将文件指针移动到位图数据的开始处
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    // 为位图图像数据分配足够的内存
    bitmapImage = (unsigned char*)malloc(bitmapinfoHeader->biSizeImage);
    // 核实内存分配
    if (!bitmapImage)
    {
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }

    // 读入位图图像数据
    fread(bitmapImage, 1, bitmapinfoHeader->biSizeImage, filePtr);
    // 确位位图图像数据被读入
    if (bitmapImage == NULL)
    {
        fclose(filePtr);
        return NULL;
    }

    // 由于位图的颜色格式为BGR，因此交换R和B的值，以得到RGB数据
    for (imageIdx = 0; imageIdx < bitmapinfoHeader->biSizeImage; imageIdx += 3)
    {
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }

    // 关闭文件并返回位图数据
    fclose(filePtr);
    return bitmapImage;
}

// 绘制纹理立方体的函数，为每个顶点定义纹理坐标
void drawCube(float xPos, float yPos, float zPos)
{
    glPushMatrix();
    glTranslatef(xPos, yPos, zPos);
    // 顶面
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);   // 纹理和四边形的左下
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);    // 纹理和四边形的右下
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 0.5, -0.5);   // 纹理和四边形的右上
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);  // 纹理和四边形的左上
    glEnd();

    // 正面
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
    glEnd();
    // 右面
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glEnd();
    // 左面
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
    glEnd();
    // 底面
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
    glEnd();

    // 后面
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0, 0.0); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
    glEnd();
    glPopMatrix();
}

// 注意初始化函数中的代码
void Init()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    // 启用2D纹理映射
    glEnable(GL_TEXTURE_2D);

    bitmapData = LoadBitmapFile("C:/Users/Lenovo/Desktop/计算机图形学/实验六/texture.bmp", &bitmapinfoHeader);
    // 生成纹理对象
    glGenTextures(1, &texture);
    // 绑定纹理
    glBindTexture(GL_TEXTURE_2D, texture);
    // 对纹理进行过滤
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // 载入纹理图像
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapinfoHeader.biWidth, bitmapinfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
}

// 显示函数
void mydisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除屏幕和深度缓存
    glLoadIdentity();                // 重置当前的模型视图矩阵
    glTranslatef(0.0, 0.0, -3.0);
    glRotatef(angleX, 1.0, 0.0, 0.0);   // 绕X轴旋转，旋转角度为angle
    glRotatef(angleY, 0.0, 1.0, 0.0);   // 绕Y轴旋转，旋转角度为angle
    glRotatef(angleZ, 0.0, 0.0, 1.0);   // 绕Z轴旋转，旋转角度为angle
    drawCube(0.0, 0.0, 0.0);
    glutSwapBuffers();
}

// 鼠标回调函数
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        angleX += 20.0; // 每次左键单击，角度增加20度
        angleY += 20.0;
        angleZ += 20.0;
        if (angleX >= 360) angleX -= 360; // 防止角度溢出
        if (angleY >= 360) angleY -= 360;
        glutPostRedisplay(); // 通知重新绘制
    }
}

//窗口形状改变回调函数
void reshape(GLsizei width, GLsizei height) { // 重置OpenGL窗口大小
    glViewport(0, 0, width, height); //重置当前视口大小
    glMatrixMode(GL_PROJECTION); // 修改投影矩阵
    glLoadIdentity(); // 导入单位阵,重置投影矩阵
    gluPerspective(50, (GLfloat)width / (GLfloat)height, 0.1, 100); //设置透视投影
    glMatrixMode(GL_MODELVIEW); // 切换到模型视图矩阵
    glLoadIdentity(); // 重置模型视图矩阵
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 680);
    glutCreateWindow("OpenGL程序:纹理映射之立方体");
    Init();
    glutDisplayFunc(mydisplay);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
