import matplotlib.pyplot as plt
import numpy as np

# Функция для кодирования точек по отношению к окну
def compute_code(x, y, xmin, ymin, xmax, ymax):
    code = 0
    if x < xmin:  # левая граница окна
        code |= 1
    elif x > xmax:  # правая граница окна
        code |= 2
    if y < ymin:  # нижняя граница окна
        code |= 4
    elif y > ymax:  # верхняя граница окна
        code |= 8
    return code

# Функция отсечения отрезка по отсекающему окну
def cohen_sutherland(x1, y1, x2, y2, xmin, ymin, xmax, ymax):
    code1 = compute_code(x1, y1, xmin, ymin, xmax, ymax)
    code2 = compute_code(x2, y2, xmin, ymin, xmax, ymax)
    accept = False

    while True:
        if not (code1 | code2):  # оба конца отрезка внутри окна
            accept = True
            break
        elif code1 & code2:  # отрезок полностью за пределами окна
            break
        else:
            x = 0
            y = 0
            code_out = code1 if code1 else code2  # выбираем точку с невалидным кодом
            if code_out & 1:  # левая граница
                x = xmin
                y = y1 + (y2 - y1) * (xmin - x1) / (x2 - x1)
            elif code_out & 2:  # правая граница
                x = xmax
                y = y1 + (y2 - y1) * (xmax - x1) / (x2 - x1)
            elif code_out & 4:  # нижняя граница
                y = ymin
                x = x1 + (x2 - x1) * (ymin - y1) / (y2 - y1)
            elif code_out & 8:  # верхняя граница
                y = ymax
                x = x1 + (x2 - x1) * (ymax - y1) / (y2 - y1)

            if code_out == code1:  # обновляем координаты первой точки
                x1 = x
                y1 = y
                code1 = compute_code(x1, y1, xmin, ymin, xmax, ymax)
            else:  # обновляем координаты второй точки
                x2 = x
                y2 = y
                code2 = compute_code(x2, y2, xmin, ymin, xmax, ymax)

    return accept, x1, y1, x2, y2

with open('/Users/vanya/Documents/pkg/pkg5/Lab_5_code/pkg5/input.txt', 'r') as file:
    lines = file.readlines()
    n = int(lines[0])
    segments = []
    for i in range(1, n + 1):
        x1, y1, x2, y2 = map(float, lines[i].split())
        segments.append([(x1, y1), (x2, y2)])
    xmin, ymin, xmax, ymax = map(float, lines[n + 1].split())

window = (xmin, ymin, xmax, ymax)  # координаты отсекающего окна (Xmin, Ymin, Xmax, Ymax)

# Создание графика
plt.figure()
plt.axis([0, 300, 0, 300])

# Отображение окна
plt.plot([window[0], window[2], window[2], window[0], window[0]],
         [window[1], window[1], window[3], window[3], window[1]], 'b-')

# Отображение и отсечение отрезков
for seg in segments:
    x1, y1 = seg[0]
    x2, y2 = seg[1]
    plt.plot([x1, x2], [y1, y2], 'r-')

    accept, new_x1, new_y1, new_x2, new_y2 = cohen_sutherland(x1, y1, x2, y2, *window)
    if accept:
        plt.plot([new_x1, new_x2], [new_y1, new_y2], 'g-')

# Настройка масштаба и пределов осей
plt.axis([window[0], window[2], window[1], window[3]])
plt.axis('equal')

plt.title('Отсечение отрезков по окну')
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.grid(True)
plt.show()