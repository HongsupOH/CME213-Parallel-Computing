#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include<iostream>
#include<ostream>
#include<stdexcept>
#include<cstring>

using uint = unsigned int; 

template <typename T>
class Matrix
{
    //public:
    //virtual int 10_norm() = 0;
    //virtual T& operator()(uint row, uint col) = 0; 
};


template <typename T>
class MatrixSymmetric: public Matrix<T>
{
public:
    MatrixSymmetric(std::size_t n){
        this->n = n;
        this->size = this->n*(this->n+1)/2;
        this->data = new T[this->size];
    };
    MatrixSymmetric(const MatrixSymmetric<T>& mat){
        this->n = mat.n;
        this->size = this->n*(this->n+1)/2;
        this->data = new T[this->size];
        std::memcpy (this->data, mat.data,mat.n*mat.n*sizeof(T));
    };

    ~MatrixSymmetric(){
        if (this->data){
            delete[] this->data;
            this->data = nullptr;
        }
    };

    MatrixSymmetric<T> operator  + (const MatrixSymmetric<T>& mat){
        if (this->n!= mat.n){
            throw std:: invalid_argument("Invalid matrix dimension.");
        }

        MatrixSymmetric<T> result(this->n);
        for (uint i=0; i<this->size;i++){
            result.data[i] = this->data[i] + mat.data[i];
        }
        return result;
    }

    MatrixSymmetric<T> operator  - (const MatrixSymmetric<T>& mat){
        if (this->n!= mat.n){
            throw std:: invalid_argument("Invalid matrix dimension.");
        }

        MatrixSymmetric<T> result(this->n);
        for (uint i=0; i<this->size;i++){
            result.data[i] = this->data[i] - mat.data[i];
        }
        return result;
    }

    MatrixSymmetric<T> operator  * (const MatrixSymmetric<T>& mat){
        if (this->n!= mat.n){
            throw std:: invalid_argument("Invalid matrix dimension.");
        }

        MatrixSymmetric<T> result(this->n);
        for (uint i=0; i<this->size;i++){
            result.data[i] = this->data[i] * mat.data[i];
        }
        return result;
    }

    MatrixSymmetric<T> operator  / (const MatrixSymmetric<T>& mat){
        if (this->n!= mat.n){
            throw std:: invalid_argument("Invalid matrix dimension.");
        }

        MatrixSymmetric<T> result(this->n);
        for (uint i=0; i<this->size;i++){
            result.data[i] = this->data[i] / mat.data[i];
        }
        return result;
    }

    const T& operator()(std::size_t row, std::size_t col) const{
        if (col>=row){
            return this-> data[this->n*row+col - (row+(row+1)/2)];
        }
        else{
            return this-> data[this->n*col+row - (col+(col+1)/2)];
        }
    }

    T& operator()(std::size_t row, std::size_t col){
        if (col>=row){
            return this-> data[this->n*row+col - (row+(row+1)/2)];
        }
        else{
            return this-> data[this->n*col+row - (col+(col+1)/2)];
        }
    }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& stream, MatrixSymmetric<U> mat){
        mat.print(stream);
        return stream;
    }
private:
    uint n;
    uint size;
    T*data;
    void print(std::ostream& stream){
        for (uint i=0; i<this->n;i++){
            for(uint j=0;j<this->n;j++){
                stream <<(*this)(i,j)<<" ";
            }
            stream<<std::endl;
        }
    }

};

#endif /* MATRIX_HPP */