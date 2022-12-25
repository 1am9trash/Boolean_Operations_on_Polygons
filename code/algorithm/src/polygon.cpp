#include <unistd.h>
#include <cmath>
#include <iomanip>

#include "polygon.hpp"


Point::Point(double x, double y) : x(x), y(y) {}

Point Point::operator+(Point const &point) const {
    return Point(x + point.x, y + point.y);
}

bool Point::operator==(Point const &point) const {
    return fabsl(x - point.x) < EPS && fabsl(y - point.y) < EPS;
}

Point Point::operator*(double const &w) const {
    return Point(x * w, y * w);
}

std::istream &operator>>(std::istream &is, Point &point) {
    is >> point.x >> point.y;
    return is;
}

std::ostream &operator<<(std::ostream &os, Point const &point) {
    os << point.x << " " << point.y;
    return os;
}

Vertex::Vertex(
    Point point, bool intersect, double alpha, bool entry_exit, bool used,
    Vertex *prev, Vertex *next, Vertex *neighbor
) : 
  point(point),
  intersect(intersect), alpha(alpha), entry_exit(entry_exit), used(used),
  prev(prev), next(next), neighbor(neighbor)
{}

Vertex *Vertex::find_next_raw_vertex() {
    Vertex *vertex = next;
    if (!vertex)
        return nullptr;
    while (vertex->intersect)
        vertex = vertex->next;
    return vertex;
}

Polygon::Polygon(bool clockwise, Vertex *root) : clockwise(clockwise), root(root) {}

void Polygon::add_vertex_suffix(Point const &point) {
    Vertex *new_vertex = new Vertex(point);

    if (root == nullptr) {
        root = new_vertex;
        root->prev = root;
        root->next = root;
    } else {
        Vertex *end_vertex = root->prev;
        end_vertex->next = new_vertex;
        new_vertex->prev = end_vertex;
        new_vertex->next = root;
        root->prev = new_vertex;
    }
}

void Polygon::add_vertex(Point const &point) {
    add_vertex_suffix(point);
    root = root->prev;
}

bool Polygon::point_in_polygon(Point const &point) {
    Vertex *vertex = root;
    bool inside = false;

    if (!vertex)
        throw "Error: Polygon is empty.\n";

    do {
        Point p1 = vertex->point;
        Point p2 = vertex->next->point;

        if (((p2.y > point.y) != (p1.y > point.y)) && (point.x < (p1.x - p2.x) * (point.y - p2.y) / (p1.y - p2.y) + p2.x))
            inside = !inside;

        vertex = vertex->next;
    } while (vertex != root);
    
    return inside;
}

void Polygon::reset_used() {
    Vertex *vertex = this->root;

    if (!vertex)
        throw "Error: Polygon is empty.\n";

    do {
        vertex->used = false;
        vertex = vertex->next;
    } while (vertex != this->root);
}

void Polygon::set_polygon_by_vector(std::vector<std::vector<double>> &polygon) {
    for (size_t i = 0; i < polygon.size(); i++) {
        if (polygon[i].size() != 2)
            throw "Error: Point dimension in polygon is wrong.\n";

        Point point(polygon[i][0], polygon[i][1]);
        if (clockwise)
            add_vertex_suffix(point);
        else
            add_vertex(point);
    }
}

void Polygon::set_polygon(Polygon const &polygon) {
    Vertex *vertex = polygon.root;

    if (!vertex)
        throw "Error: Polygon is empty.\n";

    do {
        add_vertex(vertex->point);
        vertex = vertex->next;
    } while (vertex != polygon.root);
}

std::vector<std::vector<double>> Polygon::polygon_to_vector() {
    std::vector<std::vector<double>> polygon;

    Vertex *vertex = root;

    if (!vertex)
        throw "Error: Polygon is empty.\n";

    do {
        polygon.push_back(std::vector<double>{vertex->point.x, vertex->point.y});
        vertex = vertex->next;
    } while (vertex != root);

    return polygon;
}

std::vector<Polygon> Polygon::operator&(Polygon &polygon) {
    Vertex *p1_vertex = this->root;
    Vertex *p2_vertex = polygon.root;

    if (!p1_vertex || !p2_vertex)
        throw "Error: Polygon is empty.\n";

    std::vector<Polygon> polygons;
    do {
        while (!p1_vertex->intersect || p1_vertex->used) {
            p1_vertex->used = true;
            p1_vertex = p1_vertex->next;
            if (p1_vertex == this->root)
                break;
        }
        if (p1_vertex->used)
            break;
        Vertex *cur_vertex = p1_vertex;

        void (Polygon::*add_vertex_fp)(Point const &);
        if (!cur_vertex->entry_exit)
            add_vertex_fp = &Polygon::add_vertex_suffix;
        else
            add_vertex_fp = &Polygon::add_vertex;

        Point start_point = cur_vertex->point;
        Polygon new_polygon;
        (new_polygon.*add_vertex_fp)(start_point);
        while (true) {
            cur_vertex->used = true;
            if (!cur_vertex->entry_exit) {
                do {
                    cur_vertex = cur_vertex->next;
                    cur_vertex->used = true;
                    if (cur_vertex->point == start_point)
                        break;
                    else
                        (new_polygon.*add_vertex_fp)(cur_vertex->point);
                } while (!cur_vertex->intersect);
            } else {
                do {
                    cur_vertex = cur_vertex->prev;
                    cur_vertex->used = true;
                    if (cur_vertex->point == start_point)
                        break;
                    else
                        (new_polygon.*add_vertex_fp)(cur_vertex->point);
                } while (!cur_vertex->intersect);
            }
            if (cur_vertex->point == start_point)
                break;
            cur_vertex = cur_vertex->neighbor;
        }
        polygons.push_back(new_polygon);
    } while (p1_vertex != this->root);

    this->reset_used();
    polygon.reset_used();

    return polygons;
}

std::vector<Polygon> Polygon::operator|(Polygon &polygon) {
    Vertex *p1_vertex = this->root;
    Vertex *p2_vertex = polygon.root;

    if (!p1_vertex || !p2_vertex)
        throw "Error: Polygon is empty.\n";

    std::vector<Polygon> polygons;
    do {
        while (!p1_vertex->intersect || p1_vertex->used) {
            p1_vertex->used = true;
            p1_vertex = p1_vertex->next;
            if (p1_vertex == this->root)
                break;
        }
        if (p1_vertex->used)
            break;
        Vertex *cur_vertex = p1_vertex;

        void (Polygon::*add_vertex_fp)(Point const &);
        if (!cur_vertex->entry_exit)
            add_vertex_fp = &Polygon::add_vertex_suffix;
        else
            add_vertex_fp = &Polygon::add_vertex;

        Point start_point = cur_vertex->point;
        Polygon new_polygon;
        (new_polygon.*add_vertex_fp)(start_point);
        while (true) {
            cur_vertex->used = true;
            if (!cur_vertex->entry_exit) {
                do {
                    cur_vertex = cur_vertex->prev;
                    cur_vertex->used = true;
                    if (cur_vertex->point == start_point)
                        break;
                    else
                        (new_polygon.*add_vertex_fp)(cur_vertex->point);
                } while (!cur_vertex->intersect);
            } else {
                do {
                    cur_vertex = cur_vertex->next;
                    cur_vertex->used = true;
                    if (cur_vertex->point == start_point)
                        break;
                    else
                        (new_polygon.*add_vertex_fp)(cur_vertex->point);
                } while (!cur_vertex->intersect);
            }
            if (cur_vertex->point == start_point)
                break;
            cur_vertex = cur_vertex->neighbor;
        }
        polygons.push_back(new_polygon);
    } while (p1_vertex != this->root);

    this->reset_used();
    polygon.reset_used();

    return polygons;
}

std::vector<Polygon> Polygon::operator-(Polygon &polygon) {
    Vertex *p1_vertex = this->root;
    Vertex *p2_vertex = polygon.root;

    if (!p1_vertex || !p2_vertex)
        throw "Error: Polygon is empty.\n";

    std::vector<Polygon> polygons;
    do {
        while (!p1_vertex->intersect
            || (p1_vertex->intersect && p1_vertex->entry_exit)
            || p1_vertex->used
        ) {
            p1_vertex->used = true;
            p1_vertex = p1_vertex->next;
            if (p1_vertex == this->root)
                break;
        }
        if (p1_vertex->used)
            break;
        Vertex *cur_vertex = p1_vertex;

        Point start_point = cur_vertex->point;
        Polygon new_polygon;
        new_polygon.add_vertex_suffix(start_point);
        bool prev = true;
        while (true) {
            cur_vertex->used = true;
            if (prev) {
                do {
                    cur_vertex = cur_vertex->prev;
                    cur_vertex->used = true;
                    if (cur_vertex->point == start_point)
                        break;
                    else
                        new_polygon.add_vertex_suffix(cur_vertex->point);
                } while (!cur_vertex->intersect);
            } else {
                do {
                    cur_vertex = cur_vertex->next;
                    cur_vertex->used = true;
                    if (cur_vertex->point == start_point)
                        break;
                    else
                        new_polygon.add_vertex_suffix(cur_vertex->point);
                } while (!cur_vertex->intersect);
            }
            if (cur_vertex->point == start_point)
                break;
            if (cur_vertex->entry_exit != cur_vertex->neighbor->entry_exit)
                prev = !prev;
            cur_vertex = cur_vertex->neighbor;
        }
        polygons.push_back(new_polygon);
    } while (p1_vertex != this->root);

    this->reset_used();
    polygon.reset_used();

    return polygons;
}

std::istream &operator>>(std::istream &is, Polygon &polygon) {
    Point point;
    char split;

    while (!is.eof()) {
        is >> point >> split;
        if (is.fail())
            break;
        else {
            if (polygon.clockwise)
                polygon.add_vertex_suffix(point);
            else
                polygon.add_vertex(point);
            if (split == ';')
                break;
        }
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, Polygon const &polygon) {
    Vertex *cur_vertex = polygon.root;

    while (cur_vertex) {
        std::cout << cur_vertex->point;
        std::cout << " (";
        if (cur_vertex->intersect) {
            std::cout << "entry_exit: " << cur_vertex->entry_exit << ", ";
            std::cout << "alpha: " << cur_vertex->alpha << ", ";
            std::cout << "neighbor: " << cur_vertex->neighbor->point << ", ";
        }
        std::cout << "used: " << cur_vertex->used;
        std::cout << ")";

        cur_vertex = cur_vertex->next;
        if (cur_vertex != polygon.root)
            std::cout << ",\n";
        else {
            std::cout << ";\n";
            break;
        }
    }

    return os;
}

BooleanPolygon::BooleanPolygon(bool p1_clockwise, bool p2_clockwise, bool is_hole) : 
  p1(Polygon(p1_clockwise)), p2(Polygon(p2_clockwise)), intersection_cnt(0), is_hole(is_hole), processed(false) {}

double BooleanPolygon::signed_area(Point const &a, Point const &b, Point const &c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool BooleanPolygon::find_intersection(Point const &a1, Point const &a2, Point const &b1, Point const &b2, double &alpha_a, double &alpha_b) {
    double wec_a1 = signed_area(a1, b1, b2);
    double wec_a2 = signed_area(a2, b1, b2);
    double wec_b1 = signed_area(b1, a1, a2);
    double wec_b2 = signed_area(b2, a1, a2);
    if (wec_a1 * wec_a2 < (double)0.0) {
        if (wec_b1 * wec_b2 < (double)0.0) {
            alpha_a = wec_a1 / (wec_a1 - wec_a2);
            alpha_b = wec_b1 / (wec_b1 - wec_b2);
            return true;
        }
    }
    return false;
}

Vertex *BooleanPolygon::insert_vertex(Vertex *prev_vertex, Point const &point, double alpha) {
    Vertex *new_vertex = new Vertex(point, true, alpha);

    if (!prev_vertex)
        throw "Error: Vertex is empty.\n";

    do {
        prev_vertex = prev_vertex->next;
    } while (prev_vertex->intersect && alpha - prev_vertex->alpha > EPS);

    prev_vertex->prev->next = new_vertex;
    new_vertex->prev = prev_vertex->prev;
    new_vertex->next = prev_vertex;
    prev_vertex->prev = new_vertex;

    return new_vertex;
}

void BooleanPolygon::link_vertex(Vertex *v1, Vertex *v2)  {
    v1->neighbor = v2;
    v2->neighbor = v1;
}

void BooleanPolygon::find_and_link_intersection() {
    Vertex *p1_vertex = p1.root;
    Vertex *p2_vertex = p2.root;

    if (!p1_vertex || !p2_vertex)
        throw "Error: Polygon is empty.\n";

    do {
        p1_vertex->point.x += ((double)rand() / RAND_MAX + 1.0) * PERTURB_EPS;
        p1_vertex->point.y += ((double)rand() / RAND_MAX + 1.0) * PERTURB_EPS;
        p1_vertex = p1_vertex->next;
    } while (p1_vertex != p1.root);
    
    do {
        p2_vertex->point.x += ((double)rand() / RAND_MAX + 1.0) * PERTURB_EPS;
        p2_vertex->point.y += ((double)rand() / RAND_MAX + 1.0) * PERTURB_EPS;
        p2_vertex = p2_vertex->next;
    } while (p2_vertex != p2.root);

    do {
        Vertex *p1_next_vertex = p1_vertex->find_next_raw_vertex();
        do {
            Vertex *p2_next_vertex = p2_vertex->find_next_raw_vertex();

            double alpha_a, alpha_b;
            Point const &a1 = p1_vertex->point;
            Point const &a2 = p1_next_vertex->point;
            Point const &b1 = p2_vertex->point;
            Point const &b2 = p2_next_vertex->point;
            if (find_intersection(a1, a2, b1, b2, alpha_a, alpha_b)) {
                intersection_cnt++;
                Point new_point = a1 * ((double)1.0 - alpha_a) + a2 * alpha_a;
                Vertex *new_p1_vertex = insert_vertex(p1_vertex, new_point, alpha_a);
                Vertex *new_p2_vertex = insert_vertex(p2_vertex, new_point, alpha_b);
                link_vertex(new_p1_vertex, new_p2_vertex);
            }
            p2_vertex = p2_next_vertex;
        } while (p2_vertex != p2.root);
        p1_vertex = p1_next_vertex;
    } while (p1_vertex != p1.root);
}

void BooleanPolygon::mark_intersection_state() {
    Vertex *p1_vertex = p1.root;
    Vertex *p2_vertex = p2.root;

    if (!p1_vertex || !p2_vertex)
        throw "Error: Polygon is empty.\n";

    bool p1_exit = p2.point_in_polygon(p1.root->point);
    bool p2_exit = p1.point_in_polygon(p2.root->point);

    do {
        if (p1_vertex->intersect) {
            p1_vertex->entry_exit = p1_exit;
            p1_exit = !p1_exit;
        }
        p1_vertex = p1_vertex->next;
    } while (p1_vertex != p1.root);

    do {
        if (p2_vertex->intersect) {
            p2_vertex->entry_exit = p2_exit;
            p2_exit = !p2_exit;
        }
        p2_vertex = p2_vertex->next;
    } while (p2_vertex != p2.root);
}

void BooleanPolygon::process_polygon() {
    processed = true;
    find_and_link_intersection();
    mark_intersection_state();
}

std::vector<Polygon> BooleanPolygon::and_polygon() {
    if (!processed)
        process_polygon();

    if (intersection_cnt == 0) {
        if (p2.point_in_polygon(p1.root->point))
            return std::vector<Polygon>{p1};
        else if ((p1.point_in_polygon(p2.root->point)))
            return std::vector<Polygon>{p2};
        else
            return std::vector<Polygon>();
    }

    return p1 & p2;
}

std::vector<Polygon> BooleanPolygon::or_polygon() {
    if (!processed)
        process_polygon();

    if (intersection_cnt == 0) {
        if (p2.point_in_polygon(p1.root->point))
            return std::vector<Polygon>{p2};
        else if ((p1.point_in_polygon(p2.root->point)))
            return std::vector<Polygon>{p1};
        else
            return std::vector<Polygon>{p1, p2};
    }

    if (is_hole) {
        std::vector<Polygon> and_result = and_polygon();
        std::vector<Polygon> p1_p2_diff_result = diff_polygon();

        BooleanPolygon p2_p1;
        p2_p1.p1.set_polygon(p2);
        p2_p1.p2.set_polygon(p1);
        std::vector<Polygon> p2_p1_diff_result = p2_p1.diff_polygon();

        std::vector<Polygon> result;
        result.reserve(and_result.size() + p1_p2_diff_result.size() + p2_p1_diff_result.size());
        result.insert(result.end(), and_result.begin(), and_result.end());
        result.insert(result.end(), p1_p2_diff_result.begin(), p1_p2_diff_result.end());
        result.insert(result.end(), p2_p1_diff_result.begin(), p2_p1_diff_result.end());

        return result;
    } else
        return p1 | p2;
}

std::vector<Polygon> BooleanPolygon::diff_polygon() {
    if (!processed)
        process_polygon();

    if (intersection_cnt == 0) {
        if (p2.point_in_polygon(p1.root->point))
            return std::vector<Polygon>();
        else if ((p1.point_in_polygon(p2.root->point)))
            throw "Error: Didn't deal with diff_polygon() with hole cases.\n";
        else
            return std::vector<Polygon>{p1};
    }

    return p1 - p2;
}