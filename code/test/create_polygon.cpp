#include <ctime>
#include <random>


double clip(double value, double lower, double upper) {
    return std::min(upper, std::max(value, lower));
}

std::vector<double> random_angle_steps(int steps, double irregularity) {
    constexpr double pi = 3.141592653589793238462643383279502884L;

    std::vector<double> angles;
    double lower = (pi * 2 / steps) - irregularity;
    double upper = (pi * 2 / steps) + irregularity;
    double cumsum = 0;

    for (int i = 0; i < steps; i++) {
        double angle = (double)rand() / RAND_MAX * (upper - lower) + lower;
        angles.push_back(angle);
        cumsum += angle;
    }

    cumsum /= pi * 2;
    for (int i = 0; i < steps; i++)
        angles[i] /= cumsum;

    return angles;
}

std::vector<std::vector<double>> create_polygon(
    std::vector<double> center,
    double avg_radius,
    double irregularity,
    double spikiness,
    int point_num
) {
    constexpr double pi = 3.141592653589793238462643383279502884L;
    irregularity *= pi * 2 / point_num;
    spikiness *= avg_radius;
    std::vector<double> angle_steps = random_angle_steps(point_num, irregularity);

    std::default_random_engine engine(time(0));
    std::normal_distribution<double> normal(avg_radius, spikiness);

    std::vector<std::vector<double>> polygon;
    double angle = (double)rand() / RAND_MAX * pi * 2;
    for (int i = 0; i < point_num; i++) {
        double radius = clip(normal(engine), 0, 2 * avg_radius);
        polygon.push_back(std::vector<double>{
            center[0] + radius * cos(angle), 
            center[1] + radius * sin(angle)
        });
        angle += angle_steps[i];
    }

    return polygon;
}