#include <windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/glut.h>

#define MAX_POINT 21	//允许的最大控制点个数
#define SCREEN_WIDTH 640	//屏幕的尺寸
#define SCREEN_HEIGHT 480

//定义存储控制点坐标的结构体
class Point
{
public:
	float x, y;
	void position(float x1, float y1) //设置控制点坐标
	{
		x = x1;
		y = y1;
	}
};

int POINT_NUM = 0; //绘制bezier曲线的点的个数
Point bezier[MAX_POINT]; //存储生成bezier曲线点的坐标信息

						 //初始化
void myInit()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);//清空颜色缓存
	glColor3f(1.0, 0.0, 0.0);//设置颜色信息
	glPointSize(4.0);//设置点尺寸
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
}

//画出控制点
void drawPoint(Point pt)
{
	glBegin(GL_POINTS);	//绘制控制点
	glVertex2f(pt.x, pt.y);
	glEnd();
	glFlush();
}

//画出控制点之间的连线
void drawLine(Point p1, Point p2)
{
	glBegin(GL_LINES);	//绘制控制点间的线
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
	glFlush();
}

//贝塞尔曲线
Point drawBezier(Point* X, double t, int num)
{
	Point P;	//定义Point类型用于保存生成的最后一个控制点并返回
	float* xArray = new float[num - 1];	//定义两个数组分别保存每次迭代生成的xy方向上的控制点坐标
	float* yArray = new float[num - 1];

	for (int i = 1; i < num; ++i)
	{
		for (int j = 0; j < num - i; ++j)
		{
			if (i == 1)
			{
				xArray[j] = X[j].x*(1 - t) + X[j + 1].x*t;			//第一次迭代将控制点信息直接复制给两个数组
				yArray[j] = X[j].y*(1 - t) + X[j + 1].y*t;
				continue;
			}
			xArray[j] = xArray[j] * (1 - t) + xArray[j + 1] * t;	//根据迭代公式计算新的控制点
			yArray[j] = yArray[j] * (1 - t) + yArray[j + 1] * t;
		}
	}
	P.x = xArray[0];//迭代后数组中第一个元素为控制点坐标，将其赋值给返回变量
	P.y = yArray[0];
	delete[] xArray;//释放内存空间
	delete[] yArray;
	return P;//返回值
}

//鼠标控制
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//鼠标左键确定控制点
	{
		bezier[POINT_NUM].position((float)x, (float)(SCREEN_HEIGHT - y));
		drawPoint(bezier[POINT_NUM]);
		POINT_NUM++;
		if (POINT_NUM > MAX_POINT)
			POINT_NUM--;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//鼠标右键绘制直线
	{
		if (POINT_NUM >= 3 && POINT_NUM <= MAX_POINT)//控制点数量是否满足要求
		{
			glColor3f(1.0, 0.0, 1.0);
			for (int i = 0; i < POINT_NUM; i++)
				drawPoint(bezier[i]);	//绘制控制点
			glColor3f(1.0, 1.0, 0.0);
			for (int i = 0; i < POINT_NUM - 1; i++)
				drawLine(bezier[i], bezier[i + 1]);	//绘制控制多边形
			glColor3f(0.0, 0.0, 0.0);
			Point P_PRE = bezier[0];//定义保存前一次生成的曲线上的点
			for (double t = 0.0; t <= 1.0; t += 0.05 / POINT_NUM) //根据t值递增计算曲线上的点
			{
				Point P = drawBezier(bezier, t, POINT_NUM);	//生成曲线上的点
				drawLine(P_PRE, P);	//绘制当前点与之前保存点之间的线
				P_PRE = P;	//将当前点赋值作为前一次生成的点
			}
		}
		glColor3f(1.0, 0.0, 0.0);
		POINT_NUM = 0;//绘制完一条曲线后将控制点数清零
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)//鼠标中键清空当前屏幕
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
	}
}

//显示
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

int main(int argc, char * agrv[])
{
	glutInit(&argc, agrv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Bezier曲线");
	glutMouseFunc(myMouse);
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
	return 0;
}
