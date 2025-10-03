#define _CRT_SECURE_NO_WARNINGS
#define BITMAP_ID 0x4D42     //位图标识ID
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<gl/glut.h>

float angle = 0.0; //旋转角度

BITMAPINFOHEADER bitmapinfoHeader;//位图息头部
unsigned char* bitmapData;   // 纹理数据
unsigned int  texture;    // 纹理对象

unsigned char* LoadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapinfoHeader)
{
	FILE* filePtr;   //文件指针
	BITMAPFILEHEADER bitmapFileHeader;//位图文件头
	unsigned char* bitmapImage;  //位图图像数据
	int     imageIdx = 0;   // 位图索引计数器
	unsigned char  tempRGB;   //变换变量
	printf("flag\n");

	filePtr = fopen(filename, "rb"); //以“读二进制”模式打开文件名
	if (filePtr == NULL)
		return NULL;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);//读位图文件头
	if (bitmapFileHeader.bfType != BITMAP_ID)//不等于位图标识符
	{
		fclose(filePtr);
		return NULL;
	}

	//通过检查位图标识ID，来验证这是否是一个BMP文件
	fread(bitmapinfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	//将文件指针移动到位图数据的开始处
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	//为位图图像数据分配足够的内存
	bitmapImage = (unsigned char*)malloc(bitmapinfoHeader->biSizeImage);
	//核实内存分配
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	//读入位图图像数据
	fread(bitmapImage, 1, bitmapinfoHeader->biSizeImage, filePtr);
	//确定位图图像数据被读入
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	//由于位图的颜色格式为BGR，因此交换R和B的值，以得到RGB数据
	for (imageIdx = 0; imageIdx < bitmapinfoHeader->biSizeImage; imageIdx += 3) //修改imageIdx++为imageIdx+=3，修正原代码贴图颜色无法正常显示的问题
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	//关闭文件并返回位图数据
	fclose(filePtr);
	return bitmapImage;
}

//绘制纹理立方体的函数，为每个顶点定义纹理坐标
void drawCube()
{
	glBegin(GL_QUADS);
	// 顶面
	glTexCoord2f(0.25, 0.66); glVertex3f(0.5, -0.5, 0.5);   // 左下
	glTexCoord2f(0.5, 0.66); glVertex3f(0.5, 0.5, 0.5);    // 右下
	glTexCoord2f(0.25, 1.0); glVertex3f(-0.5, 0.5, 0.5);   // 右上
	glTexCoord2f(0.5, 1.0); glVertex3f(-0.5, -0.5, 0.5);  // 左上

	// 正面
	glTexCoord2f(0.25, 0.33); glVertex3f(0.5, -0.5, -0.5);    // 左下
	glTexCoord2f(0.5, 0.33); glVertex3f(0.5, 0.5, -0.5);     // 右下
	glTexCoord2f(0.5, 0.66); glVertex3f(0.5, 0.5, 0.5);    // 右上
	glTexCoord2f(0.25, 0.66); glVertex3f(0.5, -0.5, 0.5);   // 左上

	// 右面
	glTexCoord2f(0.5, 0.33); glVertex3f(0.5, 0.5, -0.5);   // 左下
	glTexCoord2f(0.75, 0.33); glVertex3f(-0.5, 0.5, -0.5);    // 右下
	glTexCoord2f(0.75, 0.66); glVertex3f(-0.5, 0.5, 0.5);   // 右上
	glTexCoord2f(0.5, 0.66); glVertex3f(0.5, 0.5, 0.5);  // 左上

	// 左面
	glTexCoord2f(0.0, 0.33); glVertex3f(-0.5, -0.5, -0.5);  // 左下
	glTexCoord2f(0.25, 0.33); glVertex3f(0.5, -0.5, -0.5);   // 右下
	glTexCoord2f(0.25, 0.66); glVertex3f(0.5, -0.5, 0.5);  // 右上
	glTexCoord2f(0.0, 0.66); glVertex3f(-0.5, -0.5, 0.5); // 左上

	// 底面
	glTexCoord2f(0.25, 0.0); glVertex3f(-0.5, -0.5, -0.5);   // 左下
	glTexCoord2f(0.5, 0.0); glVertex3f(-0.5, 0.5, -0.5);  // 右下
	glTexCoord2f(0.5, 0.33); glVertex3f(0.5, 0.5, -0.5); // 右上
	glTexCoord2f(0.25, 0.33); glVertex3f(0.5, -0.5, -0.5);  // 左上

	// 后面
	glTexCoord2f(0.75, 0.33); glVertex3f(-0.5, 0.5, -0.5);   // 左下
	glTexCoord2f(1.0, 0.33); glVertex3f(-0.5, -0.5, -0.5);  // 右下
	glTexCoord2f(1.0, 0.66); glVertex3f(-0.5, -0.5, 0.5); // 右上
	glTexCoord2f(0.75, 0.66); glVertex3f(-0.5, 0.5, 0.5);  // 左上

	glEnd();
}

//注意初始化函数中的代码
void Init()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	//启用2D纹理映射
	glEnable(GL_TEXTURE_2D);

	bitmapData = LoadBitmapFile("C:/Users/Lenovo/Desktop/计算机图形学/实验六/texture.bmp", &bitmapinfoHeader);
	//生成纹理对象
	glGenTextures(1, &texture);
	//绑定纹理
	glBindTexture(GL_TEXTURE_2D, texture);
	//对纹理进行过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//载入纹理图像
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapinfoHeader.biWidth, bitmapinfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);
}

//显示函数
void mydisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存
	glLoadIdentity();                //重置当前的模型观察矩阵
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(angle, 1.0, 0.0, 0.0);   //绕X轴旋转，旋转角度为angle
	glRotatef(angle, 0.0, 1.0, 0.0);   //绕y轴旋转
	glRotatef(angle, 0.0, 0.0, 1.0);   //绕z轴旋转
	drawCube();          //绘制纹理映射立方体
	glutSwapBuffers();
}

//全局回调函数
void myIdle()
{
	if (angle >= 360)
		angle = 0.0;
	angle += 0.1;//自动旋转每次旋转0.1，慢一点方便观察
	mydisplay();
}

//窗口形状改变回调函数
GLvoid reshape(GLsizei width, GLsizei height)    // 重置OpenGL窗口大小
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));//重置当前视口大小，起始位置为客户端窗口左下角（非坐标原点）
	glMatrixMode(GL_PROJECTION);  // 修改投影矩阵
	glLoadIdentity();             // 导入单位阵,重置投影矩阵
	//(fovy, (float)w/h,near, far)
	gluPerspective(50, (float)width / (float)height, 0.1, 100);//宽高比改为当前值，视线区域与屏幕大小一致设置透视投影
	glMatrixMode(GL_MODELVIEW);   // 切换到模型视图矩阵
	glLoadIdentity();             // 重置模型视图矩阵
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1280, 680);//对默认置的修改
	glutCreateWindow(" OpenGL程序:纹理映射之立方体");
	Init();
	glutDisplayFunc(mydisplay);
	glutReshapeFunc(reshape);//当窗口尺寸改变时，图形比例不发生变化，该窗口的形状改变回调函数将被调用。
	//glutIdleFunc设置全局的回调函数，当没有窗口事件到达时，GLUT程序功能可以执行后台处理任务或连续动画。
	//如果启用，这个IdleFunction会被不断调用,直到有窗口事件发生。
	glutIdleFunc(myIdle);
	glutMainLoop();
	return 0;
}
