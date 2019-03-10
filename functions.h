#ifndef FUNCTIONS_INCLUDE
#define FUNCTIONS_INCLUDE
#include <vector>
std::vector<std::vector<float> > sameconvolution(std::vector<std::vector<float> > , std::vector<std::vector<float> > , int , int , int , int);
std::vector<std::vector<float> > validconvolution(std::vector<std::vector<float> > , std::vector<std::vector<float> > , int , int , int );
float* averagepool(float* , int );
float* convertedmatrix(float*, int , int );
float* matrixmult(float*, float*, int , int );
float* maxpool(float* , int );
float* relu(float*, int );
std::vector<float> sigmoid(std::vector<float> );
std::vector<float> softmax(std::vector<float> );
float* tanh(float*, int );
float* samemult(float* , float* , int , int );

#endif
