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

# 栅栏边填充算法
def fill_polygon(P, BClr, FClr):
    # 创建一个足够大的画布
    canvas_size = 1000
    canvas = np.full((canvas_size, canvas_size, 3), BClr, dtype=np.float32)

    # 找出最小和最大 y 值，以确定扫描线范围
    min_y = min([p[1] for p in P])
    max_y = max([p[1] for p in P])

    # 栅栏定义：通过多边形顶点的垂直线
    fence_x = (min([p[0] for p in P]) + max([p[0] for p in P])) // 2

    # 填充过程
    for i in range(len(P)):
        j = (i + 1) % len(P)
        x1, y1 = P[i]
        x2, y2 = P[j]

        # 找出当前边的 y 范围
        if y1 > y2:
            y1, y2 = y2, y1
            x1, x2 = x2, x1

        for y in range(y1, y2):
            # 计算交点
            if y2 != y1:
                x_intersect = x1 + (y - y1) * (x2 - x1) / (y2 - y1)
                x_int = int(x_intersect)

                # 填充色块
                if x_int < fence_x:  # 交点在栅栏左侧
                    canvas[y, x_int:fence_x] = FClr
                else:  # 交点在栅栏右侧
                    canvas[y, fence_x:x_int] = FClr

        # 绘制当前边
        plt.plot([x1, x2], [y1, y2], color='black')

        # 显示当前填充状态
        plt.imshow(canvas)
        plt.plot(*zip(*P, P[0]), color='black')  # 绘制完整的多边形边界
        plt.plot([x1, x2], [y1, y2], color='black')  # 重新绘制当前边
        plt.axvline(x=fence_x, color='red', linestyle='--')  # 绘制栅栏线
        plt.pause(0.5)  # 暂停以观察每一步

    # 显示最终结果
    plt.imshow(canvas)
    plt.plot(*zip(*P, P[0]), color='black')  # 最终结果中也重新绘制边界
    plt.axvline(x=fence_x, color='red', linestyle='--')  # 最终结果中也重新绘制栅栏线
    plt.plot(*zip(*P, P[0]), color='black')  # 绘制完整多边形边界
    plt.show()

# 调用函数
fill_polygon(P, BClr, FClr)
