#include <iostream>
#include <vector>

#define EPS ((double)1e-15)
#define PERTURB_EPS ((double)1e-9)


class Point {
public:
    Point(double x=0.0, double y=0.0);
    Point operator+(Point const &point) const;
    bool operator==(Point const &point) const;
    Point operator*(double const &w) const;
    friend std::istream &operator>>(std::istream &is, Point &point);
    friend std::ostream &operator<<(std::ostream &os, Point const &point);

    double x, y;
};

class Vertex {
public:
    Vertex(
        Point point=Point(), 
        bool intersect=false, 
        double alpha=-1,
        bool entry_exit=false, 
        bool used = false,
        Vertex *prev=nullptr, 
        Vertex *next=nullptr, 
        Vertex *neighbor=nullptr
    );
    
    Vertex *find_next_raw_vertex();

    Point point;
    bool intersect;
    double alpha;
    bool entry_exit;    // entry=false, exit=true
    bool used;
    Vertex *prev;
    Vertex *next;
    Vertex *neighbor;
};

class Polygon {
public:
    Polygon(bool clockwise=true, Vertex *root=nullptr);
    void add_vertex_suffix(Point const &point);
    void add_vertex(Point const &point);
    bool point_in_polygon(Point const &point);
    void reset_used();
    void set_polygon_by_vector(std::vector<std::vector<double>> &polygon);
    void set_polygon(Polygon const &polygon);
    std::vector<std::vector<double>> polygon_to_vector();
    std::vector<Polygon> operator&(Polygon &polygon);
    std::vector<Polygon> operator|(Polygon &polygon);
    std::vector<Polygon> operator-(Polygon &polygon);
    friend std::istream &operator>>(std::istream &is, Polygon &polygon);
    friend std::ostream &operator<<(std::ostream &os, Polygon const &polygon);

    bool clockwise;
    Vertex *root;
};

class BooleanPolygon {
public:
    BooleanPolygon(bool p1_clockwise=true, bool p2_clockwise=false, bool is_hole=true);
    static double signed_area(Point const &a, Point const &b, Point const &c);
    bool find_intersection(Point const &a1, Point const &a2, Point const &b1, Point const &b2, double &alpha_a, double &alpha_b);
    static Vertex *insert_vertex(Vertex *prev_vertex, Point const &point, double alpha);
    static void link_vertex(Vertex *v1, Vertex *v2);
    void find_and_link_intersection();
    void mark_intersection_state();
    void process_polygon();
    std::vector<Polygon> and_polygon();
    std::vector<Polygon> or_polygon();
    std::vector<Polygon> diff_polygon();

    Polygon p1, p2;
    int intersection_cnt;
    bool is_hole;
    bool processed;
};
