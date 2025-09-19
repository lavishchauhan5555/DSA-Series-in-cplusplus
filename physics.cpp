#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <iomanip>
#include <windows.h>


struct Vec2 {
    double x, y;
    Vec2(double a = 0, double b = 0) : x(a), y(b) {}
};

struct Circle {
    Vec2 pos, vel, acc;
    double radius, mass, restitution;
    bool isStatic;

    Circle(double x, double y, double r, double m = 1.0, double rest = 0.6, bool st = false)
        : pos(x, y), vel(0, 0), acc(0, 0),
          radius(r), mass(m), restitution(rest), isStatic(st) {}

    void applyForce(const Vec2 &f) {
        if (!isStatic) {
            acc.x += f.x / mass;
            acc.y += f.y / mass;
        }
    }

    void integrate(double dt) {
        if (isStatic) {
            acc = Vec2();
            return;
        }
        vel.x += acc.x * dt;
        vel.y += acc.y * dt;
        pos.x += vel.x * dt;
        pos.y += vel.y * dt;
        acc = Vec2(); // reset acceleration
    }
};

int main() {
    const double height = 700.0;  // ground level
    const double gravity = 980.0; // pixels/sec^2

    std::vector<Circle> bodies;
    bodies.emplace_back(200, 50, 20, 2.0, 0.7);
    bodies.emplace_back(260, 30, 16, 1.0, 0.85);
    bodies.emplace_back(320, 10, 24, 3.0, 0.5);

    const double dt = 1.0 / 60.0; // time step ~60 FPS

    for (int step = 0; step < 120; ++step) {
        // Apply gravity
        for (auto &b : bodies)
            b.applyForce(Vec2(0, gravity * b.mass));

        // Integrate positions
        for (auto &b : bodies)
            b.integrate(dt);

        // Ground collision
        for (auto &b : bodies) {
            if (b.pos.y + b.radius > height) {
                b.pos.y = height - b.radius; // snap to ground
                if (b.vel.y > 0)
                    b.vel.y = -b.vel.y * b.restitution; // bounce
                if (std::abs(b.vel.y) < 1e-1)
                    b.vel.y = 0; // stop tiny bounces
            }
        }

        // Print state
        std::cout << "Step " << step << '\n';
        for (size_t i = 0; i < bodies.size(); ++i) {
            auto &b = bodies[i];
            std::cout << "Body " << i << ": pos=("
                      << std::fixed << std::setprecision(2)
                      << b.pos.x << "," << b.pos.y
                      << ") vel=(" << b.vel.x << "," << b.vel.y << ")\n";
        }
        std::cout << "---------------------------------\n";

        // Slow down simulation for readability
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        Sleep(100);

    }

    return 0;
}
