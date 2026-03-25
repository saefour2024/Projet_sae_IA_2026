/**
  ******************************************************************************
  * @file    model.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SINGLE_FILE
#include "number.h"
#include "model.h"
// #include <chrono>

 // InputLayer is excluded
#include "conv2d_10.c"
#include "weights/conv2d_10.c" // InputLayer is excluded
#include "conv2d_11.c"
#include "weights/conv2d_11.c" // InputLayer is excluded
#include "flatten_5.c" // InputLayer is excluded
#include "dense_10.c"
#include "weights/dense_10.c"
#endif


void cnn(
  const input_t input,
  dense_10_output_type dense_10_output) {
  
  // Output array allocation
  static union {
    conv2d_10_output_type conv2d_10_output;
  } activations1;

  static union {
    conv2d_11_output_type conv2d_11_output;
    flatten_5_output_type flatten_5_output;
  } activations2;


// Model layers call chain 
  
  
  conv2d_10( // Model input is passed as model parameter
    input,
    conv2d_10_kernel,
    conv2d_10_bias,
    activations1.conv2d_10_output
    );
  
  
  conv2d_11(
    activations1.conv2d_10_output,
    conv2d_11_kernel,
    conv2d_11_bias,
    activations2.conv2d_11_output
    );
  
  
  flatten_5(
    activations2.conv2d_11_output,
    activations2.flatten_5_output
    );
  
  
  dense_10(
    activations2.flatten_5_output,
    dense_10_kernel,
    dense_10_bias,// Last layer uses output passed as model parameter
    dense_10_output
    );
}

#ifdef __cplusplus
} // extern "C"
#endif