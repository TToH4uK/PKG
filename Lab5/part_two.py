import matplotlib.pyplot as plt

# Функция для отображения системы координат
def draw_axes():
    plt.axhline(0, color='black',linewidth=0.5)
    plt.axvline(0, color='black',linewidth=0.5)

# Функция для отображения отрезка
def draw_line(x1, y1, x2, y2, color='blue'):
    plt.plot([x1, x2], [y1, y2], color=color)

# Функция для отображения окна отсечения
def draw_clipping_window(xmin, ymin, xmax, ymax):
    plt.plot([xmin, xmax, xmax, xmin, xmin], [ymin, ymin, ymax, ymax, ymin], color='red')

# Функция для реализации алгоритма Кируса-Бека для отсечения отрезков
def cyrus_beck(line, xmin, ymin, xmax, ymax):
    x1, y1, x2, y2 = line

    dx = x2 - x1
    dy = y2 - y1

    p = [-dx, dx, -dy, dy]
    q = [x1 - xmin, xmax - x1, y1 - ymin, ymax - y1]

    t_enter = 0
    t_exit = 1

    for i in range(4):
        if p[i] == 0:
            if q[i] < 0:
                return None
        else:
            t = q[i] / p[i]
            if p[i] < 0:
                t_enter = max(t_enter, t)
            else:
                t_exit = min(t_exit, t)

    if t_enter > t_exit:
        return None

    clipped_x1 = x1 + t_enter * dx
    clipped_y1 = y1 + t_enter * dy
    clipped_x2 = x1 + t_exit * dx
    clipped_y2 = y1 + t_exit * dy

    return [clipped_x1, clipped_y1, clipped_x2, clipped_y2]

# Чтение данных из файла
with open('/Users/vanya/Documents/pkg/pkg5/Lab_5_code/pkg5/input.txt', 'r') as file:
    lines = file.readlines()
    n = int(lines[0])
    segments = []
    for i in range(1, n + 1):
        x1, y1, x2, y2 = map(float, lines[i].split())
        segments.append([x1, y1, x2, y2])
    xmin, ymin, xmax, ymax = map(float, lines[n + 1].split())

# Визуализация системы координат
plt.figure(figsize=(8, 8))
draw_axes()

# Отображение исходных отрезков
for segment in segments:
    draw_line(segment[0], segment[1], segment[2], segment[3])

# Отображение окна отсечения
draw_clipping_window(xmin, ymin, xmax, ymax)

# Отсечение и визуализация видимых частей отрезков
for segment in segments:
    clipped_segment = cyrus_beck(segment, xmin, ymin, xmax, ymax)
    if clipped_segment:
        draw_line(clipped_segment[0], clipped_segment[1], clipped_segment[2], clipped_segment[3], color='green')

plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.title('Clipping of Segments')
plt.grid()
plt.show()