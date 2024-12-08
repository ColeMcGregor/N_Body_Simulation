#include <iostream>
#include <cmath>
#include <vector>
#include "../vector.h"
#include "../body.h"

using namespace std;

int passed_tests = 0;
int total_tests = 0;

void assert_equal(double expected, double actual, const std::string &message)
{
    total_tests++;
    if (std::fabs(expected - actual) < 1e-6)
    {
        passed_tests++;
        cout << ":) |" << message << endl;
    }
    else
    {
        cout << "Fuck you | " << message << endl;
    }
}

void assert_vector_equal(const Vector &expected, const Vector &actual, const std::string &message)
{
    total_tests++;
    if (std::fabs(expected.x - actual.x) < 1e-6 && 
        std::fabs(expected.y - actual.y) < 1e-6 && 
        std::fabs(expected.z - actual.z) < 1e-6)
    {
        passed_tests++;
        cout << ":) | " << message << endl;
    }
    else
    {
        cout << "Fuck you | " << message << endl;
    }
}

void test_gravForce()
{
    Vector pos1(0, 0, 0);
    Vector vel1(0, 0, 0);
    Vector accel1(0, 0, 0);
    Vector netForce1(0, 0, 0);
    vector<Vector> trajectory1;

    Vector pos2(1, 0, 0);
    Vector vel2(0, 0, 0);
    Vector accel2(0, 0, 0);
    Vector netForce2(0, 0, 0);
    vector<Vector> trajectory2;

    Body body1(pos1, vel1, accel1, netForce1, 5.0, 1.0, 1.0, "planet", {}, trajectory1);
    Body body2(pos2, vel2, accel2, netForce2, 10.0, 1.0, 1.0, "planet", {}, trajectory2);

    Vector expectedForce(6.6743e-10, 0, 0); // Use F = G * m1 * m2 / r^2 with G = 6.67430e-11, m1 = 5, m2 = 10, r = 1
    Vector actualForce = body1.gravForce(body2);

    assert_vector_equal(expectedForce, actualForce, "Gravitational force between two bodies");
}

void test_applyForce()
{
    Vector pos(0, 0, 0);
    Vector vel(0, 0, 0);
    Vector accel(0, 0, 0);
    Vector netForce(0, 0, 0);
    vector<Vector> trajectory;

    Body body(pos, vel, accel, netForce, 10.0, 1.0, 1.0, "planet", {}, trajectory);
    Vector force(20.0, 0, 0);
    body.applyForce(force);

    Vector expectedAccel(2.0, 0, 0); // a = F / m
    assert_vector_equal(expectedAccel, body.acceleration, "Applying force to body");
}

void test_update()
{
    Vector pos(0, 0, 0);
    Vector vel(0, 0, 0);
    Vector accel(1.0, 0, 0);
    Vector netForce(0, 0, 0);
    vector<Vector> trajectory;

    Body body(pos, vel, accel, netForce, 1.0, 1.0, 1.0, "planet", {}, trajectory);
    body.update(1.0);

    Vector expectedPosition(1.0, 0, 0); // position = 0 + velocity * 1 + 0.5 * acceleration * 1^2
    Vector expectedVelocity(1.0, 0, 0); // velocity = 0 + acceleration * 1

    assert_vector_equal(expectedPosition, body.position, "Updating position with timestep");
    assert_vector_equal(expectedVelocity, body.velocity, "Updating velocity with timestep");
}

void test_sumForces()
{
    Vector pos1(0, 0, 0);
    Vector vel1(0, 0, 0);
    Vector accel1(0, 0, 0);
    Vector netForce1(0, 0, 0);
    vector<Vector> trajectory1;

    Vector pos2(1, 0, 0);
    Vector vel2(0, 0, 0);
    Vector accel2(0, 0, 0);
    Vector netForce2(0, 0, 0);
    vector<Vector> trajectory2;

    Body body1(pos1, vel1, accel1, netForce1, 5.0, 1.0, 1.0, "planet", {}, trajectory1);
    Body body2(pos2, vel2, accel2, netForce2, 10.0, 1.0, 1.0, "planet", {}, trajectory2);

    vector<Body> bodies = {body1, body2};
    Vector expectedForce(6.6743e-10, 0, 0); // Same as gravForce test
    Vector actualForce = body1.sumForces(bodies);

    assert_vector_equal(expectedForce, actualForce, "Summing forces on body");
}


int main()
{
    test_gravForce();
    test_applyForce();
    test_update();
    test_sumForces();

    std::cout << "\nSummary: " << passed_tests << "/" << total_tests << " tests passed.\n";
    return (total_tests == passed_tests) ? 0 : 1;
}
