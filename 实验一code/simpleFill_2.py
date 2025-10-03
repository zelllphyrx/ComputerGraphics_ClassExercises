import matplotlib.pyplot as plt
import numpy as np

# 假设的五边形顶点，以(x, y)的形式
P = [
    (100, 100),
    (500, 100),
    (500, 500),
    (100, 500),
    (300, 300)
]

# 背景色和填充色
BClr = (1.0, 1.0, 1.0)  # 白色
FClr = (0.0, 0.0, 1.0)  # 蓝色


# 边填充算法
def fill_polygon(P, BClr, FClr):
    # 创建一个足够大的画布
    canvas_size = 1000
    canvas = np.full((canvas_size, canvas_size, 3), BClr, dtype=np.float32)
    fill_count = np.zeros((canvas_size, canvas_size), dtype=int)  # 用于记录每个像素的填充次数

    # 绘制五边形边界
    def draw_edges():
        for i in range(len(P)):
            j = (i + 1) % len(P)
            plt.plot([P[i][0], P[j][0]], [P[i][1], P[j][1]], color='black')

    for i in range(len(P)):
        j = (i + 1) % len(P)
        x1, y1 = P[i]
        x2, y2 = P[j]
        k = (x2 - x1) / (y2 - y1) if y2 != y1 else float('inf')

        if y1 > y2:
            yMin, yMax = y2, y1
            x = x2
        else:
            yMin, yMax = y1, y2
            x = x1

        # 修改：排除上顶点，y 的范围为 [yMin, yMax)
        for y in range(int(yMin), int(yMax)):  # 改为不包含 yMax
            # 计算扫描线与边的交点
            x_new = x1 + (y - y1) * k if k != float('inf') else x1
            for m in range(int(x_new), canvas_size):
                # 增加填充次数
                fill_count[int(y), int(m)] += 1

                # 奇数次填充使用填充色，偶数次恢复背景色
                if fill_count[int(y), int(m)] % 2 == 1:
                    canvas[int(y), int(m)] = FClr
                else:
                    canvas[int(y), int(m)] = BClr

        # 显示每一步的结果
        plt.imshow(canvas)
        draw_edges()  # 重新绘制边框
        plt.draw()
        plt.pause(0.5)
        plt.clf()

    # 显示最终结果
    plt.imshow(canvas)
    draw_edges()  # 最终结果中也重新绘制边框
    plt.show()


# 调用函数
fill_polygon(P, BClr, FClr)
