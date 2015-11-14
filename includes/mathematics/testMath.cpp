#include "LinearAlgebra.hpp"

using namespace coffeemill;

int main()
{
    Realvec v1;
    Realvec v2(1e0);
    Realvec v3(1e0, 2e0, 3e0);
    Realvec v4(v2);
    Realvec v5(v2 + v3);
    Realvec v6(v2 - v3);
    Realvec v7(v2*2e0);
    Realvec v8(2e0*v2);
    Realvec v9(v2/2e0);
    Realvec vv5;
    Realvec vv6;
    Realvec vv7;
    Realvec vv8;
    Realvec vv9;
    vv5 = v2 + v3;
    vv6 = v2 - v3;
    vv7 = v2 * 2e0;
    vv8 = 2e0 * v2;
    vv9 = v2 / 2e0;

    std::cout << "v1                 : " << v1 << std::endl;
    std::cout << "v2(1e0)            : " << v2 << std::endl;
    std::cout << "v3(1e0, 2e0, 3e0)  : " << v3 << std::endl;
    std::cout << "v4(v2)             : " << v4 << std::endl;
    std::cout << "v5(v2+v3)          : " << v5 << std::endl;
    std::cout << "v6(v2-v3)          : " << v6 << std::endl;
    std::cout << "v7(v2*2e0)         : " << v7 << std::endl;
    std::cout << "v8(2e0*v2)         : " << v8 << std::endl;
    std::cout << "v9(v2/2e0)         : " << v9 << std::endl;
    std::cout << "v5 = v2 + v3       : " << vv5 << std::endl;
    std::cout << "v6 = v2 - v3       : " << vv6 << std::endl;
    std::cout << "v7 = v2 * 2e0      : " << vv7 << std::endl;
    std::cout << "v8 = 2e0 * v2      : " << vv8 << std::endl;
    std::cout << "v9 = v2 / 2e0      : " << vv9 << std::endl;
    std::cout << "length(v2)         : " << length(v2) << std::endl;
    std::cout << "len_square(v2)     : " << len_square(v2) << std::endl;
    std::cout << "dot_prod(v2, v3)   : " << dot_prod(v2, v3) << std::endl;
    Realvec temp;
    temp = cross_prod(v2, v3);
    std::cout << "cross_prod(v2, v3) : " << temp << std::endl;

//     Matrix3 m1;
//     Matrix3 m2(2e0);
//     Matrix3 m4(m2);
//     Matrix3 m5(m2 + m2);
//     Matrix3 m6(m2 - m2);
//     Matrix3 m7(m2*2e0);
//     Matrix3 m8(2e0*m2);
//     Matrix3 m9(m2/2e0);
//
//     std::cout << "m1                 : " << std::endl << m1 << std::endl;
//     std::cout << "m2(1e0)            : " << std::endl << m2 << std::endl;
//     std::cout << "m4(m2)             : " << std::endl << m4 << std::endl;
//     std::cout << "m5(m2+m2)          : " << std::endl << m5 << std::endl;
//     std::cout << "m6(m2-m2)          : " << std::endl << m6 << std::endl;
//     std::cout << "m7(m2*2e0)         : " << std::endl << m7 << std::endl;
//     std::cout << "m8(2e0*m2)         : " << std::endl << m8 << std::endl;
//     std::cout << "m9(m2/2e0)         : " << std::endl << m9 << std::endl;
//
//     m5 = m2 + m2;
//     m6 = m2 - m2;
//     m7 = m2 * 2e0;
//     m8 = 2e0 * m2;
//     m9 = m2 / 2e0;
//
//     std::cout << "m5 = m2 + m2       : "<< std::endl << m5 << std::endl;
//     std::cout << "m6 = m2 - m2       : "<< std::endl << m6 << std::endl;
//     std::cout << "m7 = m2 * 2e0      : "<< std::endl << m7 << std::endl;
//     std::cout << "m8 = 2e0 * m2      : "<< std::endl << m8 << std::endl;
//     std::cout << "m9 = m2 / 2e0      : "<< std::endl << m9 << std::endl;
//
//     m1(0,0) = 1e0;
//     m1(0,1) = 1e0;
//     m1(0,2) = 1e0;
//     m1(1,1) = 1e0;
//     m1(1,2) = 1e0;
//     m1(2,2) = 1e0;
//     std::cout << "m1: " << std::endl << m1 << std::endl;
//     Matrix3 m3temp;
//     m3temp = m1 * m2;
//     std::cout << "m1 * m2" << std::endl << m3temp << std::endl;
//     Matrix3 m10(m1 * m2);
//     std::cout << "m10(m1*m2): " << std::endl << m10 << std::endl;

    Matrix4 m1;
    Matrix4 m2(2e0);
    Matrix4 m3(1e0);
    Matrix4 m4(m2);
    Matrix4 m5(m2 + m3);
    Matrix4 m6(m2 - m3);
    Matrix4 m7(m2*2e0);
    Matrix4 m8(2e0*m2);
    Matrix4 m9(m2/2e0);

    std::cout << "m1                 : " << std::endl << m1 << std::endl;
    std::cout << "m2(1e0)            : " << std::endl << m2 << std::endl;
    std::cout << "m4(m2)             : " << std::endl << m4 << std::endl;
    std::cout << "m5(m2+m2)          : " << std::endl << m5 << std::endl;
    std::cout << "m6(m2-m2)          : " << std::endl << m6 << std::endl;
    std::cout << "m7(m2*2e0)         : " << std::endl << m7 << std::endl;
    std::cout << "m8(2e0*m2)         : " << std::endl << m8 << std::endl;
    std::cout << "m9(m2/2e0)         : " << std::endl << m9 << std::endl;

    m5 = m2 + m2;
    m6 = m2 - m2;
    m7 = m2 * 2e0;
    m8 = 2e0 * m2;
    m9 = m2 / 2e0;

    std::cout << "m5 = m2 + m2       : " << std::endl << m5 << std::endl;
    std::cout << "m6 = m2 - m2       : " << std::endl << m6 << std::endl;
    std::cout << "m7 = m2 * 2e0      : " << std::endl << m7 << std::endl;
    std::cout << "m8 = 2e0 * m2      : " << std::endl << m8 << std::endl;
    std::cout << "m9 = m2 / 2e0      : " << std::endl << m9 << std::endl;

    m1(0,0) = 1e0;
    m1(0,1) = 1e0;
    m1(0,2) = 1e0;
    m1(0,3) = 1e0;
    m1(1,1) = 1e0;
    m1(1,2) = 1e0;
    m1(1,3) = 1e0;
    m1(2,2) = 1e0;
    m1(2,3) = 1e0;
    m1(3,3) = 1e0;
    std::cout << "m1: " << std::endl << m1 << std::endl;
    Matrix4 m4temp;
    m4temp = m1 * m2;
    std::cout << "m1 * m2" << std::endl << m4temp << std::endl;
    Matrix4 m10(m1 * m2);
    std::cout << "m10(m1*m2): " << std::endl << m10 << std::endl;


    return 0;
}
