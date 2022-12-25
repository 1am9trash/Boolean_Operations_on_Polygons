import sys 
sys.path.append("..") 

import BooleanPolygon
from visualization_tool import visualize


filename = input("filename = ")
polygon1, polygon2 = visualize.read_polygon_from_file(filename)

bp = BooleanPolygon.BooleanPolygon(is_hole=True)
bp.p1.set_polygon(polygon1)
bp.p2.set_polygon(polygon2)

try:
    and_result = bp.and_polygon()
    visualize.draw_polygon(
        [[polygon1], [polygon2], and_result],
        [0, 0, 0.3],
        ["r", "g", "b"],
        ["polygon1", "polygon2" , "and result"]
    )
except Exception as e:
    print(f"Exception: {e}")

try:
    or_result = bp.or_polygon()
    visualize.draw_polygon(
        [[polygon1], [polygon2], or_result],
        [0, 0, 0.3],
        ["r", "g", "b"],
        ["polygon1", "polygon2" , "or result"]
    )
except Exception as e:
    print(f"Exception: {e}")

try:
    diff_result = bp.diff_polygon()
    visualize.draw_polygon(
        [[polygon1], [polygon2], diff_result],
        [0, 0, 0.3],
        ["r", "g", "b"],
        ["polygon1", "polygon2" , "diff result"]
    )
except Exception as e:
    print(f"Exception: {e}")