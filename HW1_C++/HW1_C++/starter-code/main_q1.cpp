#include "matrix.hpp"


int main()
{
    // TODO: Write your tests here //
    MatrixSymmetric<double> a(4);
    MatrixSymmetric<double> b(4);
    int size = 4*5/2;
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            a(i,j) = i+j;
            b(i,j) = i*2+j;
        }
    }
    std::cout<<"Result: "<<std::endl;

    //MatrixSymmetric<double> c = a+b;
    std::cout<<"A: "<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<"B: "<<std::endl;
    std::cout<<b<<std::endl;
    //std::cout<<"C = A+B: "<<std::endl<<c<<std::endl;

    //std::cout<<"Result: "<<std::endl;
    MatrixSymmetric<double> d = a-b;
    std::cout<<"D = A-B: "<<std::endl<<d<<std::endl;

    MatrixSymmetric<double> e = a*b;
    std::cout<<"E = AXB: "<<std::endl<<d<<std::endl;

    MatrixSymmetric<double> f = a/b;
    std::cout<<"F = A/B: "<<std::endl<<d<<std::endl;
    return 0;
}