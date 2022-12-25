import numpy as np
import matplotlib.pyplot as plt


def read_polygon_from_file(filename):
    polygons = []

    with open(filename, "r") as reader:
        polygon = []
        for line in reader.readlines():
            if line.strip() == "":
                continue
            line = line.strip()
            symbol = line[-1]
            x, y = map(float, line[:-1].split())
            polygon.append([x, y])

            if symbol == ";":
                polygons.append(polygon)
                polygon = []

    return polygons

def draw_polygon(list_of_polygons, alphas, colors, labels):
    fig, host = plt.subplots()
    host.grid(False)

    x_mn = 1e10
    x_mx = -1e10
    y_mn = 1e10
    y_mx = -1e10

    for polygons in list_of_polygons:
        for polygon in polygons:
            x = [point[0] for point in polygon]
            y = [point[1] for point in polygon]
            x_mn = min(x_mn, min(x))
            x_mx = max(x_mx, max(x))
            y_mn = min(y_mn, min(y))
            y_mx = max(y_mx, max(y))

    padding = 0.05
    x_padding = (x_mx - x_mn) * padding
    y_padding = (y_mx - y_mn) * padding
    host.set_xlim(x_mn - 3 * x_padding, x_mx + x_padding)
    host.set_ylim(y_mn - y_padding, y_mx + y_padding)

    for i, polygons in enumerate(list_of_polygons):
        alpha = alphas[i] if len(alphas) > i else 0
        color = colors[i] if len(colors) > i else "b"
        label = labels[i] if len(labels) > i else None

        for j, polygon in enumerate(polygons):
            x = [point[0] for point in polygon]
            y = [point[1] for point in polygon]
            if alpha == 0:
                x.append(x[0])
                y.append(y[0])
                if j == 0:
                    host.plot(x, y, color=color, alpha=0.7, label=label)
                else:
                    host.plot(x, y, color=color, alpha=0.7)
            else:
                if j == 0:
                    host.fill(x, y, color=color, alpha=alpha, label=label)
                else:
                    host.fill(x, y, color=color, alpha=alpha)
    host.legend(loc="upper left", fontsize=10)

    plt.show()

if __name__ == "__main__":
    polygon1, polygon2 = read_polygon_from_file("testcase/simple_square.txt")
    polygon3 = []
    # polygons = read_polygon_from_file("../output.txt")
    # polygon1 = polygons[0]
    # polygon2 = polygons[1]
    # polygon3 = polygons[2:]

    draw_polygon(
        [[polygon1], [polygon2], polygon3],
        [0, 0, 0.3],
        ["r", "g", "b"],
        ["polygon 1", "polygon 2", "result"]
    )
