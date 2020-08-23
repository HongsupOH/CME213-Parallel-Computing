#include <iostream>
#include <string>
#include <vector>

/* TODO: Make Matrix a pure abstract class with the 
 * public method:
 *     std::string repr()
 */
class Matrix 
{
public:
    virtual std::string repr() = 0;
};

/* TODO: Modify the following function so that it 
 * inherits from the Matrix class */
class SparseMatrix: public Matrix 
{
public:
    std::string repr() 
    {
        return "sparse";
    }
};

/* TODO: Modify the following function so that it 
 * inherits from the Matrix class */
class ToeplitzMatrix : public Matrix
{
public:
    std::string repr() 
    {
        return "toeplitz";
    }
};

/* TODO: This function should accept a vector of Matrices and call the repr 
 * function on each matrix, printing the result to standard out. 
 */
void PrintRepr(std::vector<std::vector<double>> Mat)
{
    int m = Mat.size();
    int n = Mat[0].size();
    int total = m*n;
    int zero = 0;
    for (uint i=0;i<m;i++){
        for(uint j=0;j<n;j++){
            if(Mat[i][j]==0){
                zero++;
            }
        }
    }
    
    if (zero>=total/2){
        SparseMatrix sp;
        std::cout<<sp.repr()<<std::endl;
    }

    bool tp = true;
    for (uint i = 0;i<n;i++){
        int row = 0;
        int col = i;
        double piv = Mat[row][col];
        while (++row<m && ++col<n){
            if (Mat[row][col]!=piv){
                tp = false;
            }
        }
    }

    for (uint i = 0;i<m;i++){
        int row = i;
        int col = 0;
        double piv = Mat[row][col];
        while (++row<m && ++col<n){
            if (Mat[row][col]!=piv){
                tp = false;
            }
        }
    }
    
    if (tp==true){
        ToeplitzMatrix tm;
        tm.repr();
        std::cout<<tm.repr()<<std::endl;
    }
}

/* TODO: Your main method should fill a vector with an instance of SparseMatrix
 * and an instance of ToeplitzMatrix and pass the resulting vector
 * to the PrintRepr function.
 */ 
int main() 
{
    std::vector<std::vector<double>> mat{{5,6,0,0},
                                         {0,5,6,0},
                                         {0,0,5,6},
                                         {2,0,0,5}};
    PrintRepr(mat);
}
