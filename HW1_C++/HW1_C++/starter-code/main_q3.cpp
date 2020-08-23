#include <iostream>
#include <random>
#include <set>

// TODO: add your function here //


int main()
{
    // Test with N(0,1) data.
    std::cout << "Generating N(0,1) data" << std::endl;
    std::set<double> data;
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 1.0);
    for (unsigned int i = 0; i < 1000; ++i)
        data.insert(distribution(generator));
    std::cout<<data.size()<<std::endl;
    // TODO: print out number of points in [2, 10] //
    double lb = 2;
    double ub = 10;
    int cnt = 0;
    std::set<double>::iterator ele;
    for (double ele:data){
        if(ele>=lb && ele<=ub){
            cnt++;
        }
    }

    std::cout<<cnt<<std::endl;

    return 0;
}
