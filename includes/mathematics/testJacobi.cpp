// #include "Jacobi44.hpp"
#include "LinearAlgebra.hpp"
#include <iostream>
#include <iomanip>
// #include <eigen3/Eigen/Core>
// #include <eigen3/Eigen/Eigenvalues>

using namespace coffeemill;
// using namespace Eigen;

int main()
{
    Matrix4 m(0e0);

    m(0,0) = 2e0;
    m(0,1) = 3e0;
    m(1,0) = 3e0;
    m(0,2) = 5e0;
    m(2,0) = 5e0;
    m(0,3) = 7e0;
    m(3,0) = 7e0;
    m(1,1) = 1e0;
    m(1,2) = 11e0;
    m(2,1) = 11e0;
    m(1,3) = 0e0;
    m(3,1) = 0e0;
    m(2,2) = 2e0;
    m(2,3) = 0e0;
    m(3,2) = 0e0;
    m(3,3) = 2e0;

    JacobiSolver<4> solver(m);
    std::cout << std::setprecision(16) << solver.get_eigenvalue(0) << std::endl;
    std::cout << solver.get_eigenvalue(1) << std::endl;
    std::cout << solver.get_eigenvalue(2) << std::endl;
    std::cout << solver.get_eigenvalue(3) << std::endl;
    std::cout << (solver.get_mineigenpair()).first << std::endl;

//     Matrix4d M = MatrixXd::Zero(4,4);
//
//     M(0,0) = 2e0;
//     M(0,1) = 3e0;
//     M(1,0) = 3e0;
//     M(0,2) = 5e0;
//     M(2,0) = 5e0;
//     M(0,3) = 7e0;
//     M(3,0) = 7e0;
//     M(1,1) = 1e0;
//     M(1,2) = 11e0;
//     M(2,1) = 11e0;
//     M(1,3) = 0e0;
//     M(3,1) = 0e0;
//     M(2,2) = 2e0;
//     M(2,3) = 0e0;
//     M(3,2) = 0e0;
//     M(3,3) = 2e0;
//
//     EigenSolver<Matrix4d> esolver(M);
//     std::cout << std::setprecision(16) << esolver.eigenvalues()(0) << std::endl;
//     std::cout << esolver.eigenvalues()(1) << std::endl;
//     std::cout << esolver.eigenvalues()(2) << std::endl;
//     std::cout << esolver.eigenvalues()(3) << std::endl;

    return 0;
}
