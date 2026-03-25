#define SINGLE_FILE
/**
  ******************************************************************************
  * @file    defines.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, Université Côte d'Azur, LEAT, France
  * @version 2.1.0
  * @date    10 january 2024
  * @brief   Global C pre-processor definitions to use to build all source files (incl. CMSIS-NN)
  */

/* CMSIS-NN round mode definition */
#if defined(WITH_CMSIS_NN) || defined(WITH_NMSIS_NN)


#define ARM_NN_TRUNCATE 1
#define RISCV_NN_TRUNCATE 1

#endif // defined(WITH_CMSIS_NN) || defined(WITH_NMSIS_NN)
/**
  ******************************************************************************
  * @file    number.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    2 february 2021
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <stdint.h>
#include <stddef.h>
#include <math.h>

#ifdef TRAPV_SHIFT
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#endif

#ifdef WITH_CMSIS_NN
#include "arm_nnfunctions.h"
#endif

#define _clamp_to(type, number) clamp_to_number_t_ ## type (number)
#define clamp_to(type, number) _clamp_to(type, number)
#define _scale(type, number, scale_factor, round_mode) scale_number_t_ ## type (number, scale_factor, round_mode)
#define scale(type, number, scale_factor, round_mode) _scale(type, number, scale_factor, round_mode)
#define _scale_and_clamp_to(type, number, scale_factor, round_mode) scale_and_clamp_to_number_t_ ## type (number, scale_factor, round_mode)
#define scale_and_clamp_to(type, number, scale_factor, round_mode) _scale_and_clamp_to(type, number, scale_factor, round_mode)

typedef enum {
  ROUND_MODE_NONE,
  ROUND_MODE_FLOOR,
  ROUND_MODE_NEAREST,
} round_mode_t;

// Idea 1: Write the smallest min max interval of the net, could be an issue for hybrid int type network
// Idea 2: listing any interval and add type in name in a switch case like <- better but painfull
// #define NUMBER_MIN		// Max value for this numeric type
// #define NUMBER_MAX		// Min value for this numeric type

// // Idea 1: List of all types and write any corresponding function 
// typedef  number_t;		// Standard size numeric type used for weights and activations
// typedef  long_number_t;	// Long numeric type used for intermediate results

#define NUMBER_MIN_INT32_T -2147483648
#define NUMBER_MAX_INT32_T 2147483647

static inline int64_t min_int32_t(
    int64_t a,
    int64_t b) {
	if (a <= b)
		return a;
	return b;
}

static inline int64_t max_int32_t(
    int64_t a,
    int64_t b) {
	if (a >= b)
		return a;
	return b;
}

static inline int64_t scale_number_t_int32_t(
  int64_t number, int scale_factor, round_mode_t round_mode) {


  if (scale_factor <= 0) {
#ifdef TRAPV_SHIFT
    // Check for possible overflow of left shift
    if (number > INT64_MAX >> -scale_factor) {
      fprintf(stderr,
              "Error: scale() overflow, number=%ld, scale_factor=%d, limit=%d\n",
              number,
              scale_factor,
              INT16_MAX >> -scale_factor);
      assert(number <= INT64_MAX >> -scale_factor);
    }
#endif
    // No rounding to apply when shifting left
    return number << - scale_factor;
  } else {
    if (round_mode == ROUND_MODE_NEAREST) {
      number += (1 << (scale_factor - 1)); // +0.5 in fixed-point
    }
    return number >> scale_factor;
  }
}
static inline int32_t clamp_to_number_t_int32_t(
  int64_t number) {
	return (int32_t) max_int32_t(
      NUMBER_MIN_INT32_T,
      min_int32_t(
        NUMBER_MAX_INT32_T, number));
}
static inline int32_t scale_and_clamp_to_number_t_int32_t(
  int64_t number, int scale_factor, round_mode_t round_mode) {
#ifdef WITH_CMSIS_NN
  // Not really CMSIS-NN but use SSAT anyway
  if (scale_factor <= 0) {
    // No rounding to apply when shifting left
    return __SSAT(number << - scale_factor, sizeof(int32_t) * 8);
  } else {
    if (round_mode == ROUND_MODE_NEAREST) {
      number += (1 << (scale_factor - 1)); // +0.5 in fixed-point
    }
    return __SSAT(number >> scale_factor, sizeof(int32_t) * 8);
  }
#else
  number = scale_number_t_int32_t(number, scale_factor, round_mode);
  return clamp_to_number_t_int32_t(number);
#endif
}

#define NUMBER_MIN_INT16_T -32768
#define NUMBER_MAX_INT16_T 32767

static inline int32_t min_int16_t(
    int32_t a,
    int32_t b) {
	if (a <= b)
		return a;
	return b;
}

static inline int32_t max_int16_t(
    int32_t a,
    int32_t b) {
	if (a >= b)
		return a;
	return b;
}

static inline int32_t scale_number_t_int16_t(
  int32_t number, int scale_factor, round_mode_t round_mode) {


  if (scale_factor <= 0) {
#ifdef TRAPV_SHIFT
    // Check for possible overflow of left shift
    if (number > INT32_MAX >> -scale_factor) {
      fprintf(stderr,
              "Error: scale() overflow, number=%d, scale_factor=%d, limit=%d\n",
              number,
              scale_factor,
              INT16_MAX >> -scale_factor);
      assert(number <= INT32_MAX >> -scale_factor);
    }
#endif
    // No rounding to apply when shifting left
    return number << - scale_factor;
  } else {
    if (round_mode == ROUND_MODE_NEAREST) {
      number += (1 << (scale_factor - 1)); // +0.5 in fixed-point
    }
    return number >> scale_factor;
  }
}
static inline int16_t clamp_to_number_t_int16_t(
  int32_t number) {
	return (int16_t) max_int16_t(
      NUMBER_MIN_INT16_T,
      min_int16_t(
        NUMBER_MAX_INT16_T, number));
}
static inline int16_t scale_and_clamp_to_number_t_int16_t(
  int32_t number, int scale_factor, round_mode_t round_mode) {
#ifdef WITH_CMSIS_NN
  // Not really CMSIS-NN but use SSAT anyway
  if (scale_factor <= 0) {
    // No rounding to apply when shifting left
    return __SSAT(number << - scale_factor, sizeof(int16_t) * 8);
  } else {
    if (round_mode == ROUND_MODE_NEAREST) {
      number += (1 << (scale_factor - 1)); // +0.5 in fixed-point
    }
    return __SSAT(number >> scale_factor, sizeof(int16_t) * 8);
  }
#else
  number = scale_number_t_int16_t(number, scale_factor, round_mode);
  return clamp_to_number_t_int16_t(number);
#endif
}




static inline void int64_t_to_float(int64_t * tabint, float * tabfloat, long tabdim, int scale_factor){
  for (int i=0; i<tabdim; i++){
    tabfloat[i] = (float)tabint[i] / (1<<scale_factor);
  }
}

static inline void int32_t_to_float(int32_t * tabint, float * tabfloat, long tabdim, int scale_factor){
  for (int i=0; i<tabdim; i++){
    tabfloat[i] = (float)tabint[i] / (1<<scale_factor);
  }
}

static inline void int16_t_to_float(int16_t * tabint, float * tabfloat, long tabdim, int scale_factor){
  for (int i=0; i<tabdim; i++){
    tabfloat[i] = ((float)tabint[i]) / (1<<scale_factor);
  }
}

static inline void int8_t_to_float(int8_t * tabint, float * tabfloat, long tabdim, int scale_factor){
  for (int i=0; i<tabdim; i++){
    tabfloat[i] = ((float)tabint[i]) / (1<<scale_factor);
  }
}
#endif //__NUMBER_H__

#ifdef __cplusplus
} // extern "C"
#endif
/**
  ******************************************************************************
  * @file    conv2d.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    14 december 2022
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef _CONV2D_10_H_
#define _CONV2D_10_H_

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS      1
#define INPUT_HEIGHT        28
#define INPUT_WIDTH         28
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE_Y  3
#define CONV_KERNEL_SIZE_X  3
#define CONV_STRIDE_Y       2
#define CONV_STRIDE_X       2

#define ZEROPADDING_TOP    0
#define ZEROPADDING_BOTTOM 1
#define ZEROPADDING_LEFT   0
#define ZEROPADDING_RIGHT  1

#define CONV_OUTHEIGHT     ( ( (INPUT_HEIGHT - CONV_KERNEL_SIZE_Y + ZEROPADDING_TOP + ZEROPADDING_BOTTOM) / CONV_STRIDE_Y ) + 1 )
#define CONV_OUTWIDTH      ( ( (INPUT_WIDTH - CONV_KERNEL_SIZE_X + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE_X ) + 1 )


typedef int16_t conv2d_10_output_type[CONV_OUTHEIGHT][CONV_OUTWIDTH][CONV_FILTERS];

#if 0
void conv2d_10(
  const number_t input[INPUT_HEIGHT][INPUT_WIDTH][INPUT_CHANNELS],               // IN
  const number_t kernel[CONV_FILTERS][CONV_KERNEL_SIZE_X][CONV_KERNEL_SIZE_Y][INPUT_CHANNELS], // IN

  const number_t bias[CONV_FILTERS],						                // IN

  number_t output[CONV_OUTHEIGHT][CONV_OUTWIDTH][CONV_FILTERS]);               // OUT
#endif

#undef INPUT_CHANNELS
#undef INPUT_WIDTH
#undef INPUT_HEIGHT
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_STRIDE_X
#undef CONV_STRIDE_Y
#undef ZEROPADDING_TOP
#undef ZEROPADDING_BOTTOM
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTWIDTH
#undef CONV_OUTHEIGHT

#endif//_CONV2D_10_H_
/**
  ******************************************************************************
  * @file    conv2d.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    24 november 2021
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "conv2d_10.h"
#include "number.h"
#endif

#ifdef WITH_CMSIS_NN
#include "arm_nnfunctions.h"
#elif defined(WITH_NMSIS_NN)
#include "riscv_nnfunctions.h"
#endif

#define INPUT_CHANNELS      1
#define INPUT_HEIGHT        28
#define INPUT_WIDTH         28
#define CONV_FILTERS        8
#define CONV_KERNEL_SIZE_Y  3
#define CONV_KERNEL_SIZE_X  3
#define CONV_STRIDE_Y       2
#define CONV_STRIDE_X       2
#define CONV_GROUPS         1
#define CHANNELS_PER_GROUP  (INPUT_CHANNELS / CONV_GROUPS)
#define FILTERS_PER_GROUP   (CONV_FILTERS / CONV_GROUPS)

#define ZEROPADDING_TOP    0
#define ZEROPADDING_BOTTOM 1
#define ZEROPADDING_LEFT   0
#define ZEROPADDING_RIGHT  1

#define CONV_OUTHEIGHT     ( ( (INPUT_HEIGHT - CONV_KERNEL_SIZE_Y + ZEROPADDING_TOP + ZEROPADDING_BOTTOM) / CONV_STRIDE_Y ) + 1 )
#define CONV_OUTWIDTH      ( ( (INPUT_WIDTH - CONV_KERNEL_SIZE_X + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE_X ) + 1 )

#define ACTIVATION_RELU

// For fixed point quantization
#define WEIGHTS_SCALE_FACTOR 8
#define BIASES_SCALE_FACTOR 8
#define TMP_SCALE_FACTOR 8
#define INPUT_SCALE_FACTOR 8
#define OUTPUT_SCALE_FACTOR 8
#define OUTPUT_ROUND_MODE ROUND_MODE_FLOOR
#define NUMBER_T int16_t
#define LONG_NUMBER_T int32_t


static inline void conv2d_10(
  const NUMBER_T input[INPUT_HEIGHT][INPUT_WIDTH][INPUT_CHANNELS],               // IN
  const NUMBER_T kernel[CONV_FILTERS][CONV_KERNEL_SIZE_X][CONV_KERNEL_SIZE_Y][INPUT_CHANNELS / CONV_GROUPS], // IN

  const NUMBER_T bias[CONV_FILTERS],						                // IN

  NUMBER_T output[CONV_OUTHEIGHT][CONV_OUTWIDTH][CONV_FILTERS]) {               // OUT

#if !defined(WITH_CMSIS_NN) && !defined(WITH_NMSIS_NN)
  unsigned short pos_x, pos_y, z, k; 	// loop indexes for output volume
  unsigned short x, y;
  int input_x, input_y;
  LONG_NUMBER_T	kernel_mac;
  LONG_NUMBER_T tmp;
  static LONG_NUMBER_T	output_acc[CONV_OUTHEIGHT][CONV_OUTWIDTH];

  for (k = 0; k < CONV_FILTERS; k++) { 
    for (pos_y = 0; pos_y < CONV_OUTHEIGHT; pos_y++) { 
      for (pos_x = 0; pos_x < CONV_OUTWIDTH; pos_x++) { 
        output_acc[pos_y][pos_x] = 0;

        for (z = 0; z < INPUT_CHANNELS / CONV_GROUPS; z++) {
          kernel_mac = 0; 
            
          for (y = 0; y < CONV_KERNEL_SIZE_Y; y++) {
            input_y = pos_y * CONV_STRIDE_Y - ZEROPADDING_TOP + y;

            for (x = 0; x < CONV_KERNEL_SIZE_X; x++) {
              input_x = pos_x * CONV_STRIDE_X - ZEROPADDING_LEFT + x;

              if (input_x < 0 || input_x >= INPUT_WIDTH || input_y < 0 || input_y >= INPUT_HEIGHT) // ZeroPadding2D
                tmp = 0;
              else
                tmp = (LONG_NUMBER_T)input[input_y][input_x][z + (k / FILTERS_PER_GROUP) * CHANNELS_PER_GROUP] * (LONG_NUMBER_T)kernel[k][y][x][z];
              kernel_mac = kernel_mac + tmp;
            }
          }

          output_acc[pos_y][pos_x] = output_acc[pos_y][pos_x] + kernel_mac;

        }
      }
    }

    for (pos_y = 0; pos_y < CONV_OUTHEIGHT; pos_y++) { 
      for (pos_x = 0; pos_x < CONV_OUTWIDTH; pos_x++) { 
        // Scale for possible additional precision of bias
        output_acc[pos_y][pos_x] = scale(NUMBER_T, output_acc[pos_y][pos_x],  WEIGHTS_SCALE_FACTOR - TMP_SCALE_FACTOR, OUTPUT_ROUND_MODE);

        // Scale bias to match accumulator
        output_acc[pos_y][pos_x] += scale(NUMBER_T, (LONG_NUMBER_T)bias[k], BIASES_SCALE_FACTOR - TMP_SCALE_FACTOR - INPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);


#ifdef ACTIVATION_LINEAR
        output[pos_y][pos_x][k] = scale_and_clamp_to(NUMBER_T, output_acc[pos_y][pos_x], INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);
#elif defined(ACTIVATION_RELU) || defined(ACTIVATION_RELU6)
        // Activation function: ReLU
        if (output_acc[pos_y][pos_x] < 0) {
          output[pos_y][pos_x][k] = 0;
        } else {
#if defined(ACTIVATION_RELU6)
        if (output_acc[pos_y][pos_x] > scale(NUMBER_T, 6, -(INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR), OUTPUT_ROUND_MODE)) {
          output_acc[pos_y][pos_x] = scale(NUMBER_T, 6, -(INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR), OUTPUT_ROUND_MODE);
        }
#endif
          output[pos_y][pos_x][k] = scale_and_clamp_to(NUMBER_T, output_acc[pos_y][pos_x], INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);
        }
#else
#error "Unsupported activation function"
#endif
      }
    }
  }
#else

#if BIASES_SCALE_FACTOR > WEIGHTS_SCALE_FACTOR
#error "CMSIS-NN does not support BIASES_SCALE_FACTOR larger than WEIGHTS_SCALE_FACTOR"
#endif

  static q15_t bufferA[INPUT_HEIGHT*INPUT_WIDTH*INPUT_CHANNELS];
#ifdef WITH_CMSIS_NN
  arm_convolve_HWC_q15_basic_nonsquare(
#elif defined(WITH_NMSIS_NN)
  riscv_convolve_HWC_q15_basic_nonsquare(
#endif
                                      (q15_t*)input, //Im_in
                                      INPUT_WIDTH, //dim_im_in_x
                                      INPUT_HEIGHT, //dim_im_in_y
                                      INPUT_CHANNELS, //ch_im_in
                                      (q15_t*)kernel, //wt
                                      CONV_FILTERS, //ch_im_out
                                      CONV_KERNEL_SIZE_X, //dim_kernel_x
                                      CONV_KERNEL_SIZE_Y, //dim_kernel_y
                                      ZEROPADDING_LEFT, //padding_x, left and right must be equal
                                      ZEROPADDING_TOP, //padding_y, top and bottom must be equal
                                      CONV_STRIDE_X, //stride_x
                                      CONV_STRIDE_Y, //stride_y
                                      (q15_t*)bias, //bias
                                      INPUT_SCALE_FACTOR + WEIGHTS_SCALE_FACTOR - BIASES_SCALE_FACTOR, //bias_shift
                                      INPUT_SCALE_FACTOR + WEIGHTS_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, //out_shift
                                      (q15_t*)output, //Im_out
                                      CONV_OUTWIDTH, //dim_im_out_x
                                      CONV_OUTHEIGHT, //dim_im_out_y
                                      bufferA, //bufferA
                                      NULL //bufferB, unused
                                      );
#ifdef ACTIVATION_RELU
#ifdef WITH_CMSIS_NN
  arm_relu_q15((q15_t*)output, CONV_FILTERS * CONV_OUTHEIGHT * CONV_OUTWIDTH);
#elif defined(WITH_NMSIS_NN)
  riscv_relu_q15((q15_t*)output, CONV_FILTERS * CONV_OUTHEIGHT * CONV_OUTWIDTH);
#endif
#elif !defined(ACTIVATION_LINEAR)
#error "Unsupported activation with CMSIS-NN"
#endif


#endif
}

#undef INPUT_CHANNELS
#undef INPUT_WIDTH
#undef INPUT_HEIGHT
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_STRIDE_X
#undef CONV_STRIDE_Y
#undef CONV_GROUPS
#undef CHANNELS_PER_GROUP
#undef FILTERS_PER_GROUP
#undef ZEROPADDING_TOP
#undef ZEROPADDING_BOTTOM
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTWIDTH
#undef CONV_OUTHEIGHT
#undef ACTIVATION_RELU
#undef WEIGHTS_SCALE_FACTOR
#undef BIASES_SCALE_FACTOR
#undef TMP_SCALE_FACTOR
#undef INPUT_SCALE_FACTOR
#undef OUTPUT_SCALE_FACTOR
#undef NUMBER_T
#undef LONG_NUMBER_T
/**
  ******************************************************************************
  * @file    weights/conv2d.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#include <stdint.h>

#define INPUT_CHANNELS     1
#define CONV_FILTERS       8
#define CONV_KERNEL_SIZE_Y 3
#define CONV_KERNEL_SIZE_X 3
#define CONV_GROUPS        1


const int16_t conv2d_10_bias[CONV_FILTERS] = {-22, -7, -6, 28, -7, -3, -13, 1}
;


const int16_t conv2d_10_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{29}
, {-3}
, {77}
}
, {{31}
, {49}
, {122}
}
, {{-84}
, {76}
, {-39}
}
}
, {{{-86}
, {-115}
, {-84}
}
, {{100}
, {110}
, {138}
}
, {{-12}
, {74}
, {124}
}
}
, {{{-122}
, {-18}
, {57}
}
, {{-272}
, {23}
, {223}
}
, {{-148}
, {57}
, {195}
}
}
, {{{-96}
, {-208}
, {-95}
}
, {{-18}
, {-28}
, {-1}
}
, {{189}
, {205}
, {192}
}
}
, {{{90}
, {198}
, {-142}
}
, {{101}
, {164}
, {-16}
}
, {{-157}
, {-49}
, {87}
}
}
, {{{74}
, {-13}
, {-28}
}
, {{115}
, {172}
, {-127}
}
, {{182}
, {-25}
, {-86}
}
}
, {{{-155}
, {113}
, {-41}
}
, {{-62}
, {215}
, {22}
}
, {{118}
, {137}
, {111}
}
}
, {{{-51}
, {137}
, {12}
}
, {{53}
, {78}
, {168}
}
, {{50}
, {48}
, {-178}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS
/**
  ******************************************************************************
  * @file    conv2d.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    14 december 2022
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef _CONV2D_11_H_
#define _CONV2D_11_H_

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define INPUT_CHANNELS      8
#define INPUT_HEIGHT        14
#define INPUT_WIDTH         14
#define CONV_FILTERS        16
#define CONV_KERNEL_SIZE_Y  3
#define CONV_KERNEL_SIZE_X  3
#define CONV_STRIDE_Y       2
#define CONV_STRIDE_X       2

#define ZEROPADDING_TOP    0
#define ZEROPADDING_BOTTOM 1
#define ZEROPADDING_LEFT   0
#define ZEROPADDING_RIGHT  1

#define CONV_OUTHEIGHT     ( ( (INPUT_HEIGHT - CONV_KERNEL_SIZE_Y + ZEROPADDING_TOP + ZEROPADDING_BOTTOM) / CONV_STRIDE_Y ) + 1 )
#define CONV_OUTWIDTH      ( ( (INPUT_WIDTH - CONV_KERNEL_SIZE_X + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE_X ) + 1 )


typedef int16_t conv2d_11_output_type[CONV_OUTHEIGHT][CONV_OUTWIDTH][CONV_FILTERS];

#if 0
void conv2d_11(
  const number_t input[INPUT_HEIGHT][INPUT_WIDTH][INPUT_CHANNELS],               // IN
  const number_t kernel[CONV_FILTERS][CONV_KERNEL_SIZE_X][CONV_KERNEL_SIZE_Y][INPUT_CHANNELS], // IN

  const number_t bias[CONV_FILTERS],						                // IN

  number_t output[CONV_OUTHEIGHT][CONV_OUTWIDTH][CONV_FILTERS]);               // OUT
#endif

#undef INPUT_CHANNELS
#undef INPUT_WIDTH
#undef INPUT_HEIGHT
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_STRIDE_X
#undef CONV_STRIDE_Y
#undef ZEROPADDING_TOP
#undef ZEROPADDING_BOTTOM
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTWIDTH
#undef CONV_OUTHEIGHT

#endif//_CONV2D_11_H_
/**
  ******************************************************************************
  * @file    conv2d.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    24 november 2021
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "conv2d_11.h"
#include "number.h"
#endif

#ifdef WITH_CMSIS_NN
#include "arm_nnfunctions.h"
#elif defined(WITH_NMSIS_NN)
#include "riscv_nnfunctions.h"
#endif

#define INPUT_CHANNELS      8
#define INPUT_HEIGHT        14
#define INPUT_WIDTH         14
#define CONV_FILTERS        16
#define CONV_KERNEL_SIZE_Y  3
#define CONV_KERNEL_SIZE_X  3
#define CONV_STRIDE_Y       2
#define CONV_STRIDE_X       2
#define CONV_GROUPS         1
#define CHANNELS_PER_GROUP  (INPUT_CHANNELS / CONV_GROUPS)
#define FILTERS_PER_GROUP   (CONV_FILTERS / CONV_GROUPS)

#define ZEROPADDING_TOP    0
#define ZEROPADDING_BOTTOM 1
#define ZEROPADDING_LEFT   0
#define ZEROPADDING_RIGHT  1

#define CONV_OUTHEIGHT     ( ( (INPUT_HEIGHT - CONV_KERNEL_SIZE_Y + ZEROPADDING_TOP + ZEROPADDING_BOTTOM) / CONV_STRIDE_Y ) + 1 )
#define CONV_OUTWIDTH      ( ( (INPUT_WIDTH - CONV_KERNEL_SIZE_X + ZEROPADDING_LEFT + ZEROPADDING_RIGHT) / CONV_STRIDE_X ) + 1 )

#define ACTIVATION_RELU

// For fixed point quantization
#define WEIGHTS_SCALE_FACTOR 8
#define BIASES_SCALE_FACTOR 8
#define TMP_SCALE_FACTOR 8
#define INPUT_SCALE_FACTOR 8
#define OUTPUT_SCALE_FACTOR 8
#define OUTPUT_ROUND_MODE ROUND_MODE_FLOOR
#define NUMBER_T int16_t
#define LONG_NUMBER_T int32_t


static inline void conv2d_11(
  const NUMBER_T input[INPUT_HEIGHT][INPUT_WIDTH][INPUT_CHANNELS],               // IN
  const NUMBER_T kernel[CONV_FILTERS][CONV_KERNEL_SIZE_X][CONV_KERNEL_SIZE_Y][INPUT_CHANNELS / CONV_GROUPS], // IN

  const NUMBER_T bias[CONV_FILTERS],						                // IN

  NUMBER_T output[CONV_OUTHEIGHT][CONV_OUTWIDTH][CONV_FILTERS]) {               // OUT

#if !defined(WITH_CMSIS_NN) && !defined(WITH_NMSIS_NN)
  unsigned short pos_x, pos_y, z, k; 	// loop indexes for output volume
  unsigned short x, y;
  int input_x, input_y;
  LONG_NUMBER_T	kernel_mac;
  LONG_NUMBER_T tmp;
  static LONG_NUMBER_T	output_acc[CONV_OUTHEIGHT][CONV_OUTWIDTH];

  for (k = 0; k < CONV_FILTERS; k++) { 
    for (pos_y = 0; pos_y < CONV_OUTHEIGHT; pos_y++) { 
      for (pos_x = 0; pos_x < CONV_OUTWIDTH; pos_x++) { 
        output_acc[pos_y][pos_x] = 0;

        for (z = 0; z < INPUT_CHANNELS / CONV_GROUPS; z++) {
          kernel_mac = 0; 
            
          for (y = 0; y < CONV_KERNEL_SIZE_Y; y++) {
            input_y = pos_y * CONV_STRIDE_Y - ZEROPADDING_TOP + y;

            for (x = 0; x < CONV_KERNEL_SIZE_X; x++) {
              input_x = pos_x * CONV_STRIDE_X - ZEROPADDING_LEFT + x;

              if (input_x < 0 || input_x >= INPUT_WIDTH || input_y < 0 || input_y >= INPUT_HEIGHT) // ZeroPadding2D
                tmp = 0;
              else
                tmp = (LONG_NUMBER_T)input[input_y][input_x][z + (k / FILTERS_PER_GROUP) * CHANNELS_PER_GROUP] * (LONG_NUMBER_T)kernel[k][y][x][z];
              kernel_mac = kernel_mac + tmp;
            }
          }

          output_acc[pos_y][pos_x] = output_acc[pos_y][pos_x] + kernel_mac;

        }
      }
    }

    for (pos_y = 0; pos_y < CONV_OUTHEIGHT; pos_y++) { 
      for (pos_x = 0; pos_x < CONV_OUTWIDTH; pos_x++) { 
        // Scale for possible additional precision of bias
        output_acc[pos_y][pos_x] = scale(NUMBER_T, output_acc[pos_y][pos_x],  WEIGHTS_SCALE_FACTOR - TMP_SCALE_FACTOR, OUTPUT_ROUND_MODE);

        // Scale bias to match accumulator
        output_acc[pos_y][pos_x] += scale(NUMBER_T, (LONG_NUMBER_T)bias[k], BIASES_SCALE_FACTOR - TMP_SCALE_FACTOR - INPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);


#ifdef ACTIVATION_LINEAR
        output[pos_y][pos_x][k] = scale_and_clamp_to(NUMBER_T, output_acc[pos_y][pos_x], INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);
#elif defined(ACTIVATION_RELU) || defined(ACTIVATION_RELU6)
        // Activation function: ReLU
        if (output_acc[pos_y][pos_x] < 0) {
          output[pos_y][pos_x][k] = 0;
        } else {
#if defined(ACTIVATION_RELU6)
        if (output_acc[pos_y][pos_x] > scale(NUMBER_T, 6, -(INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR), OUTPUT_ROUND_MODE)) {
          output_acc[pos_y][pos_x] = scale(NUMBER_T, 6, -(INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR), OUTPUT_ROUND_MODE);
        }
#endif
          output[pos_y][pos_x][k] = scale_and_clamp_to(NUMBER_T, output_acc[pos_y][pos_x], INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);
        }
#else
#error "Unsupported activation function"
#endif
      }
    }
  }
#else

#if BIASES_SCALE_FACTOR > WEIGHTS_SCALE_FACTOR
#error "CMSIS-NN does not support BIASES_SCALE_FACTOR larger than WEIGHTS_SCALE_FACTOR"
#endif

  static q15_t bufferA[INPUT_HEIGHT*INPUT_WIDTH*INPUT_CHANNELS];
#ifdef WITH_CMSIS_NN
  arm_convolve_HWC_q15_basic_nonsquare(
#elif defined(WITH_NMSIS_NN)
  riscv_convolve_HWC_q15_basic_nonsquare(
#endif
                                      (q15_t*)input, //Im_in
                                      INPUT_WIDTH, //dim_im_in_x
                                      INPUT_HEIGHT, //dim_im_in_y
                                      INPUT_CHANNELS, //ch_im_in
                                      (q15_t*)kernel, //wt
                                      CONV_FILTERS, //ch_im_out
                                      CONV_KERNEL_SIZE_X, //dim_kernel_x
                                      CONV_KERNEL_SIZE_Y, //dim_kernel_y
                                      ZEROPADDING_LEFT, //padding_x, left and right must be equal
                                      ZEROPADDING_TOP, //padding_y, top and bottom must be equal
                                      CONV_STRIDE_X, //stride_x
                                      CONV_STRIDE_Y, //stride_y
                                      (q15_t*)bias, //bias
                                      INPUT_SCALE_FACTOR + WEIGHTS_SCALE_FACTOR - BIASES_SCALE_FACTOR, //bias_shift
                                      INPUT_SCALE_FACTOR + WEIGHTS_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, //out_shift
                                      (q15_t*)output, //Im_out
                                      CONV_OUTWIDTH, //dim_im_out_x
                                      CONV_OUTHEIGHT, //dim_im_out_y
                                      bufferA, //bufferA
                                      NULL //bufferB, unused
                                      );
#ifdef ACTIVATION_RELU
#ifdef WITH_CMSIS_NN
  arm_relu_q15((q15_t*)output, CONV_FILTERS * CONV_OUTHEIGHT * CONV_OUTWIDTH);
#elif defined(WITH_NMSIS_NN)
  riscv_relu_q15((q15_t*)output, CONV_FILTERS * CONV_OUTHEIGHT * CONV_OUTWIDTH);
#endif
#elif !defined(ACTIVATION_LINEAR)
#error "Unsupported activation with CMSIS-NN"
#endif


#endif
}

#undef INPUT_CHANNELS
#undef INPUT_WIDTH
#undef INPUT_HEIGHT
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_STRIDE_X
#undef CONV_STRIDE_Y
#undef CONV_GROUPS
#undef CHANNELS_PER_GROUP
#undef FILTERS_PER_GROUP
#undef ZEROPADDING_TOP
#undef ZEROPADDING_BOTTOM
#undef ZEROPADDING_LEFT
#undef ZEROPADDING_RIGHT
#undef CONV_OUTWIDTH
#undef CONV_OUTHEIGHT
#undef ACTIVATION_RELU
#undef WEIGHTS_SCALE_FACTOR
#undef BIASES_SCALE_FACTOR
#undef TMP_SCALE_FACTOR
#undef INPUT_SCALE_FACTOR
#undef OUTPUT_SCALE_FACTOR
#undef NUMBER_T
#undef LONG_NUMBER_T
/**
  ******************************************************************************
  * @file    weights/conv2d.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#include <stdint.h>

#define INPUT_CHANNELS     8
#define CONV_FILTERS       16
#define CONV_KERNEL_SIZE_Y 3
#define CONV_KERNEL_SIZE_X 3
#define CONV_GROUPS        1


const int16_t conv2d_11_bias[CONV_FILTERS] = {14, -7, -95, 8, -93, 14, -23, -8, -59, -73, 9, -45, -25, -103, -170, 11}
;


const int16_t conv2d_11_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{62, 10, -67, -109, 67, 80, 85, 113}
, {-35, -72, -61, -219, 137, -35, -91, -25}
, {-123, -109, -82, -81, -167, -178, -107, -99}
}
, {{-148, -137, -43, -74, -96, -140, -161, -163}
, {-89, 69, 53, 67, -121, -144, -20, -47}
, {28, 96, 45, 134, 25, -2, 102, 33}
}
, {{21, 39, 57, 120, -66, 68, 110, 74}
, {67, 46, 21, -14, 48, 58, 18, 93}
, {39, 43, 8, 53, -5, 73, -33, 19}
}
}
, {{{-42, -153, 57, -31, 42, -14, -37, -63}
, {-86, -113, 80, -25, -91, -30, -86, -85}
, {-89, 7, 111, 81, -7, -4, -41, -94}
}
, {{34, 96, -9, 64, -7, 60, 57, 44}
, {52, 84, 55, 74, 70, 89, 121, 88}
, {37, -7, -85, -63, 74, 54, 32, 61}
}
, {{-3, -19, -148, -143, 58, 37, -108, -41}
, {39, -60, -36, -177, 96, -16, -64, -26}
, {9, -148, -128, -26, -9, 7, -63, -13}
}
}
, {{{-9, 21, -8, -2, 77, -32, 3, -97}
, {-32, 52, -60, 99, -76, -54, 14, -98}
, {-60, -122, 78, 3, -92, -18, 0, 6}
}
, {{59, 47, -48, -67, 72, 0, 84, 25}
, {57, 10, -58, -56, 14, 61, 22, 94}
, {-1, 53, -26, 10, 23, -4, 53, 48}
}
, {{12, 45, -59, -2, -64, 10, 35, -32}
, {46, 7, 28, -28, -68, 38, 21, 43}
, {25, -29, -58, -70, 19, 30, 49, 49}
}
}
, {{{34, -35, 129, -34, 51, 127, -6, 68}
, {-99, -71, -118, -152, -91, -90, -95, -43}
, {38, 57, 80, 66, 10, -59, 65, -3}
}
, {{-17, -136, -94, -75, 100, 50, -148, -38}
, {-51, 8, 55, -30, -174, -148, -60, -121}
, {26, 60, 119, 13, 43, 49, 111, 69}
}
, {{-104, -55, 70, -110, -63, -58, -144, -98}
, {-26, 47, 102, 38, -51, -56, 6, -23}
, {35, 20, 19, -3, 2, 47, 18, 69}
}
}
, {{{5, -32, 121, -82, -22, -29, 9, 22}
, {21, 15, 196, -30, 84, 3, 76, 11}
, {33, -5, 112, -30, -28, 50, 14, 34}
}
, {{1, -12, 141, -43, -22, 17, 32, 35}
, {13, -35, 130, -171, 85, 49, 2, -45}
, {51, 15, 72, -112, 49, 14, 24, -1}
}
, {{-84, -69, 20, -48, 44, -6, -63, -64}
, {-59, -49, 50, -118, -74, -100, -16, -89}
, {87, 54, 75, -17, 41, 50, 15, 11}
}
}
, {{{-122, -82, 122, -92, -102, 11, -256, -215}
, {-79, 11, 24, 6, -153, -86, -26, -69}
, {-74, 10, -18, -25, -44, -28, 26, -23}
}
, {{-68, -85, 28, -70, -128, -212, -215, -193}
, {25, 81, 101, 84, 91, 53, 71, 13}
, {48, -19, 32, -34, -20, 81, -17, 104}
}
, {{-132, -78, 124, -63, -142, -54, -102, -120}
, {83, 69, 189, -20, 6, 10, 20, 38}
, {-9, 18, 97, -10, 104, 54, 36, 44}
}
}
, {{{66, -13, 39, 9, 27, 61, 14, 55}
, {-5, -55, -91, -58, -78, -4, -59, 7}
, {-70, 50, 153, -23, -72, -80, -181, -164}
}
, {{101, 66, 100, -7, 103, 124, 77, 85}
, {-103, -232, 81, -80, -113, -25, -142, -83}
, {-30, 43, 108, 8, -171, -300, -39, -79}
}
, {{58, 13, 62, -42, 65, 87, 132, 129}
, {-86, -103, 55, -79, -44, -56, -115, -126}
, {-15, 72, 162, -9, -92, -82, 13, -13}
}
}
, {{{8, 13, -72, 38, -43, 37, 62, 25}
, {50, 19, -85, 13, 7, 67, 47, 11}
, {28, 77, 9, 13, -31, 17, 25, -17}
}
, {{70, 1, -3, 35, 2, 20, 113, 42}
, {-5, 27, 88, 37, 53, 57, 64, 72}
, {29, -17, -42, -97, 91, 54, 35, 55}
}
, {{-53, -79, -40, -109, 55, 8, -108, -86}
, {-53, -82, -349, -56, -77, -24, -108, -65}
, {-73, -57, -105, -17, 47, -70, -107, -106}
}
}
, {{{14, 23, 62, 2, -29, -13, 5, -33}
, {39, 38, 176, 9, 29, 36, 29, 32}
, {-135, -194, -173, -100, -87, -18, -77, -63}
}
, {{-23, -30, 137, -129, -1, -103, 37, -43}
, {90, 38, 242, -31, 84, 57, 97, 77}
, {-142, -186, 58, -80, -37, 28, -151, -77}
}
, {{-63, 1, 20, -48, -76, -49, 54, -25}
, {82, -33, 193, -31, 53, 83, 105, 96}
, {22, 10, 79, 10, -37, -30, -108, -57}
}
}
, {{{-71, -71, 27, 20, 31, 13, -45, -61}
, {3, -58, 87, -150, 70, 7, 51, 58}
, {-4, 18, -24, 27, 54, 20, 6, -50}
}
, {{-47, -85, -167, -7, 14, -34, -81, -49}
, {41, -42, 99, 16, -26, -50, 16, -7}
, {89, -12, 38, -124, 66, 101, 45, 65}
}
, {{37, 42, 57, 66, -146, -100, -17, 24}
, {-26, 40, 28, -56, 88, 124, 41, 37}
, {-18, -67, 48, -78, 39, 13, 56, -11}
}
}
, {{{-9, -9, 8, 105, 29, -23, 61, 2}
, {-68, -1, -16, 82, -50, -27, 48, -71}
, {-137, 8, -143, 40, -82, -53, -25, -92}
}
, {{39, 47, 40, -36, 117, -13, 18, 35}
, {85, 108, 57, 110, 108, 131, 68, 107}
, {105, 121, 90, 80, 42, 86, 59, 57}
}
, {{-145, -240, -125, -192, -194, -210, -238, -171}
, {-37, -165, -59, -310, 66, -126, -87, -11}
, {-3, -37, -38, -87, 19, 31, -84, -22}
}
}
, {{{50, 35, 23, 43, -29, 14, 72, 16}
, {37, -33, -2, -62, 103, 36, 61, 85}
, {-24, -22, -215, -100, 0, 28, -91, -11}
}
, {{-2, 73, 105, -34, -31, -46, 57, 30}
, {44, -1, -47, -92, 24, 90, 35, 69}
, {-65, -276, -71, -187, -46, -35, -117, -52}
}
, {{16, -16, -13, -34, 31, 86, 13, 15}
, {24, -51, -74, -63, -10, 16, 16, -21}
, {-67, -154, -126, -131, -31, 41, -113, 28}
}
}
, {{{-56, 9, -45, 65, -62, 56, -38, -45}
, {-60, -68, 9, -36, -163, -44, -43, -29}
, {-11, -72, 165, -77, -21, -51, -18, -4}
}
, {{74, 107, -159, 88, 135, 53, 97, -2}
, {-11, 11, -199, 145, -48, 17, 27, -49}
, {-104, -78, 7, -38, -83, -47, -88, -57}
}
, {{79, -21, 8, -71, 76, -69, 31, -18}
, {79, 11, -76, 75, 127, 62, 52, 90}
, {-29, 45, -310, 81, 0, 80, 21, 2}
}
}
, {{{-24, -25, -11, -15, -81, -47, -58, -57}
, {33, 43, -67, -29, 133, 59, 82, 72}
, {20, 34, -146, -15, -22, 61, 3, 5}
}
, {{5, 24, -3, -39, -79, -123, -36, -80}
, {78, 41, -28, -12, 66, 32, 92, 36}
, {-8, -29, -36, -108, 35, 33, -35, 22}
}
, {{2, 66, -132, -32, -30, -21, 63, 11}
, {44, -11, -79, -11, 99, -1, 33, 16}
, {34, -35, -138, -28, -22, 40, 10, 6}
}
}
, {{{-78, -22, -145, -1, 46, -33, -126, -50}
, {-8, 14, -7, 31, -62, -126, -33, -56}
, {92, 56, 136, -7, 15, 92, 45, 83}
}
, {{-26, 63, 99, 13, -109, -140, -7, -15}
, {16, 32, 104, 25, 36, 105, 104, 114}
, {48, -6, -90, -37, -32, 56, -20, 86}
}
, {{39, 4, 16, 23, -73, 79, 120, 71}
, {3, -9, 7, -31, -3, 23, -38, -54}
, {-77, -91, 36, -81, 25, -39, -45, -56}
}
}
, {{{112, 106, -31, -25, 61, 104, 40, 111}
, {95, 115, -85, 70, 112, 70, 62, 87}
, {123, 134, -16, 24, 69, 95, 46, 128}
}
, {{-79, -133, 5, -138, 1, -80, -174, -21}
, {-5, -183, -96, -249, 38, -23, -110, 18}
, {-33, -75, -39, -48, -17, -60, -129, -54}
}
, {{-170, -82, -15, 5, -130, -58, -106, -93}
, {-89, 29, 62, 12, -126, -170, -7, -109}
, {-17, 10, -16, 129, -131, -65, 46, -43}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS
/**
  ******************************************************************************
  * @file    flatten.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    24 january 2023
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef _FLATTEN_5_H_
#define _FLATTEN_5_H_

#ifndef SINGLE_FILE
#include "number.h"
#endif

#define OUTPUT_DIM 784

typedef int16_t flatten_5_output_type[OUTPUT_DIM];

#if 0
void flatten_5(
  const number_t input[7][7][16], 			      // IN
	number_t output[OUTPUT_DIM]); 			                // OUT
#endif

#undef OUTPUT_DIM

#endif//_FLATTEN_5_H_
/**
  ******************************************************************************
  * @file    flatten.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 2.0.0
  * @date    26 november 2021
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "flatten_5.h"
#include "number.h"
#endif

#define OUTPUT_DIM 784

#define NUMBER_T int16_t
#define LONG_NUMBER_T int32_t

static inline void flatten_5(
  const NUMBER_T input[7][7][16], 			      // IN
	NUMBER_T output[OUTPUT_DIM]) {			                // OUT

  NUMBER_T *input_flat = (NUMBER_T *)input;

  // Copy data from input to output only if input and output don't point to the same memory address already
  if (input_flat != output) {
    for (size_t i = 0; i < OUTPUT_DIM; i++) {
      output[i] = input_flat[i];
    }
  }
}

#undef OUTPUT_DIM
#undef NUMBER_T
#undef LONG_NUMBER_T
/**
  ******************************************************************************
  * @file    fc.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version V2.0
  * @date    24 january 2023
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef _DENSE_10_H_
#define _DENSE_10_H_

#ifndef SINGLE_FILE
#include "number.h"
#include <stdint.h>
#endif

#define INPUT_SAMPLES 784
#define FC_UNITS 32

typedef int16_t dense_10_output_type[FC_UNITS];

#if 0
void dense_10(
  const number_t input[INPUT_SAMPLES], 			      // IN
	const number_t kernel[FC_UNITS][INPUT_SAMPLES],  // IN

	const number_t bias[FC_UNITS],			              // IN

	number_t output[FC_UNITS]); 			                // OUT
#endif

#undef INPUT_SAMPLES
#undef FC_UNITS

#endif//_DENSE_10_H_
/**
  ******************************************************************************
  * @file    fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#ifndef SINGLE_FILE
#include "dense_10.h"
#include "number.h"
#endif

#ifdef WITH_CMSIS_NN
#include "arm_nnfunctions.h"
#elif defined(WITH_NMSIS_NN)
#include "riscv_nnfunctions.h"
#endif

#define INPUT_SAMPLES 784
#define FC_UNITS 32
#define ACTIVATION_RELU

// For fixed point quantization
#define WEIGHTS_SCALE_FACTOR 8
#define BIASES_SCALE_FACTOR 8
#define TMP_SCALE_FACTOR 8
#define INPUT_SCALE_FACTOR 8
#define OUTPUT_SCALE_FACTOR 8
#define OUTPUT_ROUND_MODE ROUND_MODE_FLOOR
#define NUMBER_T int16_t
#define LONG_NUMBER_T int32_t


static inline void dense_10(
  const NUMBER_T input[INPUT_SAMPLES], 			      // IN
	const NUMBER_T kernel[FC_UNITS][INPUT_SAMPLES],  // IN

	const NUMBER_T bias[FC_UNITS],			              // IN

	NUMBER_T output[FC_UNITS]) {			                // OUT

#if !defined(WITH_CMSIS_NN) && !defined(WITH_NMSIS_NN)
  unsigned short k, z; 
  LONG_NUMBER_T output_acc;

  for (k = 0; k < FC_UNITS; k++) { 
    output_acc = 0;
    for (z = 0; z < INPUT_SAMPLES; z++) 
      output_acc = output_acc + ((LONG_NUMBER_T)kernel[k][z] * (LONG_NUMBER_T)input[z]);

    output_acc = scale(NUMBER_T, output_acc, WEIGHTS_SCALE_FACTOR - TMP_SCALE_FACTOR, OUTPUT_ROUND_MODE);

    output_acc += scale(NUMBER_T, (LONG_NUMBER_T)bias[k], BIASES_SCALE_FACTOR - TMP_SCALE_FACTOR - INPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);


    // Activation function
#ifdef ACTIVATION_LINEAR
    // Linear (MEANS NONE)
    output[k] = scale_and_clamp_to(NUMBER_T, output_acc, INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);
#elif defined(ACTIVATION_RELU) || defined(ACTIVATION_RELU6)
    // ReLU
    if (output_acc < 0) {
      output[k] = 0;
    } else {
#if defined(ACTIVATION_RELU6)
      if (output_acc > scale(NUMBER_T, 6, -(INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR), OUTPUT_ROUND_MODE)) {
        output_acc = scale(NUMBER_T, 6, -(INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR), OUTPUT_ROUND_MODE);
      }
#endif
      output[k] = scale_and_clamp_to(NUMBER_T, output_acc, INPUT_SCALE_FACTOR + TMP_SCALE_FACTOR - OUTPUT_SCALE_FACTOR, OUTPUT_ROUND_MODE);
    }
#else
#error "Unsupported activation function"
#endif
  }
#else

#if BIASES_SCALE_FACTOR > WEIGHTS_SCALE_FACTOR
#error "CMSIS-NN does not support BIASES_SCALE_FACTOR larger than WEIGHTS_SCALE_FACTOR"
#endif

  static q15_t bufferA[INPUT_SAMPLES];
#ifdef WITH_CMSIS_NN
  arm_fully_connected_q15(
#elif defined(WITH_NMSIS_NN)
  riscv_fully_connected_q15(
#endif
                             (q15_t*)input,
                             (q15_t*)kernel,
                             INPUT_SAMPLES,
                             FC_UNITS,
                             INPUT_SCALE_FACTOR + WEIGHTS_SCALE_FACTOR - BIASES_SCALE_FACTOR,
                             INPUT_SCALE_FACTOR + WEIGHTS_SCALE_FACTOR - OUTPUT_SCALE_FACTOR,
                             (q15_t*)bias,
                             (q15_t*)output,
                             (q15_t*)bufferA);
#ifdef ACTIVATION_RELU
#ifdef WITH_CMSIS_NN
  arm_relu_q15((q15_t*)output, FC_UNITS);
#elif defined(WITH_NMSIS_NN)
  riscv_relu_q15((q15_t*)output, FC_UNITS);
#endif
#elif !defined(ACTIVATION_LINEAR)
#error "Unsupported activation with CMSIS-NN"
#endif


#endif
}

#undef INPUT_SAMPLES
#undef FC_UNITS
#undef ACTIVATION_RELU
#undef WEIGHTS_SCALE_FACTOR
#undef BIASES_SCALE_FACTOR
#undef INPUT_SCALE_FACTOR
#undef OUTPUT_SCALE_FACTOR
#undef NUMBER_T
#undef LONG_NUMBER_T
/**
  ******************************************************************************
  * @file    weights/fc.cc
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    24 march 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */

#include <stdint.h>

#define INPUT_SAMPLES 784
#define FC_UNITS 32


const int16_t dense_10_bias[FC_UNITS] = {0, -13, 60, 13, -40, -36, 89, 79, 2, -57, -90, 26, -12, -44, 12, -3, -19, 32, 101, -24, 27, -43, -44, 55, -45, -12, 32, -2, -22, -43, -24, -2}
;

const int16_t dense_10_kernel[FC_UNITS][INPUT_SAMPLES] = {{37, -29, 74, 14, 126, 81, 93, 37, -88, -4, -41, 28, -92, 21, 5, -11, 52, -122, 130, -8, -24, 32, 28, 26, 3, -11, -35, 117, 106, -56, -55, -77, -18, 15, -10, 98, -9, 86, 50, 187, -13, -55, 35, 164, 12, 6, 34, 95, 53, 6, 31, 44, 72, 124, 130, 21, 142, 39, 77, 105, 52, 183, 80, 37, 58, -31, 27, -7, 61, 63, 42, 6, 32, 51, 22, -29, -35, 103, 75, -6, -66, -3, 9, 29, 140, -1, 60, 87, 0, -152, 97, -123, 6, -8, -89, 7, -26, -14, 46, -27, 4, -18, -8, -19, -27, 13, 59, -10, 18, -30, -114, -29, 35, 58, -46, 22, -36, 28, 43, -42, 48, -53, -37, -91, -63, -100, 20, 67, 59, -40, -9, -36, 46, -15, 51, -17, 62, -17, -4, -3, 137, 22, 3, 74, 40, -2, 20, 11, -69, 0, -60, -43, -75, 4, 25, -126, -59, 11, 22, -141, 67, 37, -25, 16, -26, -10, 51, 4, 16, -40, -62, 92, -18, 39, 63, 36, -76, 32, 22, 9, -85, 146, 33, -10, 26, -8, -7, 1, 6, 8, 34, 45, -75, 4, 5, -29, -15, 44, -27, -16, 65, -41, 6, 61, 14, 35, -25, 13, -55, -45, -30, 18, -23, -1, -45, -19, -69, -103, -72, 9, -37, -75, -124, 3, 11, 76, 29, -15, 23, 51, 92, -67, 31, 43, -22, -90, 95, 137, 28, 42, -57, -86, -20, -11, -24, 44, -28, -80, 90, 52, 39, 0, 30, 120, 21, 57, -75, 19, 39, 23, 59, -37, 62, -15, -25, 15, -62, -32, -57, 2, 30, -25, -12, -12, 38, -39, -81, -5, -14, 60, 36, -5, -75, -56, 64, 2, 44, -24, 64, 53, -129, 62, 0, 80, -20, 36, -51, 76, -26, 22, -48, -22, 47, -83, 100, 33, 25, 152, -41, -178, -5, 62, 75, 17, 4, -3, -76, 90, 223, 18, -123, 81, 85, -75, -24, -20, -27, -16, -92, -13, -69, 14, 21, 19, 18, -25, -38, 1, -112, 10, 49, -126, 18, 67, 186, 81, -2, 51, -128, 109, -66, 52, 34, 50, 0, 69, -4, 36, -103, -32, -18, -6, 9, 6, -115, -57, -38, 13, 5, 39, 96, 110, 64, -20, 36, -42, 14, 107, -129, 78, -54, -24, 101, -124, 68, -38, 51, -82, 16, 147, 16, -13, -24, -68, -59, -56, 110, -26, 35, -67, -10, -25, -32, -115, -30, -130, 41, -13, -49, -62, 53, -38, 23, -55, -25, -75, 146, -46, -89, -13, -17, -27, -39, -20, 14, -55, -94, -27, -6, -63, 0, 47, -84, -80, -66, -94, -39, -13, -63, -64, -38, -27, -36, 11, -82, -34, -106, -56, 33, -2, -80, -1, -36, -73, 72, 43, -128, -85, -63, 100, -71, -43, 1, 43, 90, 136, 5, 58, 30, 26, 114, -119, -10, 81, -43, -69, -141, -63, 60, -90, -55, -53, -46, 62, 100, 55, 4, -108, 56, 47, -52, 75, -73, 40, 7, -155, -44, 87, 24, -43, -3, -89, 36, 30, 19, 18, 86, -17, 7, 27, -60, -66, -90, -18, 4, -84, 85, -69, 40, -17, 23, 19, 67, 44, 82, 3, -28, -5, 62, 12, 6, -24, -26, 88, -4, -20, 13, 22, -67, -21, 20, 11, -126, 37, -60, 23, -46, -82, 72, 2, 18, 11, 3, 16, -47, 0, 21, 16, -76, -45, -39, 193, -65, -2, -71, 2, 82, 131, -138, -15, -58, -74, 19, -53, -33, -48, -108, -113, -109, 100, 5, 1, -32, 10, 26, 17, -3, -22, 104, 63, -39, 8, -276, 2, -14, -9, 47, 13, -10, 22, -28, 37, 11, -53, 53, 25, -50, -34, -56, 26, 10, -126, 50, -66, -28, 29, 33, 60, 42, 59, 57, -2, -64, 49, -18, -39, -76, -43, 7, 57, 28, 31, 54, -150, 17, -8, 22, -32, 94, -5, -8, -28, -6, -21, 24, -86, -33, 38, 54, -152, 59, 15, -41, 1, -122, -9, -2, -1, -13, -66, -28, 18, -110, -59, 59, 21, -57, -25, 10, 23, -5, -51, -2, 13, 37, 12, 36, -26, 7, 32, 13, 34, 48, 9, 118, -113, 41, 39, -138, -99, 9, -27, -14, -85, -115, -12, -36, -30, 6, 2, -3, -10, 57, 21, -1, -103, -1, -20, 49, -103, -16, -66, -121, 26, 24, -16, -33, -36, 56, 29, 33, -59, -42, -16, 3, -155, -21, 2, -58, 11, -58, 51, -290, -80, -71, 49, -76, 12, -8, -45, -23, -44, -70, -19, -6, -22, 32, 5, -63, -95, -137, -49, 46, -11, -4, -22, -85, -43, -53, 29, -74, -15, 4, -167, -111, -73, 24, -47, -71, 44, 6, -32, -11, -85, -37, 45, 4, 3, 15, 12, 19, -15, -9, -33}
, {79, 51, -18, 57, -34, 40, 89, -24, 44, -84, 39, 23, 11, 7, -4, 64, 22, -74, -98, 41, 70, -8, -80, -10, -6, -32, -71, -76, 85, -80, -19, 9, 24, -61, 37, -1, 145, -51, 20, -109, -29, 17, -120, 11, -14, -28, 30, -26, -24, 138, 50, 14, 8, -13, 22, -31, 90, 38, 8, 58, 23, 2, 68, 118, -40, -20, -70, -55, -18, -64, 87, -4, 13, -20, 14, 14, 13, -70, 22, -103, -7, -39, 62, -45, -16, -73, -143, 73, 43, 78, 75, -85, 1, 151, 47, -126, -13, 35, -20, 36, 19, 38, 10, 77, 7, -16, -25, -5, 29, -67, -5, 4, -6, 58, 73, 54, 67, 39, -23, -16, -56, 86, 20, 124, -56, 52, 13, 41, 1, 8, 86, 6, 23, 65, -74, 3, 44, 34, -54, 3, 1, 71, 33, -61, 1, 29, 44, 83, -3, 37, 20, 89, 53, -3, -33, 122, -4, 13, 6, 32, -153, 63, 48, -79, 87, -62, -36, 75, -8, -72, 9, 19, 51, -26, 2, 102, -64, -9, 12, -27, 29, -219, -27, -100, -26, -116, -16, -43, 19, 12, -97, 55, -38, -22, -71, -114, -87, -224, -16, -66, -8, -147, -66, 11, -1, -104, -27, -36, -82, -51, 29, -15, 14, 50, -70, -113, 14, 39, -35, 6, -21, -17, 67, -71, -20, 45, -54, -15, 10, -38, -12, 35, 90, -10, 2, -57, -9, -161, 14, -103, 17, -17, 14, 3, 62, 14, 67, 17, 4, 70, 12, 9, -97, 53, -35, -15, -27, -17, -45, -139, -2, 16, 123, 47, 120, -69, -83, 68, -29, 13, -20, 62, -98, -61, -118, -19, 85, 62, 81, -156, -108, -31, -22, -185, -59, -16, -142, 59, -33, -79, -100, 12, 137, -52, -45, -26, 34, -57, -64, -55, 50, -20, -231, -35, -121, -26, -1, -71, 32, 88, 31, -46, 22, -15, 102, 19, -2, -34, -171, -8, 42, -19, 24, 4, 34, 27, -12, -21, 34, -5, 89, 0, -4, -22, -32, 11, -71, 15, 81, -60, -8, -62, -71, 9, 4, -3, -8, 19, -2, 3, -3, 38, -40, -45, -86, -17, 58, -57, -26, 43, -47, -19, -20, 23, 79, 14, -58, -21, 67, -14, -54, -111, -12, -75, -37, 45, 22, -87, -19, -51, -8, -27, -114, 67, -15, -20, -139, 108, 21, 56, 35, -79, 79, 69, 39, -28, -62, -58, -51, 13, -15, -50, 92, 151, -39, 125, 3, -170, 28, 48, -23, 12, 9, 90, 23, -43, -44, 72, -2, 41, -10, 78, 77, -16, -113, 102, -38, -1, 92, -5, -21, -9, -67, -2, 3, 77, 14, 27, 8, 28, 21, 13, 19, -16, 41, 34, 126, -6, 40, 4, -1, -38, -73, 3, -34, -66, -10, -24, -52, -127, 51, -86, -74, -53, 98, 53, 76, -8, -70, -13, -108, -31, -37, -58, -34, -46, 33, -84, -56, -21, 100, -79, 67, -34, -77, 17, -90, -120, 89, -89, -42, -52, 40, -22, -146, 12, -5, -22, 28, 33, -109, 66, 28, -172, -47, 6, -176, -44, -31, 68, 102, -125, -120, 14, 47, -4, -59, 25, 5, 38, -46, -48, -17, -2, -75, 20, 35, -54, -87, 26, 30, -11, -86, -116, 20, 38, -17, -5, 49, 12, 64, -13, 57, 39, 9, 76, -8, -49, -10, 43, -39, -43, 4, 5, -76, -15, -63, 12, 8, 32, -4, -1, 15, -35, 33, 1, 42, 18, 93, 45, -10, 113, -51, 112, 32, 44, 99, 71, 68, -19, -10, 61, 30, -3, 61, 33, 39, 51, -1, -49, 51, -38, 252, 18, -12, -113, 46, -40, 28, 39, 9, 123, 61, 52, 70, 35, 68, 28, 106, 47, 48, -176, -12, -64, 22, 69, -64, -193, 64, 62, -68, -24, 65, 65, 61, 101, 90, -10, -6, 16, -6, 4, -150, -65, -2, 31, 50, -91, 8, 8, 9, -21, 5, -25, 143, -131, -60, -7, 109, 36, -21, 89, -47, 38, 124, 52, 105, -22, 37, 91, 7, 30, 63, 53, 3, 12, 26, 7, -11, -8, -17, 106, -13, 11, 6, -18, -55, 19, -34, -41, 8, 21, -45, 28, -14, 25, -83, -74, -97, -50, -32, -73, 33, -14, -63, -54, -22, 104, -44, 29, -137, 54, -173, 29, 67, 26, -22, -63, 28, -6, 145, -5, -153, 12, -81, -78, -7, -21, -168, 25, -68, -26, -42, -73, -21, 46, -31, 65, -77, 13, -60, -11, 79, 14, -151, 101, 45, 15, 29, 19, 27, 53, 45, 32, -59, -120, -33, 68, -16, -77, -92, 75, -28, 13, 18, -6, 34, 53, -55, -71, 77, 30, -20, -37, -81, 42, -3, -40, 107, -13, 17, 1, -13, 58, 56, -30, -17, 1, -24, 15, 12, -4, 3, 71}
, {-32, -3, -86, 14, 17, 5, 52, 2, -4, -50, -125, 15, -92, 25, 17, 14, -74, -141, -64, -32, 15, -39, 25, -19, -23, -58, -204, -93, 4, 63, -108, 54, 70, -101, 21, 93, -75, 62, -35, 15, 91, 25, 12, -51, 56, -146, -131, 30, 81, -93, -46, -4, -22, 39, 47, -77, 7, -18, 16, -50, 63, 3, -9, 62, 5, -32, 15, 18, 86, -3, 20, 17, 44, 2, 92, 22, -7, 72, 35, -106, -34, -28, 17, 35, 24, 30, 11, -39, 10, -97, 46, -85, -59, -67, 24, 62, 6, 43, -18, -31, 11, 17, 54, -6, -72, 20, -77, 44, -9, -63, -31, 12, 56, -48, -67, 75, -10, 12, 161, -48, -100, 22, 5, -80, -167, -77, -62, -140, -9, 20, 6, 24, -54, 10, 58, -56, -123, 28, 2, -108, 28, -25, 23, -134, 79, 0, -28, -23, 92, -18, -95, -46, -146, -26, -26, -23, -106, 7, 12, -116, 70, 16, 89, -86, -5, -4, -53, -30, 61, 23, -20, 12, 32, -26, 49, -11, -8, 57, 46, -38, -49, 52, -46, -61, 33, -1, 29, -44, 70, -1, -6, -33, 18, 18, 41, -71, 6, 22, -17, -27, 60, -14, 61, -18, 50, 22, -3, 8, 89, 39, 49, 60, -42, 28, 78, 10, -74, -27, 44, -7, 12, -76, -8, -25, 61, 10, 1, -15, -75, 19, 18, -76, 160, 12, 39, 44, 117, 92, 12, -41, -76, -85, -107, -27, 16, 30, 40, -154, 47, -11, -69, -142, -73, 70, -2, -14, -23, -27, 5, 50, 86, -39, -41, -109, -51, -7, 31, -122, -56, -12, -8, -92, -3, 21, 31, -62, -23, -31, -57, 24, 40, 14, -119, -17, 67, -10, -22, -12, -11, 81, 16, -9, -102, -56, 14, 75, -20, 53, -100, 51, -73, 19, 64, -78, -31, 59, 87, 57, -24, 0, -5, 79, 163, 26, 13, 99, -11, 204, 37, 4, -1, 35, 157, 41, 11, 14, 76, 83, -114, -17, 26, 42, 51, 43, -17, 35, -22, -127, -16, -45, -52, 19, 22, -88, 104, 21, 34, -177, 4, 79, -130, -50, -99, -20, 42, 16, 34, 12, 94, -14, 10, 24, -7, -273, 9, -84, -4, -5, -13, 22, -30, -7, 31, -21, 70, -31, 47, 103, -3, 55, 92, -47, 97, -29, -31, -25, 36, -50, 65, -40, 45, 16, 14, -79, -23, 21, 38, -3, 51, -5, -51, -169, -63, 30, 77, -164, 52, -41, -56, -81, -52, -20, -62, -20, -208, 30, -6, -75, -55, 11, 38, -24, -3, -65, -40, -100, -192, -71, -62, -113, -131, 89, -86, 19, 12, -21, -76, 20, 28, -51, 18, 8, -1, 32, -33, 7, -114, -70, -41, -6, -30, 38, 29, 2, 31, -44, -11, 23, 54, 25, -2, -13, 54, 116, 42, -34, -69, 62, 19, -3, 12, -12, 25, 37, 9, -149, 126, -38, 27, 23, 97, 12, -49, 71, 103, 23, -15, 9, -12, -22, 110, 80, -36, -21, -41, -14, -18, 39, -43, 39, -7, -18, 25, -82, 69, -10, -72, 27, 2, -41, -116, -63, -157, -99, -27, 36, 42, -37, 23, -80, 107, 54, -70, 5, 39, 16, -12, -252, -106, -32, -124, 49, -59, 99, 25, -55, 1, 85, -63, -66, -21, 83, -51, -75, -27, -60, -28, 16, 37, 14, -1, 16, 68, 9, -6, -80, 20, 21, -42, -29, -78, 101, -58, -37, -79, -82, -94, -15, -159, -81, -73, -114, -11, -63, -12, 2, -35, -57, -15, -28, -4, 6, 31, 25, -89, 39, 89, -156, 120, 39, -91, 47, -42, 77, 83, -21, -14, 80, -43, 16, -16, 14, 33, -34, 103, 16, 44, -15, 53, 90, -8, 16, 33, 35, 48, 27, -8, 96, -6, 55, 99, -3, 66, 74, 39, -11, 11, -38, 5, 180, 66, 37, 46, -20, 65, 42, -20, 38, 144, -32, -29, -95, -19, 24, -44, -12, 11, 7, -17, -9, -14, -31, -73, -31, -84, -57, -60, -14, 9, -71, -13, -12, -100, -88, -1, -1, -67, -46, -16, 5, -14, -88, 4, 4, 11, 30, -30, 33, -52, 10, -58, 29, 69, -84, 24, 58, 40, -46, -153, 104, -78, 56, -51, -112, 125, -15, -47, -54, 36, -102, -20, -45, 66, -30, -3, 51, -80, -50, 29, -74, 29, 82, 92, -33, 105, 68, -109, 31, 29, 70, 73, -1, 7, 101, 18, -67, 38, 82, -26, 59, -6, 30, -8, -19, -14, 30, -76, -39, -61, 52, 28, 48, 42, 41, 10, 116, 102, 1, 6, 96, 91, 5, -12, -3, -9, 23, -17, 26, 10, 14, -87, 13, 42, 38, 87, -15, -1, 29, -22, -13, -2, -14, 22, 48, -11, 21, 22, 1, 44, 20, -3, -9, -14, -34}
, {56, -70, 96, 33, -7, 32, 21, -20, -9, 58, -20, -10, 19, -3, 29, 32, -5, 27, -15, 14, 77, -21, 109, 124, -28, -63, -40, 139, 14, -45, 52, -56, -22, 28, -27, -31, 36, 63, 75, 27, 82, 82, -60, 115, -1, 122, 79, 56, 5, 26, -10, 90, 91, 33, 20, 60, 76, -37, 111, 83, -16, 116, 19, 0, 27, 26, 110, 117, 4, 126, 137, 137, 101, 98, 17, 59, 1, 208, 43, 47, 54, -7, -4, 80, -79, 164, 20, 49, 58, 75, -28, 4, -33, 43, -45, 61, 120, -16, -35, 7, -2, -59, 39, 10, 8, 101, 42, 48, -20, -51, 68, 4, -50, 67, 19, 3, -5, 7, -46, -39, 73, 127, 85, 126, 46, -2, 46, -36, -24, -117, -7, 7, 24, -5, -12, 38, 57, -42, -17, 182, 31, -22, -53, 0, -79, -34, -3, 33, 93, 16, 5, 18, 91, -49, 61, 111, 146, 59, -36, -14, -40, -32, -49, 121, 78, 116, 111, 46, 45, -57, 24, -66, -88, 53, -2, 58, 51, -14, -74, 147, -22, 108, -12, 48, 37, 71, -91, 43, -68, 126, 43, 135, 69, -101, -36, 273, 15, 165, -47, 2, -44, 191, -85, -48, -74, 15, -25, 18, 80, -116, 20, 20, 68, -71, -31, 40, 92, 70, 82, 48, -46, 57, 55, -8, -64, -89, -111, -66, -25, -89, 2, 23, 50, -89, -41, -37, 148, -103, -84, 69, 0, 42, -64, -43, 49, -14, -3, 49, -21, -16, 131, 22, 93, -6, -54, 80, 31, 5, -12, -27, -49, -57, -140, -15, -42, -17, 67, -141, 101, -43, -91, 15, 94, 57, -33, -6, -100, 80, -66, 42, -49, 5, -2, 33, 98, 13, 18, -26, 140, 64, 16, -63, -18, -62, -41, -8, -53, -82, -57, -20, 34, -86, 44, 13, 188, 59, -48, 33, 33, -108, -52, 26, -44, -60, 6, -45, -24, -58, 55, 90, -8, 29, -66, -9, 79, -71, -37, 33, 146, 111, 39, 56, 49, 51, 144, -12, 58, 146, 74, -37, -47, 27, 76, 42, -20, 1, 17, 95, -22, 15, 42, -1, 134, 142, 84, -2, -58, -11, 4, -60, -72, 6, 27, 0, 50, -55, 18, 69, 19, 79, 77, 62, 17, -15, -126, 36, -102, 47, 55, 0, -83, -42, 53, 1, 20, 67, 169, -5, -28, 78, 126, 68, -1, 75, 61, 38, 54, 106, 38, 43, -8, 48, -5, -1, -105, -86, 10, -12, 2, -45, 80, -17, 103, -35, -3, -13, 87, 28, -49, -88, -4, 41, -36, -31, -32, -19, 43, -50, 44, -34, 8, -4, 84, 107, 73, -37, 68, -92, -9, 51, -3, -14, 23, 79, 39, 69, 198, 0, -20, 56, 1, 16, 28, -8, -25, 72, -8, 27, 6, 83, -77, 167, 19, -38, 43, 20, -7, 3, 75, 33, 50, 12, -33, 35, 20, 82, -9, 34, 18, -63, -7, 37, 7, 51, -38, 76, 24, 13, 48, 27, -49, 155, 29, 85, 108, 13, 91, -5, 21, 13, -3, -15, -59, 45, -4, 56, 38, -45, -1, 54, 9, 22, 58, 1, -17, -69, 63, 36, 43, 4, -20, 7, 75, -13, 164, -88, -70, 60, 165, 149, 44, 13, 49, 52, -21, -7, 75, 47, 73, 11, 7, 22, -44, -4, 60, 51, 102, -8, 11, -69, 43, 9, 39, -7, 46, 81, 11, 15, -94, 46, -226, 34, -81, -81, -4, -40, -31, 40, 155, 8, -24, 163, 1, 57, -11, -13, 1, -126, -121, 62, 19, -61, -50, -39, -59, -17, -69, 4, 30, -4, -30, -22, 2, -2, -71, 4, 15, -7, -39, -1, 56, -16, -11, -49, -89, 36, 18, 12, -68, -22, -37, -48, 30, 54, -63, 24, -33, 9, -2, 25, 0, -21, -21, 57, 6, 26, 0, 11, 84, -117, -55, 2, 124, 41, 39, 9, -17, 119, 21, 91, 75, 78, 31, 54, 34, 91, 29, 16, 8, 83, -2, -25, 101, 40, 149, 83, -32, 123, 88, -5, 43, -92, 34, 49, 83, 2, 9, 63, 14, 13, 13, 17, -14, -33, 24, -35, -44, -88, 38, 35, 41, -42, 28, 15, -110, -1, -138, -122, -181, -103, -25, -2, -149, -34, -71, -104, -174, -72, -29, -214, -31, -101, -128, -149, -35, -125, 11, -117, -39, -17, -85, -60, -15, 89, -56, -37, 42, 3, -170, -11, -56, -127, 6, -90, -2, -67, -44, -98, -87, 64, -28, -29, -131, 29, -99, -42, -53, 22, -10, -42, -87, -50, -53, -59, -214, -103, -33, -69, 14, -183, -65, -35, 14, 26, 7, 69, 68, -30, -4, -35, 102, 58, -12, -19, -154, 60, -9, 17, 133, 4, 7, 41, 19, -59, 82, 20, 3, 10, 20, -1, -3, 12, -13, 115}
, {-45, -19, -84, -61, 59, -40, 7, -31, -41, -88, -93, -8, 13, 14, -4, 32, 16, -93, -113, -51, -18, -49, -36, -122, 26, -42, -33, -110, 1, -101, -73, -97, -42, -30, 3, -39, -95, -72, -129, -148, -62, -127, -35, -70, -58, -33, 14, -67, -3, 44, 8, 55, -41, 67, 77, -14, 29, 63, -46, 13, -46, 77, -1, 23, 11, 57, 54, -14, 5, 96, 49, -44, 0, 61, -19, 4, 54, -71, 95, -7, -26, 119, -49, 38, -162, 146, 60, -58, -14, -85, 49, 98, 67, -203, -10, 61, 21, 17, 19, -37, -10, -50, 28, 31, 17, 37, -17, -13, 25, -25, 36, 19, 106, -20, 121, 71, -21, 85, -78, -87, 13, 29, 36, -93, 158, -71, -78, -38, -52, 39, 80, -4, -43, 9, -20, -62, 152, -54, 48, -13, 97, 15, -19, 10, -40, 10, 30, -58, -94, -55, 152, -64, 66, -58, -62, -8, 77, -109, -76, 30, -3, 77, -30, 63, 19, 133, 49, -103, 5, 57, -34, 35, -66, -57, -38, 21, -114, 67, 69, 59, -60, 55, 91, -50, 48, 41, 41, 94, 25, -3, 13, -162, 3, 57, 44, -38, -22, -46, -74, -5, 45, 34, -16, -5, 5, 48, 44, -48, -126, 12, 151, -29, 9, -43, -16, 31, 10, -95, 113, 40, 49, -38, -139, 58, 36, -82, 37, -24, 146, -11, 33, 24, -173, 23, -91, -16, -49, 123, -39, 125, 8, -6, 85, 30, 43, -30, 27, 19, 37, 96, 36, -11, 10, 60, 56, 86, -47, 49, 76, -105, -40, 41, 157, 41, 38, 74, 50, 103, 113, -11, 39, -32, 46, -106, -175, 63, 125, 3, -28, -2, 60, 53, 118, -88, -96, -86, -69, -60, -48, -138, -40, 3, -38, 71, 59, -1, 22, 18, 94, -36, -1, -28, -62, -81, 35, -81, -45, -76, -22, 103, -44, -79, 39, 41, 72, 31, 38, -1, -28, -94, 87, 40, -15, -102, -55, -53, 5, -64, -53, -41, -82, -92, 5, -40, 97, -170, 30, 85, -4, 38, 11, 11, -78, 61, 86, 28, 12, 224, -34, 4, 101, 79, 39, -41, -6, -46, -28, -2, -82, 19, 86, 22, -16, 29, -30, 57, -12, 7, -16, -5, -57, -31, -39, -210, 7, 11, 112, -140, -36, 33, 35, -38, -136, -12, -96, -1, -122, 9, -19, 47, 4, -143, 43, -1, 134, -77, -8, -43, -72, 31, -25, -53, -58, -10, 47, 140, -52, -58, -29, 74, -143, -65, -39, 47, -98, 151, -18, 10, 95, -138, -30, 39, -12, -2, 52, 131, 61, 21, 0, 52, 31, 41, -68, 17, 5, 61, -7, -60, 83, 41, -27, -96, 54, -1, 12, -59, -80, 20, -39, 108, 74, -24, 19, 29, 13, 6, 62, -36, 6, 91, 23, -8, -11, -20, 81, 51, 17, 12, -30, 12, -52, 88, 8, -28, 24, 102, -28, 97, 32, 31, 3, 0, 26, -10, -115, -43, 45, -46, 10, -118, 9, -56, -37, -166, -120, -15, -25, 20, 12, 121, -46, 18, 20, -14, 13, 31, 53, -25, -36, -5, 13, -89, 44, 115, 49, 22, -21, 103, -60, 24, -39, -33, 77, -33, -23, 46, 94, 2, -52, -56, -34, -63, -6, -83, 41, -1, 45, 37, 0, 68, -1, 76, 64, 38, 41, -19, -65, 40, -17, -72, 37, 27, -88, -13, -37, -99, 43, -26, 8, -7, -27, 89, -71, 9, -21, -22, 9, 52, -117, 38, -60, -77, -65, -41, -3, -22, 33, -42, 77, -56, -18, 9, 58, 50, -7, -35, 82, 39, -95, -7, 25, -12, -85, -6, 28, -21, -2, -31, -87, -19, 42, 0, -19, -99, 3, -21, -40, -5, 28, -19, 39, 25, 27, 23, 115, 10, 54, -15, 13, 64, -15, 16, 19, 5, 89, -6, -8, 1, 47, -35, 28, 12, -75, 94, -25, 15, 1, -45, 133, 68, 14, -1, -41, 34, 13, -21, 32, 93, -73, -115, 29, 54, -4, -28, -121, 72, 127, -150, -50, 42, -28, -61, -36, -1, 35, 5, 27, 63, -65, -12, 13, 69, -20, -16, -20, -22, -19, -31, -31, 49, 3, -14, -10, 14, -35, -7, -30, -116, -46, -7, -5, 21, 101, 69, -97, 25, -5, 101, -1, -6, -9, -24, 91, 90, 2, -59, 55, -27, 70, 54, -50, -13, -7, 10, 1, 33, -44, 45, 41, 26, 30, 104, 46, 58, -11, 60, 50, -19, -27, -24, -16, -43, -34, -92, 104, 7, 43, -21, 92, -72, -22, -46, -21, 55, 122, -68, -45, 101, -83, -15, -78, 23, 5, -10, 84, -77, -51, -48, -54, -67, -19, -92, -12, -78, -122, -150, 10, -41, -107, -19, 26, -45, -1, -94, -68, 4, -1, -19, -11, -1, 11, -9, -17, -113}
, {103, -70, 74, 91, 61, 39, 1, 49, -21, 71, 22, 29, 69, 19, 1, 31, 15, 41, 14, 37, 129, -12, 1, 60, 29, 11, 96, 146, 83, 67, 32, -90, -8, -79, -9, 5, 97, 25, 121, -13, -50, 22, -7, 170, -36, 4, -7, -47, -53, 2, 34, 6, -43, -5, 77, -11, 93, 2, 122, 89, 44, -58, 25, 68, -58, -12, 87, 14, -47, 13, 20, 132, 62, 23, 9, 83, -30, 107, -24, -34, -97, 24, 52, 33, 190, -14, -102, 122, 11, 76, 156, 20, -51, 149, -12, 8, -66, 109, 18, 63, -4, -19, -53, 117, 91, 4, 97, 52, 69, -2, -123, -3, -16, 25, -5, -5, 31, -5, -39, -24, 185, 96, 65, 19, 111, -6, 74, 54, -88, -15, -45, -33, 34, -20, 2, -38, 47, -49, 46, 120, 32, 2, -11, -10, -43, 1, -55, 62, 45, -32, 118, 19, 58, 2, -2, 91, 40, -21, -1, -57, -21, -45, -14, -20, -73, 79, 98, 32, -51, -39, 8, 3, -35, 16, 48, 67, 34, -32, -79, -15, -41, 27, -46, 17, -94, -113, -54, -27, -44, -70, 0, 6, -61, -39, -123, 81, 45, 16, -157, -119, 45, 49, -84, -97, -54, -11, 31, 94, 120, -7, -78, -29, 27, -1, -88, -31, 116, 113, -88, -176, -17, 66, 31, 25, -34, -125, -176, -34, -14, -93, 82, 31, 81, -11, -142, -104, -43, -18, -55, -18, 58, 19, 45, 9, -3, 60, 116, -28, 113, 31, 41, 120, 44, -7, 26, 68, 94, -37, 61, 73, -74, 78, 15, -133, -46, 34, -104, -3, 35, 8, 81, -19, 114, -28, 17, -37, -203, 58, 41, -2, -77, -32, 70, 15, 83, -7, -39, -95, 59, 53, 79, -177, -188, 4, -41, -98, -28, -95, -42, -52, 36, -47, 11, -135, -43, 40, -26, -49, -26, -36, -39, -97, 1, -64, 3, -76, 5, -21, 3, -74, -35, 6, -170, -31, 17, -20, 3, 28, 148, 73, 54, -2, 64, 22, 75, -29, -8, 204, 45, 15, -26, 42, -24, 124, 13, 44, -33, 16, 68, 17, 97, -118, -49, 11, -35, 35, 76, 55, 127, 20, 121, 55, 43, 176, 41, 75, -27, -11, -59, -76, 57, -21, 70, 118, 68, 82, 9, 52, 53, 17, -178, 67, 59, -75, -179, -53, 74, -45, -23, 62, 9, -10, -6, -5, 32, -3, 13, 94, -51, -7, 36, -67, -5, 50, 18, 51, 79, 99, 20, 81, -60, 73, 45, 2, 42, -64, -2, -56, 22, 8, 56, 74, 35, 78, 48, 43, 133, 78, 7, 69, 76, 25, -58, 94, 44, 55, 79, -102, 82, 106, -79, -31, 51, 60, 137, 58, 94, 1, 36, -51, -36, -93, -12, -57, -35, 84, 87, -1, 95, 77, -45, 144, -20, 63, -21, 64, -18, 5, -6, -20, -11, 62, -43, -19, -69, 14, 93, -53, -55, 92, 62, -38, -31, 32, 75, -70, 40, 27, -125, -10, 14, 145, -75, 99, -15, 157, -36, -86, 39, 51, 22, 100, 19, 79, 2, -41, -12, 76, -30, 97, -20, 58, -71, 31, -10, 53, 91, -64, -23, -32, -65, -113, 60, -36, 27, -26, -61, 97, 75, 58, 66, 29, 26, 68, -57, 28, -25, -7, 120, -6, -16, -10, -87, 83, 81, 122, 108, -11, 55, -16, 30, -59, 112, -43, -24, 50, -12, 10, -99, 7, -18, 20, -68, -48, -117, -29, 12, -72, 29, -156, -51, 29, 68, -56, -101, 2, 164, 28, -69, 16, -10, 13, -10, 4, -62, 19, -24, -25, 21, -31, 2, -78, 127, -1, -5, 40, -21, 38, 22, -35, 62, -10, -17, 21, -146, -17, 6, -36, -40, -90, -87, 89, -9, -81, 61, -45, 13, 54, -40, -34, 57, 37, 2, -18, -30, 15, 18, -58, -53, -45, 24, -45, -5, 19, 35, -53, 91, -61, -14, -6, 11, 104, -10, -86, 9, 50, -67, -77, 0, 125, -83, -32, 63, -9, 191, -52, 75, 155, 48, 68, 102, 2, 69, 73, 96, -4, 88, 72, 11, -5, 7, 108, -3, 13, 11, -30, -50, -2, -32, -20, 26, -14, -3, 15, -69, 0, -32, -145, 2, -19, 3, 53, -139, 32, -145, -43, 0, -178, -55, -77, 65, -110, -67, -76, -26, 113, 39, -33, 172, 37, -63, 77, 149, 145, -17, -5, -78, 115, 36, 109, 167, -74, -6, -90, -48, 15, 104, 75, -94, -9, -53, 29, -66, 23, -20, 83, -49, 12, -1, -92, -7, -20, -96, -26, -128, -9, -16, 6, -71, -48, -44, -23, 103, 21, -117, 82, 61, -2, 73, -21, 91, 79, -18, 39, -122, 89, 20, 107, 130, 5, 24, 39, 29, -90, 83, -5, 5, 12, 1, 3, 29, -13, -6, 141}
, {-99, 33, 20, -27, -120, -119, -66, 3, 23, 1, 22, 5, -71, 2, 21, -78, -60, 119, 81, 54, -119, 35, 2, -46, -75, -20, 13, -15, -128, 166, 92, 219, 71, 125, 48, -15, 9, 11, -78, -18, 76, 96, 126, -237, 58, 90, -73, 10, 24, 32, 16, -21, 49, -6, -1, -119, -33, 4, 91, -40, 27, 32, 36, 69, 6, -2, -2, -197, -16, -71, -30, -51, -19, -25, 15, -8, -9, 3, -5, -238, -5, -42, -23, 79, -75, -15, 10, -47, -65, -61, -37, -51, -126, -153, -27, 65, -11, 47, 12, 41, -1, 43, 29, -47, 21, 92, -29, 10, -18, 96, 53, -1, 9, -34, -62, -11, -62, 9, 109, 64, -75, -1, 48, -100, -126, 69, -75, -97, 46, 162, 20, 74, 1, 61, -36, 8, -241, 62, 66, -118, -51, 4, 121, -84, 43, 36, -36, 6, 27, 12, -29, -9, -134, 38, 6, -87, -52, -63, 15, 100, -16, -12, 27, -82, 95, -88, -59, -25, 44, 43, 21, -6, -44, -30, -13, -49, -5, -58, -73, -109, 123, -210, 23, -64, 61, -9, 10, -44, 55, 37, -113, 137, -24, -143, -34, -54, -65, 66, 54, -59, -114, 24, -122, -28, -18, -103, -76, -159, 1, -17, 36, 22, -49, 30, 44, -11, -36, -27, 34, 66, 0, -55, -13, 6, 6, 38, -5, 26, -133, -28, -21, -37, 52, -24, 80, 59, 27, -25, 73, 12, -55, -14, -98, -64, -20, -17, -109, -21, -51, -35, -38, -79, -36, -80, -34, 21, -6, -25, -83, 11, 7, -3, -71, 45, -5, -4, 118, -109, 24, -29, -80, 13, 112, 94, -22, -104, -59, -38, -85, 3, -36, 40, 11, -37, 34, -63, -21, 73, 81, 54, 103, -66, 128, -190, -14, 8, 68, 16, 23, 45, 5, 146, 91, 6, 36, 53, 0, 80, -95, 39, 45, 41, -9, 1, -28, -14, 18, 160, 96, 6, -21, -10, 51, 23, -17, 51, 22, 11, -101, 82, -94, 43, -1, 30, 78, 4, 30, -86, -16, -27, -32, 26, 182, -60, -51, -7, -45, -206, 205, -11, -128, -1, 55, -99, 134, -44, -94, -52, -48, -78, -125, -39, -2, -123, 36, -91, -55, 12, 83, 70, 26, 33, -5, -6, -299, 60, -40, 18, 115, -60, 83, -13, -2, 93, 95, 131, 66, -94, 104, -150, -73, 60, 26, 33, 49, 28, 14, -44, 114, 140, 30, 65, 21, 18, 37, -161, 12, 9, -21, -19, 146, -61, 15, 2, 26, 111, -91, 35, 31, 86, -22, -41, 55, 53, -42, -70, -101, 10, 46, -82, -91, 32, -48, 59, 38, -30, -7, -15, -31, 48, -24, 6, -30, 82, 8, 69, -32, -7, -10, -26, -25, 87, 90, 2, 77, 30, -23, 108, 39, -73, 0, 49, 70, -34, -57, -86, -2, -10, -76, 32, 4, -83, 122, 6, 66, -94, 63, 65, -8, -77, 4, 14, -48, -11, 49, 0, -23, -27, 47, -50, 6, -46, -18, -26, 3, 32, 106, 46, -73, 83, 16, 1, 21, -75, 26, 29, -27, 2, -4, -22, 74, -6, -77, -121, -53, 61, 47, -110, 78, 5, 49, 69, -57, 5, -50, 39, -38, -48, -64, -59, 12, 33, 50, 47, 18, 14, 61, 19, -8, 19, 29, 62, -37, -131, 46, -73, -11, 40, -56, 11, -25, 29, -65, 3, 75, -54, -34, 13, 84, -21, -42, 1, 56, 7, -2, -2, -2, -63, -17, -45, 3, -90, 103, -21, -65, -37, 68, 5, 15, -41, -21, 43, 95, 6, -60, 16, 24, -50, 56, 43, -56, 21, 25, 88, 61, 29, -53, 46, -63, -17, -3, 16, -44, 9, 10, -31, -16, 29, 115, 61, 86, 12, 19, 10, 21, -3, 35, 13, -37, 22, -38, -53, 52, -50, 101, 0, 61, 126, -74, 16, 17, -53, 4, 103, -1, 11, 68, 3, -22, -70, -25, 3, 102, -28, -18, -67, -16, -23, 76, 64, 55, 12, -45, 40, -60, -93, -121, 6, -57, 19, -79, 42, 26, -12, -77, 12, -26, -57, -8, 1, 10, -102, -1, -17, -3, 74, 1, 33, -14, 31, -42, 22, 9, -106, -49, 77, 100, 67, 8, 106, -22, 129, 7, -74, 110, 111, 30, -43, 21, 44, 124, 41, 25, 34, -30, -3, -71, -13, 10, -28, -64, 100, 143, -30, -20, 105, 48, 111, 9, -6, -49, 15, -26, 142, 64, 59, 6, 58, 11, 86, 124, -26, 124, 74, 33, -10, 16, -52, -54, 30, 67, 71, 108, 54, -25, 64, 41, 32, 23, 63, 46, 76, 0, -36, -81, 81, -74, 52, 26, 34, -148, -4, 54, 44, 86, -42, -6, 16, -32, -26, -15, 61, 0, 127, -28, 18, 22, 28, 40, -6, -75, 33, 13, -22}
, {-39, 34, -80, -41, -39, -92, 15, 66, 107, -78, 23, 14, 78, 7, -16, 6, -76, -21, -89, -71, 72, -70, -45, 90, -93, 52, -16, -164, -88, -79, -53, 13, 25, 61, 22, 84, 84, -1, -17, 42, 49, 40, -50, -25, 5, -35, 34, 1, 7, 4, 33, 41, -23, -35, -39, -39, 32, -14, -1, -45, 2, 17, -8, 54, -11, 26, -27, 59, -157, -79, 54, 71, -26, -19, 128, 30, -29, -140, -3, -8, -20, 18, 1, 9, -167, -59, -56, -5, -54, 28, -64, 13, -71, 13, 88, 106, -51, 25, -11, 0, 0, 18, 19, 51, -35, 95, -8, 27, 28, -61, 38, 19, -5, -38, 10, 31, 14, -6, 10, -97, 26, -17, -38, 129, -151, -53, -53, -24, 10, -75, 27, 42, -44, 23, 26, -81, -58, -65, -112, 38, -17, 15, 1, -74, 17, -32, -39, 16, 26, 52, 129, 4, 19, -14, -23, 111, -55, 64, 46, -13, 60, -4, 30, -86, 128, 111, -64, 36, -20, 29, 29, -33, -49, 30, -7, 46, 25, 59, 5, -145, 99, -53, -69, 44, -2, -63, 91, 8, 70, -97, -70, -13, 57, 70, 24, 0, -89, 73, -90, -23, -68, 92, -1, -40, 0, -45, -43, 64, 127, 2, 53, -4, 5, 18, 118, 8, -40, 47, 71, -5, 11, -10, 62, -85, -18, -64, -22, 75, 48, 35, 42, -66, 10, 116, 103, 9, -177, 22, 35, -55, -54, -23, 5, -28, -9, 24, 34, -75, 91, -30, -4, -110, 81, 47, -20, -175, 54, 1, -121, -114, 49, -49, -112, -18, 24, -47, 54, -28, 174, -49, -125, -133, -137, -27, -57, -7, 19, -34, 39, 38, 1, -11, 23, 62, 3, 19, -48, 62, -128, -67, 16, 155, 8, -12, -52, -2, 90, 57, 58, 15, -1, 19, -110, 160, -103, 132, 73, -47, 61, 62, -41, -58, -22, -28, 43, -26, 70, 46, -81, 72, 120, -3, 18, 102, 84, 3, 29, 60, -43, 96, 63, 85, 29, 9, 60, 147, 86, 36, 8, 42, 24, 60, 94, 75, -66, 128, 43, -148, -49, 5, 95, -50, -28, 7, 35, -13, 50, 27, -25, 101, -54, 57, 59, -141, 86, 78, 69, 32, 27, -16, 93, -124, -68, -27, -155, 73, 40, -16, 103, 20, 82, -50, -86, 52, 7, 38, 17, 63, -43, -53, 135, -71, -9, 22, -32, 23, 70, -12, 25, -33, 8, -39, -95, 28, 93, 24, 29, -4, 10, 34, 34, 24, -73, 18, 41, -21, -44, 11, -66, -12, 78, -48, -2, 55, -17, 79, 159, 15, -12, 33, -104, 33, -40, 9, 2, 40, 48, 74, -40, 35, 123, 94, 120, -28, 36, 38, 144, -36, -2, -3, -17, 76, 53, 29, 81, 18, -71, 39, -39, -35, -45, -8, 51, 77, -56, 0, 35, 13, 5, 52, -60, -1, 57, 44, 2, 135, -23, -29, 64, 23, 26, 50, 47, 19, 7, 129, -41, 117, 85, 2, 86, 123, 15, 46, 35, 45, 77, -30, 10, -33, -5, -24, -115, -37, 72, -36, 49, -57, 65, -20, 18, 38, -5, 19, -51, 85, 26, -21, -20, -42, 71, 6, -9, -24, 16, 17, 35, -40, 53, 81, 70, -55, -4, -39, 1, -15, 13, 42, 24, -3, 45, 0, -30, -56, -24, 23, 99, -33, -39, -4, -22, -10, 34, 34, -3, 29, 46, 9, 7, -25, -57, -70, -87, 35, 45, -25, 78, 30, 204, -28, -61, 185, 69, 123, 31, 70, -106, -83, -100, 33, 10, 1, 50, -45, -28, 32, -21, -51, -110, -6, -10, -37, 36, 101, 58, 2, -58, -36, -31, -6, 88, -55, -9, 39, 43, -59, -21, -6, -7, -53, -20, -54, 20, -118, -95, -4, -25, 27, 28, -48, -14, -14, -117, 59, 77, 151, 134, -8, -4, -105, -53, -17, 18, 13, 96, -27, 45, 43, 7, 47, 13, 48, 67, 31, 92, 62, 22, 31, 105, 88, 29, 44, 53, 48, 166, 91, -15, 92, 34, 7, 49, -4, 135, 97, -52, 37, 49, 61, 47, 29, -9, 29, 36, 30, -11, 110, 35, 6, 3, 103, -9, 51, 64, 32, 91, 92, 38, -4, -108, 39, 33, 43, 14, -66, 65, 53, 46, 82, 50, 56, -7, 117, -108, -12, -107, 52, -65, 25, 10, 29, -37, 31, 20, 32, 38, -44, -61, 54, 46, 11, -96, -25, 19, 11, -67, 10, 36, 53, 4, -100, 78, 4, 4, -13, -179, -37, -92, -36, 14, -99, 64, 29, -119, -47, -59, 39, 13, -13, 53, 57, -41, 14, 75, 7, -84, 147, -11, 1, 21, -2, 54, 142, 42, 9, 13, 166, -7, 76, 201, -28, -13, 82, 33, 55, 116, 17, 16, 6, 72, 6, -37, -4, -17, 166}
, {67, -46, 2, -11, -26, -2, 26, -35, -93, -30, -112, 15, -15, -2, -3, 105, 49, -140, -44, 85, 154, 69, 17, -69, -69, -58, -50, 118, 34, -72, -55, -27, 8, 26, -51, -40, -32, -127, 90, 109, -73, -54, -104, 145, -7, 28, 132, 104, -20, -21, 17, 55, 129, 46, -52, 39, 10, 3, -96, 26, -94, 117, -14, -72, -38, 41, 68, 124, 118, 103, 55, 187, 69, 67, 130, 140, 10, 93, 75, 148, 43, 58, -63, -47, 131, 140, 53, 125, 15, 59, 38, 106, 3, 50, 4, -10, 3, 111, 18, -3, 15, 16, 30, 82, 167, 118, 31, 40, 103, 81, 55, -4, 24, 64, 35, 29, 12, 11, -28, -107, 207, 41, 14, 40, 140, -53, 58, -12, -43, -89, 32, 37, 93, 22, 14, -146, 220, 13, -56, -15, -13, 99, -19, -222, -48, -15, 32, 28, -133, 99, 221, -14, 102, -85, -41, 53, 78, 23, -49, -78, 19, 37, -53, 65, -128, 163, -31, 36, -130, -127, 16, -44, -3, 2, -3, 5, 82, 24, 25, 55, 7, 172, -81, 81, -74, -10, 9, 5, -65, 44, 51, 108, 190, 43, 30, 50, 151, 16, -52, 20, -50, -16, 102, 13, 31, 3, 31, 188, 139, -24, -18, -37, 57, 13, 13, -42, 135, 52, -97, 84, -13, 64, 69, 36, -47, 3, 52, -29, 79, 12, 48, -15, 39, 50, -42, 92, 72, -13, -33, -14, -90, -69, 34, 48, 46, 34, -12, -34, 97, 148, 18, -6, 33, 68, -12, -90, 28, -2, 181, 75, 8, 39, 171, 70, 93, 34, 27, 52, 46, 43, 15, -64, -5, 42, 46, 53, -5, -12, 42, 4, -49, -95, 122, 29, 87, -71, -34, 49, 2, 118, 67, -10, -11, 0, -84, 56, -50, -56, 107, -44, 2, -88, -87, 101, 42, 38, 42, -29, 1, -161, -42, 28, 43, -60, 80, -18, 35, 34, 6, 47, 93, 85, 33, 99, 85, 5, 13, 67, 22, -82, 108, 64, 26, -43, 68, 209, 41, 136, 26, 46, 49, 16, -109, 37, 67, 57, 83, 139, 10, 54, 52, 13, -25, 38, 17, 31, 103, 0, -41, -103, 40, 70, -39, 89, 1, 40, 69, -2, -28, -30, 12, 44, 29, 56, 110, -13, 127, 18, -134, 104, -4, 20, 86, 5, 22, -69, -21, 80, 8, 17, -37, 18, -17, -62, -29, -82, 42, -89, 4, 10, -100, -35, -14, -13, 62, 58, 23, -63, -57, -54, 131, -62, 53, 8, -31, 45, -36, 1, 17, 43, 11, -99, 23, -57, 62, 5, -78, 22, 46, 36, 3, 9, -7, -130, -29, 22, 13, -57, -26, -91, -102, -88, 3, -38, -89, -44, -101, 89, -84, 93, -3, -54, -18, -47, 2, 44, -26, -26, 36, 96, 0, 4, 1, 86, 31, 25, 20, 77, 6, -34, 41, -33, 32, -2, 4, 99, -46, -14, -27, -62, 24, -17, -52, -39, 44, 0, 74, -149, 55, 13, -99, 83, -66, 63, -16, -184, -14, -45, -73, 74, -75, -10, 35, -14, -22, 21, -2, 32, 40, -48, -44, -70, -12, -45, -47, 27, -15, -56, 59, -66, 10, 66, -85, -23, -33, 20, 10, -73, 2, -60, -82, 100, -37, 71, 20, -55, 45, 68, -193, 35, -76, 68, 6, -54, -146, -19, -28, -51, -21, 14, 2, -54, 10, 8, -71, 31, -33, 12, -104, -114, -60, 8, 32, -60, -39, -81, 27, -31, -12, -60, -23, -11, 14, 6, -71, -10, -89, 2, -11, -26, 1, 10, 22, 8, 19, 29, -21, -5, 45, -38, 50, -6, 54, -2, 70, -75, 22, -43, -136, 49, -39, 14, 69, -95, 4, 26, -5, 48, 122, 98, 33, 24, -39, 82, 54, 32, -44, -108, 12, 18, -87, -11, 27, -2, 64, 72, 20, 25, 35, 10, -60, -42, 15, -59, -69, -51, -211, 9, 76, -18, 55, -78, -96, 121, -22, 0, 43, -15, -77, -79, -86, 16, -32, -35, 69, 39, 38, -176, -32, -35, -10, -2, -101, -86, -34, -6, -141, -64, -136, -1, -8, -20, -13, -25, 7, -52, -20, -27, -33, -31, 7, 18, 22, 66, -44, -59, -92, -59, -10, 21, 20, -4, -10, 59, -102, -13, -81, 41, 42, 4, -23, 12, 5, -20, -47, 61, 72, -125, 66, 48, -43, 3, -8, 61, -10, 63, -138, 84, -36, 16, 30, 111, 50, -1, -25, 13, 29, 7, -18, 13, 43, 24, 54, -9, -58, 10, 29, 96, -30, 106, 46, -20, 30, 22, 144, 18, -7, 17, 133, 78, -4, -16, -15, 13, 88, -17, 119, -37, 26, 38, 124, 5, 14, 48, -90, 55, 1, 42, 64, 2, 21, 4, 9, -96, 25, 10, 5, -12, 21, -23, 32, 16, 12, 47}
, {-111, -23, -37, -134, -128, -178, -169, 4, -112, -83, -74, -13, 53, 1, -2, -127, -76, -148, -90, 8, 110, -48, 56, -67, -119, -184, -157, 26, -113, -79, -23, -51, -16, -159, -19, 48, 49, 34, 79, -281, -61, -71, -144, 16, -69, -5, -18, -107, -16, 4, 1, 86, 20, 40, 12, -78, 88, 2, -211, 121, -15, 14, 81, -47, 11, 28, 14, -85, 99, 73, 32, 207, 56, 72, 9, 96, -24, 126, 53, 58, 90, 45, -81, -59, 112, 101, -16, 83, 40, 136, -45, 60, 4, 127, -54, -110, 228, 45, 35, -22, 8, -3, 64, 43, 146, 105, 15, 32, 26, 52, 211, -48, 5, -13, 31, -71, -88, -40, -113, -39, 90, -26, 58, 58, -48, -68, -71, 37, -40, -58, -37, 14, -9, -6, 0, -111, -36, -30, 11, 64, -64, -51, -72, 1, -107, 19, 22, 11, 55, 10, 31, 77, 87, 10, 27, -10, 73, 43, 55, -120, -7, 58, -7, 93, -8, 134, -79, 127, -56, -22, 3, 46, 85, -72, 40, 7, 27, 33, 57, -93, 75, 66, -79, 78, -78, 30, 76, -28, 22, -38, 65, 64, 9, 48, 75, 13, 98, -145, -9, 80, -72, -112, 111, -28, 23, -17, -55, 64, -6, -60, -24, 26, 130, -35, 29, 59, 159, 40, 134, 109, -17, 45, 145, -15, -29, -38, 21, -13, -37, -18, -16, 19, 22, 7, 61, 63, -10, -96, -45, -54, -16, -84, 48, 34, -16, 44, 95, 1, 42, 33, 32, -38, -77, -21, 0, -148, -56, -15, 45, 63, 21, 96, 130, 15, 74, 36, -92, 40, 37, 42, 18, 46, -52, -131, -51, -17, 41, 29, 59, -75, -3, -203, -16, 25, 29, -35, -170, 100, -14, -104, -114, -260, -10, -242, -33, -100, -100, -227, 26, -78, 5, -100, -336, 105, 88, -122, -74, -103, 77, -157, -121, -84, -273, -206, 18, -128, -38, -139, -171, 87, 88, -23, 76, 85, 86, -54, 13, 59, 52, -47, 136, 173, -23, 26, 13, 77, 8, 105, -47, -3, -26, 22, 2, -8, -96, -59, 21, 91, 0, -94, -16, -14, -1, -40, 9, -29, 43, 11, -10, -77, 68, 71, -26, 46, 109, 76, 47, -102, -36, -64, 32, 59, 20, 121, 0, 91, -21, 6, 32, -74, 56, 40, -41, 20, 45, 8, -37, -5, 1, 84, -15, 9, 10, -6, -18, -27, 56, -18, -64, 79, -53, -31, 16, -112, -108, 23, 24, -100, 16, -57, 10, -4, 53, -24, -60, -152, -47, -15, 60, -153, -204, -181, -11, -133, 3, -245, -110, -43, -28, 1, -32, -224, -88, -35, -21, -25, 11, -13, -12, -20, -52, -22, 35, -14, -33, -41, -179, 3, -26, -34, -10, 35, 37, 13, -17, -29, -32, 15, 8, 20, -26, -53, 28, 43, -83, 75, 73, -76, 17, -44, -160, 70, -34, -5, 72, 159, 6, 8, 42, 90, -26, 29, 20, -40, -20, -32, -2, 6, -1, 45, -21, -24, -31, 17, -52, -62, 29, -38, 71, -15, -29, 12, -18, 36, -12, -9, 63, 35, 79, 40, -51, 14, -2, -40, -37, -21, -49, -86, 69, -5, 0, 33, -16, 4, 17, 2, 48, -21, -27, -92, -50, 7, 46, 20, 8, 5, -21, -9, 8, 3, -27, -70, 7, 52, -33, -7, -26, 21, -7, -35, 38, -46, -6, -23, 36, -60, 22, 5, -40, -51, -23, -137, -85, -77, 33, -89, -31, -37, -20, 13, 12, -110, 88, 22, 45, 10, 10, 7, -1, -11, -55, -8, -93, -60, -114, -47, -31, 23, -18, -102, -11, 9, 87, 1, 2, -93, 16, -51, -32, -24, 8, 29, 23, -76, -31, 35, 10, 56, -87, 29, 61, 52, 18, -7, -24, 37, 11, -25, 45, 105, -26, 61, 18, 12, -3, 44, 6, 45, 37, 36, 75, -7, 20, 22, -55, 35, 36, 31, 47, 5, 2, 41, -48, 17, -18, 124, 65, -71, -117, 71, -143, -42, 57, 3, 179, -2, -74, 62, 101, -51, -1, -37, 1, 1, 76, 11, 40, 83, -44, 0, 7, -75, -19, -21, -21, -21, -103, -4, -9, -50, 13, -9, -43, 77, 13, 62, -207, -149, -170, 47, -28, 6, 3, -37, -58, -46, -21, 129, -30, 25, -93, 107, -104, -19, -101, 33, 24, -89, -8, 95, -5, -82, 98, 30, -100, -30, -6, -1, -81, -6, 59, -42, 17, -200, 4, 17, -18, 46, 12, -48, -133, 24, -36, 11, 12, 79, 53, 27, 64, -39, 36, -67, -40, 53, 24, -45, -79, 54, -59, 6, -18, 38, 13, 2, -51, 10, 33, -31, 31, 2, 71, 75, -29, 34, -64, 74, 17, 4, 88, -27, 22, -16, 30, -88, 29, -71, -8, -15, -51, 3, -36, -1, -13, 59}
, {38, 42, -21, -16, -15, 53, 12, 26, 23, 25, 6, 12, 154, 11, -7, 27, -70, -156, -129, -109, -11, -73, 1, 15, -88, -192, -226, -58, -34, -196, -99, -64, -49, -56, -63, -50, 60, -27, -1, -168, -26, 74, -33, -54, -17, -91, -79, -44, 5, -12, 61, -41, -19, 68, 8, 95, 10, -11, -106, -6, -31, -168, -51, -157, -10, 62, 94, 6, 37, 27, 32, 64, 12, -92, -27, -20, 56, -61, 35, 154, -8, 82, 5, -98, -31, -15, 19, -102, 17, 78, 7, 13, 42, 9, 59, -75, -132, -137, -12, -38, -9, -24, -105, -92, -77, -23, 39, -47, -43, -54, -104, -20, 3, 109, 28, -67, -21, -30, -78, 9, 4, -40, 60, 10, 112, -13, 50, 94, 24, 20, 11, -40, -49, 21, -31, 7, 70, 18, 59, 79, 46, -33, -21, -33, -47, -13, 14, -41, 29, -52, 24, -28, 182, -100, 20, -88, 53, 9, -53, -24, -7, -9, 19, 68, -100, 16, 135, -24, 1, -44, -19, 19, -30, 42, -11, -183, 17, 58, 54, 77, -18, 63, 74, 45, 17, 39, -128, 56, 82, 4, 84, -76, 74, 59, -28, -34, 126, -13, 27, 13, -28, 10, 61, 39, 0, -7, 52, 55, -9, 77, -6, -78, -37, -15, -98, -67, 75, 57, -134, -46, -10, 71, 76, 43, 32, 76, 65, -13, 46, 20, 8, 76, 11, 1, -25, -54, 14, -2, -69, -20, -25, -84, 59, -42, 10, -63, -2, -5, 17, -3, -89, 58, 57, 12, -10, 95, 23, 8, 97, 30, -39, 41, 87, 19, 0, 59, -35, 91, -40, -15, 48, 29, -48, -98, -60, 63, 81, -40, 152, -130, 56, 35, -115, 15, -84, -1, -14, 27, -51, -92, 78, 115, -6, 198, 36, 27, 70, 37, -2, 52, -18, -3, 18, -22, -11, 1, -1, -65, -16, 65, 36, 43, -83, -25, -11, 5, 49, -111, -17, -56, 23, -142, 0, 9, 12, -6, -71, -6, 96, -92, 139, -7, -131, -33, -82, 28, 41, 119, 34, -12, 22, -78, -81, 92, 57, -34, 46, 174, -176, 15, 107, 124, 81, 100, -86, -6, -8, 51, 28, 118, 40, 69, 88, 15, -82, 101, 51, -6, 78, 33, -31, 27, -57, 30, 52, 56, 87, -58, 16, 103, 8, -25, 47, -21, -27, -43, -24, 73, -106, -44, 105, 15, -110, -17, -10, -25, -34, 73, -54, -103, -12, 60, 37, -4, -40, 336, -10, 78, 69, 117, -88, 98, -7, 95, 55, -98, 24, 35, 8, -28, 36, -11, 40, 23, 5, -23, 55, -52, 19, 4, 65, 5, -58, -96, -6, -78, -32, 99, -43, -69, 64, 65, -36, -77, -45, -70, -10, -127, 8, -52, -5, -92, -66, -22, -13, -53, -14, -77, 55, 119, 48, -44, -112, -53, -44, 56, -4, 44, -55, -4, 39, -29, -33, -46, -79, 68, -183, 37, 14, 33, -46, -167, -60, 60, -5, 22, 107, -73, 114, 52, -128, 73, -14, 34, 42, 47, -2, -107, -140, 18, 28, 39, -4, -9, -36, 26, -104, 11, -136, 63, 46, -24, 31, -164, -65, 27, -17, 206, 14, 78, -50, -8, -10, 63, -132, -4, 104, 86, 84, -58, -157, 48, 37, 115, -34, -70, -63, -45, -133, -89, -101, -29, 20, 41, 26, 28, 20, -37, 36, 27, -35, -24, 54, -8, -82, 18, -83, 15, 54, 10, 100, 6, -7, -30, -62, 21, -27, -32, -120, -15, -14, -23, 16, -71, 21, -20, -22, 40, -34, 48, 2, 42, -12, -2, 145, -14, -62, 64, -187, 4, 92, -53, -55, -67, -27, -8, 37, 29, 75, -32, 45, 6, 44, 42, -18, 41, 25, -104, -47, -82, 34, 10, 7, 51, 37, -59, 40, 93, -53, -33, 36, 5, -17, -100, -121, -28, -84, 77, -25, -64, 39, -26, -52, 86, -20, -72, 35, 13, -153, -2, -96, -43, -31, -11, -99, 77, 8, -56, -110, 42, -50, -100, 52, -40, -39, 28, 9, 2, 79, 7, 54, 1, 1, 70, 70, -8, 109, 77, 54, -7, 5, 92, -27, -2, 6, -90, -2, 22, 15, -41, 33, 21, -53, 78, -31, -59, -57, 13, 122, -85, 63, -45, 66, 103, -117, -29, 84, 80, -71, 72, -23, 14, 67, -6, 91, 50, 97, 68, -6, 101, 126, -55, -78, 34, -17, -14, 58, -71, -24, -52, 56, 135, 15, -48, 16, 133, -37, -64, 80, 0, -9, 12, -4, 26, 20, -60, 36, 32, 110, 20, -24, 1, -29, -22, 41, 29, -37, -49, 77, -26, 57, -55, -56, 33, 103, -107, 7, -18, 2, 2, -9, -148, -33, 5, -32, -165, -12, -73, -105, 35, 9, -72, -20, -82, -68, -28, -14, -33, -47, -12, 124, 9, -20, -94}
, {5, -46, -25, -13, 18, -12, 71, 57, 17, -75, -71, -9, 41, 18, -1, 18, 15, -63, 53, 17, 69, 2, 8, 58, 6, 85, -24, 86, 22, 3, 25, -30, 5, 17, 15, 56, 72, -25, 49, 58, -25, 0, -81, 37, 38, 77, 54, -4, -44, 3, 58, 15, 159, 15, 87, 58, -22, -4, -16, -22, -47, 54, 12, -29, -19, 50, -10, 67, 117, -8, -47, 74, 10, 51, 101, -1, -4, -47, 25, -24, -4, 22, 88, 47, 170, 92, 30, 124, 63, 22, 126, 13, 59, 38, 0, 86, -42, 49, 49, -21, 2, -4, 51, 38, 40, -24, 51, 21, 48, 105, -40, 4, -61, -17, -115, -43, -34, -119, 56, -7, 172, -112, -119, 110, 114, -79, -31, 29, 20, -62, -65, -4, 54, 58, -17, -74, 58, 14, -30, 95, 20, 55, -9, -90, 1, -52, -41, 22, -19, 59, 182, 3, 149, -16, -27, -82, 63, 104, 2, -17, 27, -50, -70, 130, -1, 77, 188, -50, -19, -47, -51, 11, -81, 18, -3, -77, 17, -10, -23, 71, 94, 289, -22, -34, -38, 106, -45, -55, -41, 4, 77, 56, 102, 47, -21, -46, 94, 260, 11, 72, -15, 158, 50, 48, 10, 21, 15, 97, -36, 119, 106, 4, -35, 14, 70, 89, -80, -81, 74, 27, 66, -70, -81, 79, 21, -61, -60, -7, -14, -19, 111, 24, -27, -43, -24, -38, -102, -27, -107, -5, -25, -33, 23, 12, -10, -5, 36, -54, 33, 16, -63, 54, -65, 55, -23, -145, 59, 15, 61, 50, -57, -17, 13, -16, 100, -35, -141, -12, 57, 40, -15, -95, -35, -47, -10, 46, 24, 47, 17, 72, 32, 30, -64, 46, 26, 1, 7, -39, -111, -30, -23, 94, 2, 7, 2, -31, -30, 73, -15, -95, -25, -39, -57, 39, 0, 143, 44, 75, -6, 102, 31, 27, 25, 63, -30, -13, 91, -26, 54, 26, 51, 36, -18, 8, -79, 9, -51, -81, -126, 80, -180, -87, 25, -34, 39, 18, 56, 89, 69, 92, 6, -1, 8, 9, -27, 58, 28, -54, -115, -72, 31, -42, 79, -12, -37, 89, -3, 75, 2, -64, -23, 90, -182, 4, -17, 99, 119, -119, 39, 13, 45, -10, -45, 52, -47, 68, 205, 42, -126, 62, -42, 56, 129, -19, 26, 21, -2, 70, -34, -119, -23, -123, 40, 17, -41, -16, 45, -51, 41, -9, 95, 54, 13, 6, -38, -40, -48, -29, -69, -73, 43, -46, 23, -23, 20, 81, 125, -2, 0, -52, 105, -63, -16, 62, -21, 114, 189, 57, 13, -38, -55, 38, 3, 41, 62, -71, 8, 41, 0, -55, 41, 44, 2, 23, 33, 45, 80, -30, 53, 75, 64, 23, 2, 50, 84, 46, 19, 32, -58, 66, -6, -14, 11, 23, -22, -89, 54, 76, 100, 60, 70, -55, 164, 85, -81, -10, -26, 53, 57, -75, 6, -18, 23, -15, 90, 59, 137, 54, 125, -68, 9, 28, 31, 51, 33, -13, 35, 32, -30, -39, -20, -46, -42, 111, 39, 13, 79, 61, 73, 11, 15, 66, 17, 15, -20, 4, 96, -59, -49, 36, -52, 7, 121, 25, 28, -56, 0, 145, 90, 45, 90, -25, 10, -122, 5, 10, -21, -109, 44, 88, -36, -34, -31, 83, -45, -138, 63, 63, 40, 28, -57, 9, 38, 42, 131, 13, 25, 4, -67, 0, -111, -13, -88, 0, 96, -117, 144, 74, 76, 13, -81, 156, -33, 135, 38, 60, -198, -100, -85, 4, 54, -31, -77, 64, 15, 39, -2, -19, 135, 24, 11, 12, -160, 26, 66, -153, 18, -65, -155, 36, -53, -11, 29, -2, 15, -14, -105, 11, 128, 82, -6, -68, -74, -15, 60, -19, -132, -207, 38, -21, 22, -96, -90, 64, 72, 41, -21, -64, -10, -157, -96, -27, -78, -61, -54, -86, 22, -21, -40, -18, 69, -67, 29, 92, 4, -41, 24, 7, -127, -192, -39, -18, 4, -144, -70, 74, -70, -209, -92, -117, -51, 9, -174, -121, -87, -4, -150, -103, -125, 2, -15, -95, -18, -23, -17, -20, -24, -53, -31, -13, -37, -72, 34, -45, 74, -33, -40, -36, -189, -37, -16, -74, -26, -101, -115, -44, -91, 21, 41, -16, -164, 5, -31, 15, -62, 44, -22, -22, -11, -74, 39, -5, 33, 70, 14, 8, -88, 62, -162, -7, -103, 64, -23, 35, -35, -50, -65, -38, 4, 73, 76, -167, -112, 99, -89, -7, -3, 27, -19, 51, -170, -56, 79, -3, -159, -175, 54, -133, 91, -138, -107, -43, -44, 62, -10, -32, -95, -45, -62, -15, 22, 45, 20, -133, -86, 55, -5, -86, -85, 41, 25, -15, -4, -59, -26, 43, -8, 16, 41, -14, -35, 21, -16, -67}
, {12, -60, 98, 26, -75, -44, -23, -4, 62, 76, 112, -2, 37, 6, 17, -47, 37, 163, 161, 3, -131, 45, 51, 91, 190, 92, 135, 137, 3, 209, 174, -80, -40, -23, -6, 19, 68, 61, -4, 36, 4, 23, -71, 70, -40, 185, -23, -21, -7, -10, 21, 17, 99, 61, 21, 10, 21, 74, -21, 44, 28, -15, 25, 0, 7, 29, 17, -26, 104, 46, -77, -76, -11, 28, -35, -69, 37, -44, -9, 7, -4, 36, 70, 38, 204, 70, 84, 74, 84, 13, -41, -14, 30, 61, 27, -8, -21, -11, 56, 14, -6, 2, -16, -3, 134, -19, 104, 85, 59, 106, 93, 33, -20, 68, 21, -44, -21, 15, 27, 80, 86, 22, 47, 25, 60, 105, -2, -24, 79, 3, -10, 14, -33, 39, 85, 48, -18, 47, -30, 93, -19, 11, -36, 13, 34, 0, -22, 28, -153, -49, 127, -61, -4, 64, -79, -66, -35, 29, 31, -63, -62, -116, 0, 62, -3, 10, 35, -11, 72, 40, -103, 38, 7, 98, 6, -11, -44, -122, -82, 28, 89, 42, 93, -66, -32, 162, -78, -40, 27, 3, -48, -27, -103, -81, -38, -60, 58, 48, 71, 17, -4, -16, -1, 99, 8, -12, -75, -99, -71, 25, 111, -39, 47, 42, -62, -3, 75, 28, 11, 33, -63, -10, -66, 113, 49, 80, 57, 46, -46, 12, 28, 30, 122, 16, 14, 44, -103, 3, 33, 82, 172, 117, 28, 21, 17, 42, -85, 84, -37, -11, -87, 98, -33, -69, 62, 74, 114, 21, -4, 108, -85, -31, 68, 88, -86, 20, -58, -18, -110, -30, 65, 56, 18, -34, -69, 80, 29, 29, 55, 23, -26, 59, -8, -55, -45, 89, 125, -38, 39, -84, -25, 19, 98, 26, 158, -63, 67, 80, 26, 32, -26, 102, 27, 3, -110, -27, -5, 9, -61, 165, 61, -76, 87, 96, -115, 39, 31, -56, 65, -86, -108, 92, -105, -95, -92, 65, -30, -76, 90, 94, -122, -76, -12, 6, -13, -88, 12, -39, 88, 56, 51, 22, -13, -99, -64, 143, 100, -130, -97, 13, -18, -26, -25, -49, 43, 77, -45, 55, 64, -135, 30, 11, -40, 95, -76, 9, 88, -207, 92, -15, 8, 7, -69, 94, 29, -2, 28, -7, -40, -5, -102, 27, 56, -89, -11, 68, -46, -15, 26, -38, -77, -35, -15, 69, 42, -54, -15, -79, 50, 10, 42, -46, 50, 80, -29, -59, 31, 39, 44, 14, -16, 21, 21, 25, 129, -11, -44, 38, 4, 8, 55, 65, 14, 59, -13, 66, -36, 89, 43, -11, 38, -13, -12, 35, 87, -28, 47, 26, -8, -7, -34, -40, -43, 35, 29, 76, 73, 33, 28, 89, 20, 15, 15, 14, 2, 44, 80, 14, -33, -5, -12, 104, 89, -11, -146, -31, 25, -59, 77, -35, 56, -4, 98, 76, 10, -103, -23, 4, -52, -82, -92, 20, 41, -102, 77, -8, 62, 104, 5, 40, 3, -121, 49, 84, -28, -9, -10, -31, -7, 1, 15, -35, -50, 17, -56, -48, 39, 3, 50, -28, 4, -82, -30, 4, 83, -1, 73, 8, -50, 20, 34, 1, -6, 53, 17, 21, -36, 82, 66, 58, 60, -97, -56, -147, 26, 53, -15, -100, -21, 65, 75, -29, 82, 31, 27, 8, 84, 25, 17, 54, -52, 66, 56, -22, 56, 16, 2, -13, 121, 17, -45, -2, -95, 70, 49, 2, 72, 16, 197, 80, -57, 184, -40, 68, 27, 6, 60, -72, 92, -43, 46, -80, 59, 41, -60, 21, 62, 25, 142, 31, -1, -47, 118, 79, 22, -125, 3, -135, -205, 16, -111, -10, -10, -84, 23, 10, -4, -3, 3, 40, 16, -57, -6, -77, -148, 22, -72, -146, 7, -64, -47, -106, -76, 43, 7, 4, -57, 10, 100, -69, 3, 36, -11, 66, 14, 40, 97, 27, -95, 42, -3, 48, 57, -14, 36, -193, 84, 74, -16, -109, 119, 80, 90, -85, -172, 64, 9, -60, -73, -17, -57, 18, -60, -61, 36, -15, -1, -57, -12, -5, -7, -16, 2, 68, -31, 31, 51, 4, -37, 54, -45, -28, -7, -63, -80, -54, 84, 70, -90, 2, -6, 16, 16, -90, 12, -6, 70, -60, -5, 63, 114, 30, -31, 80, -31, -20, -99, -71, 46, 4, -153, 68, 134, -4, 16, 66, -78, 133, -37, 24, -110, -22, -43, -12, 28, -6, -47, -69, -110, 71, 12, -177, -21, -33, 4, 19, 112, -34, -53, 110, -144, 44, 159, 26, -164, -4, 15, -37, 6, -126, -9, 78, 31, 10, 134, -1, -2, 29, 18, 10, -82, -67, -4, -22, 56, -87, 5, 16, -100, 4, 7, -50, -21, -64, -83, 26, 8, -20, 4, 15, 33, -3, 3, -62}
, {-53, -45, 42, 11, 9, -29, 20, -17, -44, 34, -35, -22, -40, 19, 5, -48, 16, 171, 139, -9, -99, 20, 25, -69, 79, 133, 167, -11, -29, 139, 123, 23, 6, 103, -39, -17, -2, -15, -33, -14, -205, -175, -17, 5, 18, 57, 8, -53, 32, -16, -42, -31, -42, -79, -36, -190, 22, -24, 54, -95, 27, 28, 37, 4, 55, 35, -7, -41, 26, -96, -66, -34, -63, 12, 205, -144, -46, -42, -55, 48, 31, -43, 20, 1, 18, -24, 82, -136, -54, -2, 62, -89, 71, -110, 6, -22, -6, -9, 31, 1, -14, 9, 92, -56, -1, 32, 59, 70, 21, 41, 96, 31, -15, 140, 41, -64, -104, -14, -51, 40, -21, -65, -28, -94, 62, 36, 48, 57, 75, 31, 70, 12, 24, 10, -14, 151, -112, 69, 27, -17, -21, -37, 17, 126, 30, 31, 91, -6, 24, -3, -113, 151, -84, -25, 89, 46, 150, -40, -4, 56, 102, -14, -47, -134, 94, -169, -56, 40, -28, 8, 26, -20, 73, -43, -49, 87, 36, 58, 72, -114, -149, -139, 11, -18, 50, -15, 5, 39, 5, -6, 2, 64, -186, 34, 59, -24, -144, 73, 77, -38, 0, -88, -83, 98, 44, -57, -16, -58, 86, -83, 27, 58, -15, 9, -21, -4, 90, 69, 94, 69, 15, 56, 63, 53, -142, 90, -129, -112, 32, -140, -3, 130, -149, -44, -36, -64, -51, -1, -11, 28, -76, -28, -52, -28, 50, -9, 21, 141, 33, 6, 142, 182, -132, 45, -15, 98, 55, 51, -76, 48, 32, 0, 51, 57, 47, 12, -67, 91, -254, 31, 62, 108, 44, -114, -56, 86, 41, 80, -2, 36, 41, 53, 10, -7, -159, 114, 10, 54, 62, -142, -49, 88, 46, -75, -5, 3, 48, 34, 9, 11, -60, 85, 34, -71, 16, -40, -54, 51, 138, 2, 13, 100, -50, -44, 43, 45, -80, -46, 94, 35, -71, 1, -170, -68, 8, -27, 30, 37, 69, 96, -69, 96, -13, 72, 66, 12, 34, 42, -111, -97, -155, -133, -45, 119, -63, -133, -174, 9, 77, -206, -122, 51, 155, 85, -78, 32, -54, -43, -86, -68, -5, -131, -20, -74, 48, -77, -89, 108, 181, -2, -13, 80, 1, -29, -46, -111, -17, -27, -100, -42, -4, -74, 105, -14, 80, 24, 22, -79, -50, 114, -113, -18, -47, 35, 90, -23, 58, 27, 56, -15, -15, -7, 18, -170, -186, -41, -34, 7, -29, -49, 51, 18, 13, -66, -8, -40, 39, 1, 76, -5, -84, 52, -71, -33, 19, 84, 22, -51, -4, 18, -12, 7, 124, 137, 56, 13, 44, -36, 73, 76, -3, -6, 52, 80, 96, -12, 85, 40, 72, 165, 108, -64, -115, 39, -29, 98, 8, -23, 5, -66, 103, 95, 39, -33, 0, 4, 27, 16, -85, 90, 124, -46, -74, 44, -58, -89, -72, -139, -28, -103, -33, -25, 55, -28, -16, -22, -89, -80, 50, 8, -70, 27, -10, -13, -1, -121, -26, -7, 20, -132, -23, -149, -48, -3, -17, -15, -26, 10, 61, 50, -23, 8, 99, -112, 44, -84, -11, -4, -7, -59, -53, -124, 84, -21, -53, -17, -42, 71, -47, -112, -83, -17, 17, -25, -47, -76, -20, 82, -63, -23, -52, 73, -3, 63, 41, 16, 2, -8, -9, 2, 7, 52, -76, 1, 44, 73, -43, -11, -28, 92, 3, 25, 51, 25, 85, 21, 126, 43, 38, 92, 27, 102, -146, 44, 6, 5, 2, 11, 10, -2, 3, -4, 29, 57, 64, 10, -19, 107, 33, 77, 9, -38, -83, -14, -32, 33, 43, 10, -77, 4, 65, 21, -19, 21, -40, 71, 20, 19, -55, 39, 65, -50, 25, -19, 6, 20, 46, 21, 5, -12, 24, 50, -42, -68, -116, -37, 31, 8, 37, -73, -9, 9, 114, 75, 55, 8, 96, 40, -36, -18, -95, 33, 19, -28, 4, -59, 2, 16, 64, -76, 106, 37, 17, -41, -166, -42, 20, 18, -92, -15, -42, 2, -72, -72, -4, 17, -104, -87, -47, -24, -1, -38, -28, -125, -7, -85, 53, -59, 56, -116, 53, -2, -71, 17, 65, -73, -34, 29, 85, -16, -1, -11, 24, 148, -36, 1, -59, -9, -32, 17, 19, 2, 69, 32, 1, -116, 70, -14, 41, -2, -34, -53, -11, 36, -110, 61, 88, 8, 99, 10, -84, -34, -28, 4, -3, -105, -34, -67, -63, -40, -91, 4, 18, -88, -26, -27, -10, 38, -5, -88, 98, -41, -81, 11, 66, 29, -44, 111, -36, 11, -163, 45, 9, -17, -52, -2, -116, 25, 61, -41, -124, -27, -48, -110, 21, -81, -4, -16, -49, 3, 3, 2, -4, 23, -24, -59, 1, 12, -55, 17, -8, -7, -2, -14}
, {41, 64, -19, 24, 45, 115, 75, 0, 57, -41, 25, -20, -35, 6, -4, 36, 16, 38, -14, -27, -85, 14, 65, -108, 112, 58, -224, -49, 71, 79, -46, -8, 32, -110, -15, 32, -84, 59, 70, -35, 110, -36, -108, 53, 27, -48, -68, 4, 3, -57, -52, -36, 20, 19, -22, -229, -15, -27, -85, -226, 22, -19, -11, -7, -5, -79, -87, 69, 36, 63, 54, 50, 53, -62, 59, -20, -2, -84, -61, -45, 74, 9, 53, -10, 6, 62, -33, -7, 38, -67, -117, -100, -58, -3, 9, -45, -46, 48, -3, -59, -12, 30, 68, 48, -96, -17, -87, 18, 35, -61, -5, -26, 43, 16, 31, 26, 8, 30, 99, 93, -29, 17, -7, -72, -67, 44, 54, 60, -17, 1, 49, 5, -69, 46, -21, -11, 77, 79, 70, 41, 33, 6, 30, -20, -44, -40, -51, 16, 29, -89, 128, -83, 44, -9, -45, -40, 10, -37, -56, -123, -53, -51, -50, -20, 12, 2, 114, -157, 31, 71, -25, -14, 19, 73, -45, -52, 37, -41, 68, 188, -129, 255, 64, -145, 89, 46, -147, -10, 22, 25, 4, -123, -104, 17, -95, 47, 71, -141, 34, 4, -61, -169, -3, -15, -74, -15, -22, 62, 63, -159, -41, 91, -13, 28, -49, -43, -47, -24, 14, -72, -93, 9, -87, 70, -40, -72, -145, 26, 29, -45, 2, -48, 184, 30, -35, 119, 105, 132, -57, 26, 12, -73, -20, 57, 2, 25, -36, -50, 51, 17, 6, -18, -50, 93, 53, 19, 4, 26, 69, 56, -13, 20, -119, -214, -28, -15, -28, -48, -44, -106, 29, -94, -2, 38, 87, 74, -43, -38, 20, 50, 17, 79, -242, -44, 21, -26, 63, -124, 0, 29, 41, 83, -84, 192, 106, 83, 22, 100, -138, 91, -15, 65, 73, -40, 48, -90, -4, 107, -56, 37, 43, -3, -140, -135, -63, -50, 49, 28, -53, -54, -80, -66, -6, -103, -113, 50, -30, -94, -10, -35, -140, 28, -62, 17, 126, -108, -71, -144, -180, -82, -32, -120, 9, 26, -36, -77, -58, -200, 164, -158, -68, 6, -51, -70, -11, -76, 3, -45, -69, 77, -72, 42, 10, -173, -58, -30, -49, 137, -56, -34, 12, 23, 77, 25, -11, 22, -7, 72, 74, 48, 49, -77, -19, 70, -72, 71, 60, -12, 61, -105, -3, 46, -34, -121, 17, -21, 62, 59, -9, 69, 40, -43, -43, -33, 54, -50, 57, -48, -21, -48, -14, -32, 14, -29, -114, 62, -165, -22, 114, -35, -108, -45, -13, -130, 71, -18, -26, -38, 2, 7, -171, -146, 23, -13, -4, -33, -63, 7, 69, 53, 1, -11, 68, 33, 23, -8, -37, -11, 49, -1, 108, 19, -53, 41, -149, 52, 16, 24, 59, -34, 84, -54, -51, 45, 42, -4, -26, -54, -25, -70, -15, -65, -20, -37, -44, 96, 53, -16, -15, -42, -11, -86, -81, 28, 1, -15, 82, -194, 25, -25, -165, 13, 57, -11, 24, 28, -40, 1, -14, 21, -20, -35, -53, -88, 43, -23, -28, -31, -38, 7, -31, 74, -105, 26, -13, 37, 13, -6, 67, -62, 40, 37, -26, 15, 25, 42, 44, -83, -116, -37, -141, 45, 87, 13, 53, -11, 18, 26, -130, 3, -2, 105, -2, -95, 28, -27, -4, 37, -28, 4, 23, 103, 24, 15, 39, 21, 31, 22, 90, 36, -6, 58, 140, -73, 5, -10, -83, 32, -243, 28, 58, -82, -34, 57, 23, 71, -28, -2, -12, -22, -5, 24, 60, 35, -39, -34, 67, -58, 109, 9, -99, 19, 75, 17, 8, -34, -40, 9, 6, 22, 24, -6, -5, 1, 166, -14, 25, -25, -89, 26, -12, -2, 72, 70, -15, 9, 35, 80, 35, 18, -21, 35, 41, 58, 72, 45, -31, 12, -24, 83, 31, 14, 42, -40, 52, -3, -28, -5, 121, -7, 53, 41, 61, 33, -56, 27, 62, 92, -89, -49, 1, 12, 57, 20, -71, -15, -28, -24, 56, -8, -12, 22, -27, 36, -11, 3, 43, 18, 33, 8, 14, -26, -2, 39, -1, -47, 26, -8, -13, 76, 5, 24, 14, -6, -14, 58, 85, 79, -25, -123, -21, 46, -47, -124, -119, -14, -124, -74, -3, -168, 16, -183, 158, -1, 79, 4, -46, -46, -77, -72, 45, -67, -102, -146, 56, -42, -51, -143, -12, -33, 11, 58, -37, 62, 0, -36, -26, -10, -27, 43, -34, -61, -46, -16, 81, -45, -24, 0, -56, -10, -34, -39, 29, 34, -8, 12, 21, 70, -28, -1, -99, 29, 126, -2, 1, -26, -151, -36, -114, 6, -141, -38, -18, -104, -145, -105, -13, -17, -47, 66, 23, -15, 6, -28, -32, 35, -14, 11, 5, -2, -10, 1, 3, -39}
, {-144, 65, -78, -64, -109, -136, -77, 70, 143, -15, 54, -12, -2, 1, -3, -131, -85, 201, -55, -19, 54, -67, -36, 89, 68, 61, 110, -117, -142, 109, 67, 31, 17, 42, 48, -35, 9, 2, -141, -12, -40, 37, 50, -55, 42, -31, -87, -58, 24, -46, 47, -30, -118, -54, -12, -64, -100, -28, -11, -63, -16, -92, -33, -10, -41, 11, -13, 26, -15, -60, -70, -49, -63, -65, 109, -42, 60, -113, -18, 62, 47, 34, -30, 20, -154, -41, 55, -6, -30, 9, -90, 68, 12, -20, 90, 95, 24, -13, -41, 14, 1, 66, -8, -6, -40, 96, -47, 7, -52, -74, 48, 20, -31, -102, -20, -80, 28, -25, 77, -53, -73, -49, -158, 26, 82, -12, -92, -74, 46, -2, 29, -3, -1, -5, 63, 86, -135, 3, -32, -129, -91, -67, -23, -10, 67, 22, 35, 15, 69, 2, -159, 113, -112, -10, 140, 33, 49, 9, -18, 24, 46, -32, 86, -80, 4, -105, 7, 1, -43, 0, 32, -54, 137, -11, -18, 84, 30, 95, -12, -85, 76, -67, -37, 33, -37, 15, 52, -9, 63, -95, -31, 6, 1, 25, 15, 61, -69, -144, -11, 58, 34, 20, -2, 6, 20, -26, -30, 0, 56, 1, 37, 5, 33, 48, 41, 28, 75, 132, 117, 63, 38, 69, 148, -108, -22, 85, 43, -37, -44, -39, 10, 76, 141, -80, 92, 5, -65, 1, 66, -59, -12, -40, -68, -33, 0, -25, 23, 37, -17, -48, -57, 66, -60, -43, -66, 82, 120, 13, -29, -3, 116, -33, -13, 18, -55, -33, 166, -102, -116, 41, 11, 172, -25, 90, 0, -18, 51, 98, -22, 98, 41, 6, 37, 154, -25, 49, 20, 54, -62, -95, 16, 6, -87, -46, 46, 15, -12, -41, 30, 62, -6, 8, -54, 65, -65, -81, -25, -48, 112, 128, -14, -15, -30, 36, -27, 30, -2, -29, -197, -17, 16, -175, -29, 36, 110, 21, -31, 50, 150, 30, 111, 82, -50, 35, -68, 57, -21, -51, 85, -24, -40, 16, -13, -36, 60, -42, -29, 61, 66, 27, -77, -64, -46, 11, -33, -52, -20, -19, 48, -13, 22, -33, -15, -86, 62, -12, -61, -72, 60, -8, -84, 6, -11, 58, -21, -67, 16, -4, 13, 5, -20, -26, -8, 60, -66, -38, -31, 50, 38, 80, 93, -4, 15, 72, -115, 55, -50, 42, -22, -6, 23, -125, 2, 172, 17, 58, 3, -62, 3, 56, -176, 63, -73, 27, 40, -15, -97, -85, 3, 9, -12, 62, 42, 13, -16, 12, -35, -2, -71, -24, -16, 5, -107, 62, -1, 14, 2, 117, -7, -5, 125, 102, 25, -42, 10, 51, 142, -65, 88, -81, -17, 5, 7, 31, 24, 24, 31, 17, -10, -109, 40, 20, -49, 25, 18, 7, 10, -26, -25, 68, -13, 87, 15, -25, 77, 29, 67, 9, -5, 4, 98, 50, 65, -23, -37, 42, -39, 96, -28, 15, 67, 22, 44, -11, 28, 15, 30, -57, 1, 48, 21, 89, -30, 66, 5, -26, -60, 13, 17, -9, 29, -77, 54, -12, -5, 51, -27, 107, -31, -13, 21, -85, 54, 6, 38, 55, 58, -18, -56, -51, -37, 24, -53, 74, 23, -29, -12, 8, -13, -62, -39, 37, 131, -52, 28, 162, 76, -21, -3, 61, -2, -66, 68, 30, -126, 69, -44, 43, 145, 10, 29, -52, 31, 63, 67, 84, -34, 18, 246, 81, -43, 141, 11, 100, 52, 13, 131, 48, 116, -59, 20, 28, -53, 17, -88, 46, -58, -2, -36, 3, 9, 14, 100, 22, 23, -16, -10, 19, 4, 63, 43, 0, 37, 5, 3, 37, 52, 38, -2, -39, -7, -13, -18, 42, -41, 26, -2, 1, 51, -34, 13, 41, 30, 11, -79, -32, -36, -76, 16, 105, 13, -1, -12, 57, 16, -14, -17, 41, -72, 70, 40, 148, 74, 51, 73, -9, 51, -9, 177, 144, 24, 10, 38, 164, 80, 2, 25, 142, 96, 94, 87, 19, 204, 154, 66, 17, 215, 158, 100, 2, -15, 140, 22, -5, -8, 55, 77, 51, 57, -24, -5, 50, -49, -13, -100, 105, 139, 72, 83, 91, 8, 37, 135, 125, 216, 34, 165, 77, 0, 80, 11, 93, -26, 40, 33, 92, -6, 42, -1, 48, 46, 108, 49, -1, 30, 17, 13, 0, 48, -2, -51, 68, 12, 5, 14, 107, -15, 40, 42, 32, 23, 2, 38, 5, -25, -33, -40, 9, 34, 24, 94, 54, 27, -25, 13, 182, 44, -43, 55, 95, 150, 18, 74, -87, -31, 123, 75, 59, 153, 49, 42, 57, 11, 146, 122, 51, -5, 121, 60, -69, 3, 31, 5, 111, 3, -57, 12, 0, -42, 10, -34, 18, 4, 58}
, {40, 103, -29, 13, 107, 79, 63, 94, 104, -29, 59, 17, 107, 0, -19, 37, -56, -81, -122, -113, 6, -175, -54, 16, -63, -46, 8, -52, -25, 35, -93, -29, -2, 56, 34, 66, 61, 52, 4, 96, -194, -25, 108, 29, -86, -106, -56, -66, 13, -53, 27, -10, 16, 53, -37, 83, -59, -2, -60, -106, 26, -149, 53, -63, -8, 15, 74, 33, 41, -10, -37, 68, 33, 27, 25, -8, 51, 32, 26, 106, -20, 60, 13, -34, -64, -54, 50, 115, -30, -92, 146, 25, 24, -14, -9, -109, -40, -42, -38, 38, 6, 35, 12, 28, 2, 58, 12, 1, -20, -64, -44, 19, 2, -10, 32, -46, 53, -15, -37, 17, -103, -6, -53, 27, -20, 25, 47, 47, -44, -41, -103, -89, -29, -84, -65, -34, -30, -75, -40, -49, 138, -16, -48, -13, -38, -53, -40, -12, 120, -64, -94, 22, -111, -29, 74, -324, 51, -95, -53, -2, 112, 23, 15, -37, -128, -26, -69, 10, -40, -4, 64, -12, 42, 44, -1, 20, 76, 7, 56, -59, -14, -14, -63, 70, -38, 34, 2, 3, 65, -57, 12, -9, 55, -32, 6, 75, -90, -63, 48, -8, 105, -48, 23, -32, 14, 82, -14, -32, -39, -94, -38, 8, 21, 32, -33, -102, 89, 96, -78, -35, -69, 93, 78, -104, 11, 20, 84, -12, 60, 73, 57, -22, 61, -10, -52, 29, -3, 23, 12, -92, -24, -18, 13, -41, -35, 33, 94, -61, -55, 12, -22, 6, -3, 17, 1, -27, 34, 2, 3, 107, -27, -53, 70, -5, 11, -23, -66, 95, 7, -17, 6, 46, 17, -11, 49, 23, 81, 42, 62, -29, 5, 46, 52, 104, -163, 60, 53, -21, -30, 88, 55, -44, 89, 14, -20, 124, 6, -108, 121, 66, 59, 14, -74, 61, 27, -143, 77, 55, 43, -116, 13, -11, 44, -11, 20, 51, 32, 30, -44, 18, 59, 0, 114, 43, 59, 26, 16, -31, 138, -52, 116, 81, -34, 1, -72, 105, 14, 63, 10, 77, 77, -5, -44, 10, 72, -5, 2, 116, -126, 58, -4, -15, -57, -4, -31, 13, 64, 14, 35, -2, 86, 1, -7, 125, 87, -32, 3, 27, 33, -115, -64, 113, -1, 120, 281, 37, 90, -43, -96, 187, -101, -40, 15, 46, 66, -39, -77, 98, 5, 0, 108, -3, 8, -31, -197, 18, -39, 20, 16, 82, 4, 33, 14, 136, -12, 164, 30, 26, 101, 118, -116, 41, -71, 114, 42, -13, 0, -73, -56, 7, 70, -77, 43, 71, 50, 31, -23, 6, -5, 168, -37, 51, 143, -124, -92, 63, 36, 79, 33, 65, 100, 59, 98, -59, 3, -2, -12, 74, -25, -147, -47, -1, -3, -67, 8, 2, 39, -32, 36, -25, 91, -71, -67, 83, 5, 34, 3, -114, 40, -23, 16, 30, -24, 67, -12, 39, 98, 39, -56, 81, 36, -59, 29, 27, -16, -29, 80, -20, -19, 57, -56, 85, -65, 39, -54, -80, -78, -24, -33, 111, -12, 78, 62, -3, -65, -47, -36, 18, 11, -57, -47, -28, -8, -130, -156, 40, 46, 113, -15, 18, -39, -66, -240, 35, -79, -26, 3, 51, -6, -59, -80, 19, 45, 127, -131, -47, -48, -55, -76, -30, -158, -80, -24, 60, -54, 94, 32, -73, 62, 34, 49, 67, 104, 50, 9, 157, 3, 48, 17, 26, 126, 101, 122, 99, -76, 58, -18, 21, -102, 7, 136, -139, 147, -78, -21, -74, 121, 115, 152, -14, 53, -1, -20, 67, -23, -3, 25, 21, -53, 81, -10, 33, 47, 47, -25, 3, -44, 54, 110, 17, 22, 11, 0, 16, 31, -2, 85, 2, -21, -66, -101, 9, -41, 50, 17, -77, 13, 22, -114, 2, -116, -41, -19, -50, -86, -182, -88, 42, -89, 58, -40, -142, 28, -34, -119, -77, -145, -1, -153, -196, -75, -105, 26, 40, -33, 6, -18, -127, 30, 63, 2, 6, -68, 18, 39, 9, 74, 56, 67, 52, 24, 27, 61, 81, 88, 6, 86, 127, 42, 18, 1, 84, 7, -56, 1, -56, 23, 4, -36, -29, 21, -11, -16, 54, -41, -62, -7, 45, 109, 27, -1, 62, 48, 200, 78, -12, 15, 36, 4, 10, 61, -25, 86, 26, 136, 55, 79, 89, 1, 76, 107, 1, -24, -9, 32, 83, 32, -2, 9, 10, 44, 157, 36, -22, 32, 77, -10, -46, -9, 5, 49, 38, -44, -6, 60, -56, -33, 5, 7, 29, 55, -27, 75, 27, 115, 77, 36, -19, 28, 77, 97, -33, -107, -33, 52, 10, 92, 11, 56, 37, 76, 67, 8, 121, 116, 68, 0, -7, 11, -34, 16, -15, 28, 28, -11, 3, -12, 8, -9, -6, 86, -8, 30, -40}
, {-5, 31, 2, 26, 11, 2, 63, 50, 124, 47, 26, 8, -16, 19, -5, 24, 58, -75, 78, 29, -70, 34, -1, 70, -5, 81, 61, -56, -27, -143, -22, 57, 28, 98, 33, 62, 27, 8, -105, 25, -55, -24, 146, -108, 71, 8, 13, 72, 52, -4, 33, -123, 97, -58, 72, -11, -12, 64, 78, -48, 68, -45, -50, -100, -6, -34, -12, 5, 15, -79, -72, -91, -7, -48, 5, -104, 60, -133, -55, 49, -48, -72, -71, 104, -31, 61, 62, -163, -90, -168, -253, -27, -3, -81, 47, 53, -86, -55, 32, -8, 5, 29, -89, -82, 5, 16, 11, -5, -59, 30, -56, 17, 12, 114, 49, 1, -19, -39, 0, 109, 4, -6, 101, -67, 152, -30, -27, -1, 28, 64, 24, 7, 26, -12, -89, 55, -108, 25, 96, -31, 100, -44, -6, 115, -34, -28, 59, -73, 21, -36, -133, -72, -28, -23, 84, -152, 57, -5, 1, 42, 5, -15, 44, -8, 105, -76, 70, -83, -2, 89, 26, -11, 90, -5, -92, -9, -56, 44, 46, 40, 9, 1, 125, -89, 59, 9, -8, 83, 50, -80, -80, -105, 53, 24, 71, -85, -127, 115, -26, -44, 140, 60, -26, 84, 22, 53, -11, -83, -62, 37, 131, -45, -59, 30, 31, -41, -93, -66, 107, -20, 17, -50, -124, -104, 35, 61, 102, 4, -13, 15, 41, 28, -214, -8, -45, -38, -109, -5, 8, 198, 39, 90, -26, -13, -28, -43, -46, 79, -13, 15, 48, -15, 11, 36, 16, 110, 2, 6, 46, -19, 66, 19, 36, -52, 14, 25, 21, -4, -16, 8, 44, -15, -128, -143, 16, -15, 110, -112, -34, -44, 29, 55, -17, -152, 0, -7, -33, -48, -159, -57, 0, 147, -27, 80, 57, -3, 76, 116, -33, 46, 2, 39, 66, -122, -49, -43, 61, 109, 74, 111, 22, 37, 106, 159, 29, 69, 15, 156, 101, -19, -22, 141, 46, 13, -48, 8, 3, 35, -77, 27, -107, -82, 67, 24, -11, -6, 45, 142, 94, 67, 27, 41, -69, 74, 149, 145, 3, 160, -119, 198, 89, 91, 153, 130, -3, 29, -18, 72, -28, 35, 24, 16, 19, -1, -26, 16, 51, -43, -61, -82, 0, 93, -42, -49, 26, -105, 81, 61, -121, 126, 109, 22, 121, -38, -36, -126, -48, 48, 7, -61, -86, -56, -29, -14, -48, -44, -23, -54, -39, -65, 82, 82, -4, -39, 6, 75, -16, -19, -12, 0, 78, 32, -3, -4, -42, 79, 124, 16, -10, -76, 6, 15, 44, 51, -10, 100, 46, 23, 57, -25, 8, 110, 153, -52, 48, -32, -22, 76, -33, -42, 105, 68, -31, -27, -41, 59, 94, -66, 13, 104, 48, 16, 61, 17, 44, 54, 39, 60, 12, 148, -25, 116, 25, 18, -139, -67, 8, -3, 0, 22, 115, -73, 145, 64, -77, -45, -45, 33, 49, -61, -38, -52, -37, 26, 68, 93, 85, -61, 53, 73, -36, -5, -2, 2, -2, -48, 65, 14, -15, -21, 20, 50, -18, 36, -12, 54, -90, -45, 40, -7, 82, -16, 19, -47, 4, 35, 28, 51, -14, 35, -4, -43, 58, 26, -60, 17, 76, 141, 71, -170, 50, 86, -21, -69, -26, 48, 30, -1, -47, 66, -52, 55, -29, 46, -46, -148, 73, 39, -16, -1, -8, 88, 9, 71, 98, 15, 36, 4, 8, 7, -12, 19, 23, 91, 35, -98, 114, -24, 25, -45, -15, 124, -1, -4, 13, -11, -171, -65, -87, -6, -18, -103, 3, 52, 83, 14, -43, 62, -9, 48, -23, -64, -120, 6, 51, 38, 33, 25, -113, 19, -51, 23, 9, -19, 116, 26, -2, -41, 156, 24, 51, -70, 19, -37, -66, 56, 6, -55, 23, -8, -34, 29, -37, 11, 78, -70, -65, 105, -52, -114, 16, -14, -49, -11, -60, -62, 1, -50, -63, -1, 3, -94, -31, -9, 33, -38, -88, -14, -13, -190, -14, 39, -140, -110, -108, -25, -37, -154, -54, -52, -73, -9, -80, -82, -61, 4, -81, -126, -63, -11, 13, -43, 11, -103, 2, -63, -24, -43, 28, -63, 2, -69, 6, 5, 135, -30, -97, 30, 5, -68, 45, 18, -30, 123, -108, -16, -153, -5, 64, -3, 7, 11, 27, 30, -9, -42, 63, -26, -30, 14, -83, -58, -103, -22, -29, 3, 65, -8, 27, -35, -84, 107, -2, 41, 44, 2, -66, -41, -44, -26, 92, 29, -7, -51, 2, -4, -57, 17, -3, -30, 67, 31, -74, -9, 179, 68, 29, 51, -56, 87, -71, -26, -43, -14, 8, -19, -97, 4, 24, 24, -123, -91, 31, -145, 14, -118, -7, -31, -94, -22, -25, -32, 10, 47, -45, 37, -20, 17, 41, -6, -13, -19, -15, -68}
, {30, 58, 44, 76, -121, 30, -12, -2, 47, 71, 74, -22, -10, 15, 15, 39, -6, 191, 115, 30, 91, 41, 28, -65, 4, 106, 59, 47, -62, 180, 213, 65, -29, 142, 54, -2, -63, -21, -33, 50, 50, 74, 125, -22, 50, 167, 60, 63, 23, 8, -10, -10, -60, 54, -78, 36, 1, 6, 168, -55, 43, 2, -55, -107, -18, 74, 81, 111, 48, -20, 51, 11, -52, -77, 83, 58, -53, -2, -4, -101, 6, -119, -88, 93, 18, -32, -11, -171, 0, -29, -70, 43, -19, -10, 21, 46, 67, 29, -40, 23, -5, 55, 16, 53, -15, 109, -45, 24, 23, -49, 33, -15, -13, -16, -20, 21, 22, 47, 113, 116, 5, 62, 22, 108, -42, 184, 38, 38, 78, 13, 21, 25, 51, 12, -25, 74, 1, 28, 43, 6, 110, 18, 23, -40, 33, -11, 12, -80, 27, -35, -176, 8, -87, -49, 95, -58, 70, 31, -31, 37, 37, 37, 4, -144, 47, -153, -54, 47, -77, -37, 51, -101, -35, 2, -23, -28, 43, 38, 11, -3, -21, 12, -11, 54, 15, 21, -24, 65, -37, 43, 30, 109, 3, -57, 11, 48, -86, 63, -48, -30, -37, 62, -96, 10, -24, -40, -45, -4, 40, -57, -15, 21, -17, 72, 36, 17, -45, 13, 78, 38, -11, -40, 60, -94, -23, 64, -28, -30, -46, -21, 41, 70, -21, 20, 109, 66, 18, -36, 97, -47, 52, 37, -104, 37, 27, 21, -95, 120, -101, 26, 49, -39, -86, -27, -1, 93, 79, -29, -9, -25, 56, -41, -46, 27, 20, 21, -16, -66, -15, 21, -41, 41, -31, 1, -25, -36, -99, -17, -43, 48, -41, -11, -35, -53, -9, -116, -12, 72, 46, -4, -47, -8, -62, -101, -84, 25, -48, -44, 45, -4, -19, 10, 15, 48, 89, -25, -112, 57, 128, -95, -98, 103, -187, -91, 82, -54, -82, -112, -61, 62, 74, -4, 139, 49, 52, 40, 19, 29, -62, 46, 46, 75, 29, 64, 99, 33, 3, 98, -81, -25, -107, -36, 49, -82, -36, -132, -65, 99, 58, -81, -170, 1, 75, 169, -37, 90, 36, 26, -41, -68, 62, 29, 36, 34, -33, -54, 69, 27, 70, -21, 17, 138, -4, 18, -128, -10, 46, 124, -41, 159, -70, 68, 147, -17, 10, -18, 65, 6, -15, 11, 51, 5, -2, -115, 2, -135, 147, -113, 37, -20, 62, -105, -62, -65, 38, -103, -47, -214, -72, -45, 107, -90, 77, -92, -75, 89, 59, 8, 82, -32, -116, -58, -25, -140, 2, 9, 52, -92, 42, -4, 42, -32, -15, 111, -6, -1, 42, 23, 96, 99, 57, 82, 63, 79, 115, 91, -58, -5, -52, 71, 33, -36, -5, -30, 12, 39, 12, 33, 21, -107, 67, -56, 7, -28, -120, -41, -7, -56, -23, -30, 99, -142, 74, -6, -60, 66, -74, 17, -31, -66, -9, -11, -130, 7, 31, 43, -13, -26, -6, -3, -34, -45, -56, 26, -20, -37, 67, 24, 17, -4, -25, 1, 43, -111, 17, 44, 98, -32, 31, -85, -25, -17, -107, 4, 84, -41, 15, -108, 42, -17, 90, 49, 49, 8, 36, 54, 60, -163, -123, -10, -35, 19, 111, -25, 95, 40, 60, 111, -31, 63, 51, 71, -29, -44, 66, -135, 78, 99, 23, 37, 6, 2, 51, 17, 45, 1, 4, 7, -67, 80, -32, 36, 76, -43, 12, 48, 139, 61, -146, 1, 101, -87, -2, 138, -36, -152, 19, 39, 24, -1, -30, 35, 32, -24, -129, 14, 17, -121, 163, 24, -9, -64, 52, 46, 25, -10, -20, 32, -72, 29, -92, -6, 27, -78, 135, 13, 28, -36, 237, 124, 25, -25, 17, 102, 38, 82, -36, 98, 20, 58, -100, 42, 84, 21, 174, 62, 21, 138, 75, 69, 35, 51, 46, 29, 5, 27, -72, 50, 213, 68, 29, -73, 29, 149, 74, 106, 34, 82, 52, 1, 95, 70, -33, 30, 175, 38, -73, 36, 20, -19, 31, 24, 15, 13, -64, 29, -55, 42, -11, 41, -22, -62, 2, -83, 25, 70, -8, -15, 44, 16, -30, -111, 49, -32, 93, -96, 18, 3, -196, 38, -57, 71, 13, -66, -3, 17, -168, -50, 48, -7, 29, 41, -19, 114, 2, -68, -12, -92, 14, 4, -26, 96, 43, -26, -119, 88, -128, 28, -122, 99, 13, -42, 0, 21, -12, -70, -8, 88, -29, 21, 76, 59, -96, 57, -211, 55, -43, -13, -151, -15, 9, -19, -43, 17, 69, -61, 42, 40, -28, 38, -105, 3, -50, -19, -160, 39, 44, 19, -78, -30, 117, 105, 8, -30, -156, 124, -22, -57, 122, -9, 14, 106, -6, 92, 118, 40, 33, 38, 68, 26, -94, 15, 6, 110}
, {-23, 76, -44, -16, 75, 29, 44, -14, 10, 15, 68, 14, 5, 7, 14, -9, -16, 145, -45, -54, -11, -31, 0, -27, 79, 23, 41, -114, -17, 116, 14, 29, 17, -18, 28, -17, -74, -22, -52, -7, -16, 12, 59, -103, -41, -19, 6, -83, 10, 35, -46, -57, -26, -28, 53, -15, 41, 26, 140, 80, 29, 34, 2, 1, 33, -75, 61, 21, -16, -66, -51, -65, 104, 107, 7, 20, 18, 56, 20, -14, -58, -29, -48, 18, -2, -13, -34, -85, -9, 44, -86, 55, -50, 43, 51, 4, 46, 31, 30, 18, -16, 7, -30, -9, 33, 4, -7, -18, -3, 21, 30, -5, 45, -7, 39, 61, 14, 48, 57, -15, -103, -35, -62, -144, 20, -5, -21, 0, 66, 15, 58, 7, 27, -11, 35, 39, 6, 62, 74, -144, 70, -55, 9, 70, 43, 115, 57, -53, 10, -76, -63, 17, -124, -24, 21, -42, 106, -65, 37, 21, 107, 44, 71, -97, -31, -204, 8, 31, 18, 96, 6, 54, 72, 76, -3, 1, -42, -27, -26, -82, -111, -149, 45, 20, -28, -74, 2, 19, 36, 30, -7, -118, -6, -50, -191, -101, 11, -66, 100, -16, -93, -8, -128, -51, -4, -125, -31, -70, -96, -68, 51, 0, 25, -5, -106, -93, 25, -73, 83, -39, -98, -20, -142, -38, -81, 87, 9, -21, 56, -27, -38, 39, 41, 31, 33, 57, 20, 71, 55, -5, -36, 55, -48, 11, 103, -63, 22, 136, -55, -30, -16, 7, -102, -37, -49, 102, 24, 34, -15, 80, 138, -1, 50, 70, -44, 25, 87, 8, -90, 4, 45, 106, 7, 94, 68, -26, -36, 39, -62, -43, 31, -13, 46, 8, -58, 53, 35, 14, -9, -3, 21, -136, -114, -177, 34, 59, -57, -40, -56, 16, -24, 12, 42, -89, -22, -30, -22, 70, -81, 81, -28, -27, 51, -58, -37, -6, 18, -9, -199, -160, 145, -92, 0, -40, -26, -24, 9, -119, 18, 9, -57, -90, -120, 6, -49, -109, -63, 23, -18, -74, -46, -76, -86, 11, 95, -74, -41, 162, -3, -24, -73, -87, -30, -42, -92, 16, 4, -101, -26, -5, 62, -41, -28, 38, 27, -73, -75, 6, 28, 21, -86, -55, 48, -17, -49, 1, 34, -91, 43, -27, -35, -13, -28, 14, -60, -13, -25, -11, 78, -15, -238, 31, 56, 29, 0, 45, 8, 19, -44, 35, 22, -85, 52, 16, -83, -39, 3, -94, -28, 10, 28, 7, 25, 11, -58, -86, -96, 60, 91, -52, -74, 78, 8, -3, 70, 15, -89, 44, 33, 105, 48, -105, -154, -36, -57, 103, -13, 71, 42, 100, 16, 16, 38, -38, 5, -72, -217, -40, -119, -102, -43, -96, -105, -51, -166, -45, 92, -71, -94, -67, 18, -25, -23, -17, 51, -3, -96, -11, -128, 45, 2, 67, 9, -59, -13, 39, 93, -37, -142, 54, 158, -128, -32, -55, -41, -73, -20, 23, -135, -102, 82, -34, 0, -122, -134, 129, -20, -64, -3, 22, -79, 5, -21, -70, -17, 2, -28, 37, 16, 22, 47, 37, -11, 4, 46, 90, -14, 173, -13, 33, 47, 35, -37, 23, 5, 72, 6, -44, -98, -35, -20, -46, -2, 95, 41, 34, 24, 44, -34, 54, -23, 58, 139, -60, 4, 25, -101, 9, -43, 7, -19, -61, -35, 23, -88, -212, -17, 30, 71, -46, 4, 65, 106, -56, -10, 94, -155, 62, 14, 109, 45, -61, -174, 47, 10, -52, 288, 50, 91, 28, -24, 14, 183, -70, -188, 50, -47, -78, 11, -113, -90, -39, 162, -2, 43, 34, -25, 79, -2, -3, 44, 20, 33, -51, -38, 44, 85, 7, -1, -32, -11, 22, 3, 90, 38, 38, 50, -26, 29, 61, 5, 58, 68, 2, -52, -115, -155, 58, 63, 248, 5, -2, 15, 107, -65, 45, -30, 20, 52, 24, 65, -6, 54, 53, -19, 32, 17, 8, 47, 107, 15, 36, -57, 33, -3, 27, 17, 106, 18, 51, 2, 17, 27, 42, 8, -18, 62, 46, 41, 13, 20, 63, 7, -24, -13, -6, 50, 72, 41, -58, 2, 88, -60, -26, -42, -12, 99, 106, 136, 111, 38, 121, 101, 147, 183, 32, 57, 79, -16, 24, 121, -8, 153, 48, 39, 72, 62, 120, 74, 20, 145, 81, 105, 13, -4, 125, 11, 32, 94, 28, 73, 87, 16, 90, -3, 95, 42, 33, 29, 11, -27, 16, 171, -14, 38, 23, 19, -11, 70, 94, 119, 70, 44, -4, 194, 156, 98, -8, 55, 196, 198, -16, -48, -77, 135, -1, 123, 38, 75, 75, 35, 32, 2, 163, 169, 44, 25, 53, -25, -66, -21, -6, -8, 98, -55, -42, -1, 14, -40, -16, -1, -10, -22, -18}
, {-15, 10, 88, -6, 0, 10, -32, -16, 10, 48, 35, 0, -32, -4, 35, -27, -2, -47, 32, 28, -53, 20, 7, -110, 16, -56, -25, 35, -2, -60, 15, 15, -20, 0, 9, -41, -53, -7, 62, -108, 33, -28, 3, -153, 8, 72, -47, 36, 21, -28, 1, 21, 52, 27, -44, -6, -18, -2, -7, -49, -12, -59, 31, 4, 41, -17, 11, 54, 61, 44, 17, -96, 11, -59, -38, -21, 0, 37, 55, -42, 47, 43, -29, 3, 86, 53, -64, -12, 74, 65, -40, 137, 23, 4, -3, 53, 168, 126, 14, -29, -26, -1, 102, 32, 66, 48, -26, 17, 22, 40, 189, -18, -13, -47, 31, -19, -30, 67, -21, 61, -68, -27, 64, -58, -71, 86, 19, 81, -21, 49, -53, -4, -17, -17, -11, 24, -97, 55, -74, 81, -69, 10, 55, -20, 78, 40, -15, 65, 14, 10, -15, -27, -46, 85, -81, 49, -80, -44, 46, -20, 91, -14, -26, 40, -13, 84, 72, 45, 68, 79, -27, 73, -88, 51, 86, -63, 59, 6, -17, 28, 102, 89, 41, 64, 81, 42, 8, -13, -14, 71, 104, -12, -2, 53, 15, -101, 50, -37, -25, 79, -12, -78, 189, -48, 6, 0, 6, 93, 97, 49, 18, 69, 102, 4, 157, 119, -63, -23, 160, 81, 63, -26, 107, 28, 45, -84, -13, 95, -8, -36, 63, -23, -62, -4, 51, 2, -117, 32, -58, 30, 40, 61, 24, 133, -6, 25, 87, -65, 116, -21, -102, 21, -131, -2, 36, -106, 25, -13, -38, 66, 29, 119, 83, -162, 67, 20, -97, -5, -113, -29, 35, -76, 128, 17, 9, -53, -117, 62, -107, -1, -93, 13, 13, -47, 49, -64, 71, -51, 171, 48, 16, -80, 62, -78, -98, -62, -10, -19, 84, 5, 56, -51, 7, 54, -7, 63, 75, -62, 16, -23, -9, -26, -4, 53, 94, 10, 38, 20, -110, 35, 81, -80, 74, 41, 64, 44, 54, 36, -122, -72, 119, 17, -41, -48, -33, 32, -27, -59, -72, -8, 2, -21, 36, -26, -40, 8, 46, -172, 255, -103, 19, -119, -183, -174, -10, -19, 28, -39, 45, -53, 58, 3, 78, -119, 164, -6, -85, -68, -139, -52, -46, -74, 40, -55, -82, -21, 6, 25, 167, -183, 256, 43, -159, 5, 43, 95, -34, -68, 94, -114, -33, -71, -62, -14, 74, -49, 37, -15, -31, 104, -43, 74, 61, -95, 99, -120, 56, -9, 49, 2, -90, 81, 90, 61, -29, 28, -153, 44, 34, 28, 33, 22, 36, 11, 81, 30, -105, 77, 34, 80, -98, -28, -70, 36, -40, 36, -75, -5, 67, -12, -65, -23, 8, -56, 20, -1, -128, -103, -20, -45, 52, 6, -34, 10, -81, -38, 102, -10, 69, -43, 132, -55, -18, -15, 13, -46, -12, -21, 69, -28, 13, -10, -37, 48, 96, -123, 129, 28, -28, 113, -61, 26, 55, -83, 20, -77, -100, 81, 0, -40, 85, -158, 37, -5, 19, 105, 5, -49, 24, 11, -13, -91, -19, -10, 8, 73, -48, 61, -95, 4, 40, 12, 22, -47, 12, 161, 15, -16, 75, 28, 50, 77, -202, 49, -92, 80, 60, -26, -107, -43, -66, 67, -28, -61, 6, -6, 99, 66, -116, 33, 6, 155, 78, -67, 1, -1, -33, 92, -80, 18, 90, 42, -53, 23, 61, -5, 68, 21, 54, 26, 5, 37, 76, -9, -35, 79, -17, -18, -184, -15, 80, -138, -35, 10, -1, -58, 45, 90, 80, -18, 21, 12, 46, 46, -102, 84, 50, -60, 162, -62, 45, 60, 31, 103, 85, -96, 42, 3, -8, 88, -67, 18, 36, 22, 77, 5, 7, 99, 154, -11, 22, 19, -35, -31, 53, 26, -29, -112, -36, 39, -117, -48, -13, 10, 180, 19, -17, 114, -9, 90, -47, -15, -70, -32, -13, -22, -122, -63, 97, 11, 45, 92, 90, 32, 25, 148, -37, 78, 74, 68, 52, 80, 1, 88, 135, -62, 20, -27, 34, 33, -20, 29, 65, -17, -75, 25, -13, -18, 18, -1, 13, 20, -6, 98, 3, 92, 22, 15, -59, 54, -24, 30, -15, -31, -55, 111, 110, 36, -50, 16, -17, 48, -33, -111, 73, 53, 47, -107, -22, 1, 54, -30, -1, 43, -69, -34, -107, 39, 32, 19, -95, 43, 49, -26, 4, 11, -72, 79, 75, 36, -18, -105, 9, 54, -10, 35, 56, 48, -66, -55, 78, -53, -45, -71, 1, 26, 38, -26, 35, -29, -15, -1, 38, -9, -117, -34, -34, -4, -59, -24, -103, 69, 128, -20, -103, 38, -88, -14, -76, -37, -27, 108, -61, -38, 2, 163, -5, 84, 12, 15, 15, 9, 8, 26, 26, -20, 9, -12, 1, 21, -92, 11, 6, 6}
, {14, 69, 14, 17, 100, 70, -6, -41, -8, 60, 32, -16, 118, 3, 34, -28, 2, 77, -89, -2, -23, -19, 61, -38, -23, -44, 112, 23, 5, -5, -53, -36, -5, 0, 59, -4, -46, 38, -76, -2, -98, 46, 54, 86, -20, -147, 19, 21, 33, 100, 16, -2, 34, 63, -117, 37, 48, 83, 89, 117, 9, 152, 103, 6, -13, 9, 29, -48, 53, -54, -42, -131, 1, -39, -51, -17, 70, 12, -18, 67, 18, 19, 81, 53, 41, -7, 25, 63, -6, 36, 107, -81, 36, 38, 69, -23, -2, 90, 33, 58, -10, -28, 67, 63, 121, 37, 46, 80, 120, 107, 123, -12, -117, 2, -30, -100, -42, -101, -69, 21, 25, -86, -25, -58, 31, 60, 46, 103, -138, 67, -50, -82, 14, -85, 30, 144, -137, -43, 120, -127, -24, -29, 14, 95, 31, 41, 39, -38, 54, -28, -155, 103, -217, -39, 225, 15, -84, -70, -42, 210, -35, 56, 42, 53, 76, -64, -133, 22, 18, 15, 76, -40, 7, 76, 10, 92, -14, 25, 15, -42, -79, -99, 145, 8, 77, 19, -59, 75, 63, 76, 91, -191, -198, -19, 38, -8, -75, -75, 62, -20, 36, -4, -133, 93, 25, 69, 54, -99, 25, -89, 106, 5, 45, 22, -30, -11, 98, 16, 57, 102, -44, -6, -40, 86, 66, -13, -91, -96, -101, -45, 46, 62, 23, -202, -36, -48, 33, 40, -84, -23, 76, 119, -122, 7, -94, -71, -78, 32, -169, -158, -21, 155, 20, -156, -103, 73, 172, 136, -25, 21, -89, -52, -214, 28, -159, -78, 12, -62, -93, -95, -5, 73, 138, 65, 8, 98, -12, 73, -163, 10, 71, 15, -14, 44, -141, 91, 119, -21, 36, -106, -73, 80, 3, 60, 16, -2, -24, -47, 11, 31, -58, -3, 67, 29, -34, -38, -39, 28, 95, 65, 10, 31, 20, 48, -3, 24, -40, -47, 46, 9, -58, 4, -77, -130, -59, 18, -65, -57, 27, 46, -105, -46, 20, -1, -31, -151, 9, -44, 28, 12, -23, -1, 8, -49, 94, -11, -27, 4, 127, -73, -105, -33, -3, 25, 9, 9, -139, -86, 49, -23, -43, -175, -74, 114, -2, -49, -97, -22, -51, 4, 21, -39, -37, 31, -138, -91, -200, -50, 50, -62, 62, 4, 28, 9, -14, -39, -14, -19, -27, 29, -97, 69, -11, -39, 18, -16, 8, 7, -32, 13, 32, 25, 17, -14, -67, 86, 18, 62, -36, 67, -47, 3, -35, -64, 75, 13, -67, -58, 51, -2, 58, 56, -3, -19, -43, -87, 34, -12, -4, -66, -85, -5, 56, 53, 78, 51, 41, 40, 35, 55, 27, 3, 137, 76, 59, -57, 75, 71, 53, 22, -3, 49, -9, 26, -37, 177, 149, -7, 80, -63, 74, 80, 12, -10, -34, 26, 69, -3, 55, -16, 83, 78, -38, -43, 56, -53, 22, 23, -79, -43, -38, -12, 11, -51, -15, 4, -34, 48, -109, 25, -60, 47, 15, -38, -35, 62, -3, 63, 29, -25, -112, -28, -69, 27, -65, -34, 5, 22, 96, -65, -6, 7, 82, 50, -10, 7, -3, 109, 31, 52, -100, -2, 18, -23, -8, -28, 16, 88, 34, -114, -35, -10, -3, 46, -45, 47, -102, 40, 87, -69, -56, -55, -62, -40, -6, 71, 19, 2, 19, 23, 45, 75, 36, 17, 73, 98, 48, -9, -6, 51, -13, 0, 30, -54, -25, -42, -18, 12, -28, 49, 67, -138, 10, -15, -67, -6, 124, 9, 50, -54, 52, -16, 80, 1, -35, 25, 36, -14, 147, 50, -7, 40, -69, -27, -42, 35, 39, -9, 22, 16, -31, 43, -30, 34, -18, 49, 49, 11, -41, -7, -10, -119, 42, 85, -13, 23, -38, -48, 35, 5, 51, -7, -50, 30, 11, -75, 5, -7, -70, -15, 2, 6, 18, -263, 39, -48, -16, 18, -173, -55, 2, -29, -69, 26, -85, 135, -128, -123, -12, 65, -79, -137, -29, -8, -65, -116, 18, 162, 60, 22, 29, 15, 38, 47, 86, 1, 96, 65, 70, -15, 20, 16, 1, -79, -19, -67, -31, 24, -42, -92, -13, -28, -9, 5, 69, 50, -95, -82, 18, -39, 22, -17, -58, 20, -85, -77, -127, -14, -67, -58, 9, -48, -6, -4, 81, -63, -38, 31, -18, -39, -10, -52, -50, 10, -59, -26, 67, -38, -55, -32, 34, -42, -10, -95, -30, -30, -54, -63, -118, -81, -77, -69, -104, -107, 103, -67, -38, 19, 5, -26, -56, 17, 38, -16, 8, -62, -33, 2, 24, -32, -36, -87, -69, -5, 9, -18, -55, 36, -111, -57, -7, 23, -38, -68, -106, 13, -16, -99, 26, 28, 6, -13, -3, 32, -2, -43, -13, -16, -25, -12, 78, -23, -12, -12}
, {81, -83, 114, 32, 29, 67, 46, -51, -15, 111, -20, -17, 2, 17, 24, 0, 48, 44, 59, 48, -42, 71, 105, -27, 177, 0, 26, 143, 143, 109, 3, -24, 36, -61, -38, 5, 3, 32, 154, -62, 14, 67, -36, 127, -22, 124, 39, 43, -18, -24, 5, -26, -2, -52, 25, -54, 91, 54, -113, 84, -13, 81, 19, -133, -147, -28, 50, -42, 55, -29, 85, -45, -7, -20, -180, 56, -78, 118, 26, 78, -100, -73, 53, -19, 205, -17, -72, 95, -32, 20, 33, 12, -129, 42, -187, 49, -106, 83, 7, -15, -10, -3, -39, 60, -33, -72, 40, 103, 52, -18, -167, 14, -31, 55, 58, -46, -46, -18, -124, 91, 134, 5, 63, 11, 151, 109, 46, 19, -40, -71, -28, -46, 10, -23, -100, 105, 100, 4, 23, 65, 46, 0, -57, 103, -105, -75, -24, 64, 25, 9, 108, -95, 173, 12, -91, 76, 21, 13, -56, -33, -48, -151, -54, 128, -202, 108, 165, -19, 114, 26, -258, 110, -36, 97, -54, -89, 29, -130, -78, 173, 8, 77, 73, 46, 4, -13, -230, 50, -46, -7, 14, -96, 86, -80, -116, 73, 20, 31, -132, -159, 76, 102, -167, -143, -112, 86, 92, -43, 16, 61, -37, -10, -8, 26, -33, -5, 52, 18, -141, -48, 24, 64, 24, 51, 28, 105, 102, -9, 26, 29, 42, 73, 9, 36, 22, 45, -46, 94, 59, 54, 43, 20, 41, 63, -23, 18, -15, 48, 62, 18, 38, 109, -47, -26, 33, -11, 17, 52, 44, 98, 3, 60, 115, 61, -5, 99, 46, 21, 14, 82, 109, -51, 59, 8, -28, 4, -27, 155, -18, 1, -24, -119, 22, -35, 70, 26, 75, -10, 197, 90, 40, -50, -8, 194, -102, -38, -31, -55, -14, -9, 53, -78, 88, -35, 88, 36, 2, -42, -51, -148, -11, 13, 44, 37, 5, -36, 91, -34, 117, -45, 23, 33, -131, 21, -14, 17, 38, 1, 98, 8, -18, 2, 62, -35, -21, 23, -24, 40, 15, -16, 12, -83, 47, -22, 130, -8, -27, 254, -104, 148, -52, 71, -21, 78, -86, -26, -6, -37, 48, -18, 47, 0, 31, 191, -57, -39, 20, 5, -47, 41, -38, 129, 71, 0, 23, 27, -73, -21, 25, -36, -32, 57, 56, 63, -16, -27, 27, 12, -10, 37, -31, -89, -36, 24, 50, -43, 12, -29, 50, 25, -53, 121, 108, 95, 66, 46, -9, 57, 75, -15, -44, 41, -26, 27, 49, 149, -186, 66, 111, 28, 25, -99, 35, 7, 94, 4, 20, 112, 43, 75, 50, -27, 54, -15, 1, -7, 53, -51, 30, 39, -121, -99, -101, 48, 35, -7, -114, 27, 37, 8, 9, -42, -79, -102, -4, 32, -29, -34, -52, 26, 28, 4, -76, -33, 18, -59, 37, -29, -42, 17, -61, -112, -22, -32, -76, -17, -49, -65, -70, -83, -55, -91, 8, -24, 14, 42, -6, -99, -81, 67, 27, 0, -41, -38, -6, -65, -14, -23, 54, 66, 29, -10, -66, 16, 15, -8, -44, 64, 53, 27, -34, -19, -13, -17, 32, 58, 6, 96, 1, 80, -60, -98, -72, 27, -37, -31, -1, 17, -1, -69, -56, 100, 22, -56, 13, 73, -27, -57, -111, 56, -65, -95, 47, 52, 16, -35, -74, -1, -21, 28, 0, 48, -39, -46, 118, 78, -8, -1, 42, -3, 42, 43, -1, 24, -38, 5, 178, 26, -10, -84, 82, -14, 30, -38, -84, -113, 28, -54, 55, 16, -5, -46, -109, -38, 100, 35, 26, -67, 18, 38, 60, -26, 99, 31, 2, -1, -19, 29, 1, -21, 25, -28, -25, 44, -27, -5, 10, -8, 13, -41, -74, 32, -6, 28, -18, -21, 81, 12, -26, 24, -92, 28, -48, -39, 35, 22, -108, 42, 3, -61, -47, -34, 22, -68, -135, -55, -16, -14, 31, 19, 70, 52, 6, -33, -22, 125, -6, -4, -38, -20, -76, -23, 19, -64, 14, -27, 26, -32, 19, -23, -32, -8, -14, -19, 22, 17, 55, 49, 22, -3, 18, 42, -5, 43, -1, 48, 10, -26, 12, 29, -18, -14, -20, 6, 128, -38, 37, -57, -63, -15, -31, -43, -11, 57, -87, -40, -121, 1, -16, -37, 34, 7, -36, 11, 39, 0, 45, -20, 17, -12, -67, 19, 41, 52, -4, 60, 34, 86, -27, 57, 17, -101, 7, 3, -26, -59, 65, -34, -54, 12, -43, -16, 30, 6, -50, 4, 51, 21, -7, 23, 12, -35, -36, -67, 77, -3, -34, 29, -32, -22, -30, -29, 112, 21, 133, -36, 108, -13, 39, -14, 111, -20, -18, 2, -22, -41, 22, 49, -59, 29, -16, -54, -11, -55, -43, 14, 23, 3, -6, 11, 52, -17, -1, -52}
, {48, 5, -46, 57, 3, -26, 56, 95, 95, -48, 45, -1, 41, 6, -16, 77, 2, -61, 39, 22, -21, -36, -67, 158, -72, 7, 63, 60, 66, -44, 20, 32, -9, 31, 29, 39, 3, -38, -119, 126, -43, 52, -35, 24, 10, 41, 10, 38, 12, 22, 73, 66, -27, 15, -5, 64, -67, 21, -5, 47, 10, 34, -16, 104, -42, 84, -39, -71, -95, -120, -74, 28, -16, -14, 165, 39, 66, -66, -5, -16, -71, -72, -1, -23, 43, -189, 9, 90, -75, -61, -24, -92, -48, 19, -1, -44, -139, -94, 28, 84, 19, 48, -80, -58, 30, -9, 69, -22, -61, 24, -112, 20, 16, 65, 19, 42, 63, 5, 114, 16, -47, 117, 9, 92, 1, -36, -5, -17, 35, 53, 35, -78, -11, -25, 42, 103, -81, -15, 31, 35, 44, 68, 30, 4, 12, 27, -20, -5, -64, 3, -88, 64, -78, -25, 20, -19, 9, 17, 6, 7, 78, -17, 13, -73, 85, -42, -39, 76, -51, 32, 80, -30, 5, 34, 98, 73, -104, -47, 8, -99, 180, -121, -140, -31, 8, -51, 130, -36, -12, -67, -111, 95, -52, -42, 8, 57, -145, -71, 2, -98, 12, 29, -38, 6, 85, -45, -85, -93, -63, -66, 76, -51, -16, 31, -107, -66, 38, 23, -45, -141, -75, 46, -7, -104, -7, 50, -27, 52, 18, 72, 124, 50, 27, 18, 3, -73, -57, -49, 49, 32, 60, 51, -32, -37, -72, 11, 14, 78, -11, -33, -45, 42, 104, 28, -41, 29, 44, -32, -46, -56, -68, 11, 1, 71, -26, -44, 75, -48, 24, 2, -36, 70, 30, -25, 4, 55, -55, 92, 55, 98, -27, -50, 133, 54, -4, 35, -58, -19, -48, 10, -33, -2, 21, -116, 25, -3, 76, 32, 90, -10, 44, 67, -34, 56, -72, 55, 17, 65, 77, 114, 27, -30, 124, 118, -51, 22, 52, 39, 34, -15, -21, 87, -88, -46, -21, 9, 38, -32, 168, 126, -136, -41, 79, 69, 0, -45, 86, 62, 112, 69, 57, 29, 24, 13, 36, 41, -52, 59, -73, 57, -18, 38, 17, 29, 28, 38, 2, 40, 98, 2, -25, -3, -15, 21, -38, 3, 70, 5, 79, 0, 82, -17, -3, 37, -8, -13, 9, -23, -97, 32, -36, -12, 72, 3, -20, -27, 4, 36, -42, 97, 32, -39, 12, 41, -39, 24, -15, 21, 21, -161, -7, -66, -13, 166, 25, -21, 89, 83, -36, 44, -71, 68, -70, 47, 77, -57, 36, -73, -100, 36, 156, -52, -57, 35, 31, 141, 28, 71, -38, 67, 28, 49, 73, -3, 69, 59, 59, 131, 5, 9, 66, 120, 47, 16, -30, 75, 123, 104, 15, 0, -38, 17, 12, 3, 15, 83, -36, 42, -85, -45, -71, 20, 46, -17, -98, -80, -40, 73, 67, 13, 149, -8, 6, 35, -11, 30, -84, -37, -5, -13, 35, 10, 47, 15, 66, 86, 114, 40, 59, -7, -63, 112, 22, 60, 21, 54, 102, -16, 36, 62, 22, 30, -87, 13, -24, -46, -80, -55, 44, 46, 59, 34, 133, 54, -23, 28, 31, 3, 30, 52, -106, -107, 32, -6, 41, -95, -21, 50, 129, 98, 90, 11, 1, -87, -96, 24, 11, -20, 39, -11, 10, -7, 29, 92, -7, -13, 126, -16, 20, 35, -48, 17, 86, 60, 29, 82, 6, -2, 74, -40, -66, -44, -105, 6, 58, -44, 117, 60, 254, 17, -128, 198, -27, 92, 34, 35, -119, -133, -100, 53, 5, -40, -46, -55, 7, 32, -48, -33, -20, -23, 22, -55, -105, -63, -61, -49, 33, -21, -84, 10, -82, 43, -44, -22, 5, 24, -99, 21, -71, -44, -32, -62, -64, -48, -107, 4, -92, -79, 27, -86, 15, -80, -100, 44, 7, 18, 51, -164, -88, -192, -44, -23, -46, -161, -10, -25, 81, -44, -174, 5, 48, 54, -2, -9, -11, -168, -41, -64, -35, -87, -40, -20, -5, -109, -68, 59, 24, 144, -39, 17, 9, 43, 12, 17, 33, -1, -27, 8, 3, -14, 4, -42, -2, 47, 15, 13, 37, 27, 79, -18, 46, 77, 47, 9, 50, 10, -57, 4, -50, 3, 63, -20, 16, 88, 75, -13, 18, 147, 65, 58, -56, 111, -126, 1, -50, 35, 76, -19, -28, -8, 5, 12, -110, 106, 71, -39, 26, -12, -1, 8, -94, 40, 38, -13, -27, 20, 20, -48, -9, 4, 74, -22, 75, -54, -54, 3, -16, 40, 99, 55, 70, 21, -30, -23, 60, 55, 53, 6, 67, 40, 72, 16, -15, 27, 63, 71, 39, 10, 119, -8, -6, -15, 43, -20, 98, -29, 0, 9, 138, 22, 13, 74, 31, 45, 66, 2, 1, 37, 23, 11, 5, -1, -1, 130}
, {60, 10, 70, 34, 113, 107, 44, -36, -115, 58, -23, 3, 45, -9, 25, 50, 38, 4, -8, 22, 103, 8, 44, -104, 23, 36, -13, 149, 59, 110, 26, -42, 58, 6, 96, 19, 37, 63, 65, 55, 10, 70, 48, 83, 11, 10, -7, -54, 9, 93, 54, -49, -74, -63, -51, 68, 22, -25, -80, 200, 41, 21, 44, -45, -30, 76, 26, -48, 150, 8, 56, 207, -80, -21, 13, 41, -33, 29, -65, 6, -28, 37, -17, -40, 124, 19, 65, 158, -31, 44, 119, 131, -42, 66, -76, 122, 90, 66, 3, -62, 14, -9, 24, 7, 136, 90, -50, 50, 89, 38, 107, -16, 3, 19, -11, 32, -21, 56, -17, 20, 72, -41, -62, 77, -49, 30, 103, 25, 47, 11, -10, -17, 33, 38, -41, -23, 3, 57, 6, -110, 1, 53, 12, 25, -8, 48, -11, 26, 43, -31, -77, 40, -55, -34, -41, -8, -8, -50, 1, -30, 36, 37, -35, -4, -74, -202, -97, -41, -43, -87, 6, -25, -10, -68, 32, 71, 168, 62, 50, -5, -74, 10, -103, 89, -37, -9, 29, -35, 37, 74, 127, 98, 42, 66, -34, 25, -22, -162, 35, 47, -35, -63, 69, -91, 56, 18, 61, 47, 102, 27, -67, -72, 72, -4, 63, 27, 150, 73, -150, 184, -5, 52, 167, -40, -18, -26, 50, -49, 50, 52, -18, -1, 0, 25, 1, -9, 80, 169, -5, 6, -127, -33, -49, -13, 27, 53, 106, -14, 157, 60, 35, 21, -14, 37, 14, -6, -97, -59, -27, -85, -35, 46, 71, 3, 77, 28, 35, 17, 135, -1, 34, -35, 100, 97, 88, 35, -47, 66, -131, 49, -62, 16, 68, 43, 65, -16, 66, 9, 185, 88, 28, -77, 112, -114, -50, 64, -92, -66, 23, 23, 5, -59, -77, 176, 123, 49, 67, -29, -89, -126, -2, 19, -3, -27, -12, -8, -24, -34, -13, 30, -38, 28, 137, 82, 69, 3, 30, 123, 7, -77, 217, 167, 26, -35, -164, 126, -119, -57, -40, -122, -41, -58, -66, 54, 70, -22, -10, 70, 90, 51, 3, -26, -37, -159, -59, 46, 89, 13, -5, 10, 29, 16, 24, -17, 59, -29, -99, 76, 9, 22, 19, -123, 58, -13, -21, 21, 43, -57, 120, -31, 87, -26, -91, 10, 57, 10, 6, -28, 6, 26, -6, 47, -40, -19, -22, 5, 14, 29, -17, 26, 26, 43, 13, -29, 96, -38, 13, 41, -16, -24, 51, -14, 64, -23, -69, -75, 31, -30, 19, 110, -5, -57, 47, -40, 55, -7, -110, 11, 1, 25, -60, 6, -37, -132, -43, -27, -29, -113, -42, -153, -115, -80, -147, -72, -111, -66, -163, -51, -23, -165, -156, -114, -78, -67, -44, -76, -101, -83, -48, 53, -19, -6, -84, 15, 0, 16, -9, -40, -40, -15, -143, -38, -131, 34, 43, -10, 12, -41, 53, 104, -42, 16, 3, 28, 22, -52, -38, -25, 19, 36, 39, -19, -36, -26, -38, -1, -55, -60, -54, 7, -2, -58, 14, -8, -74, 83, 49, 40, -7, -20, -86, -24, -23, 10, -24, 117, -70, -22, 27, 37, 19, 135, -66, 16, -89, 76, -3, -15, -57, 8, -40, 121, 27, 113, 41, -10, 37, 10, -48, 27, -10, 0, 82, 30, -20, -5, -58, -62, 28, 0, 2, -67, -51, -40, -35, 47, -97, 1, 29, 15, 52, 14, 103, -24, -7, 99, -148, 5, -55, 17, 55, -119, 41, -23, 10, -28, 114, 71, 47, 61, -14, 79, 51, -24, 26, -60, 34, 29, -102, -47, -51, -30, -9, 29, 19, -24, -29, 97, 112, -12, 144, 44, -25, 61, -50, 35, 76, -14, -64, -73, -63, 81, 2, 173, 152, -13, 80, 107, 12, 65, -12, 61, 54, 21, 39, -56, -19, 48, -45, 207, 2, -31, 27, 2, -3, 13, -105, 36, 107, -66, -21, -43, -7, -6, -52, 43, 50, 11, 58, 108, -16, 4, -59, 50, 78, -27, -67, 51, 47, -11, 4, 13, 37, -2, -1, -15, 0, 39, 34, -10, 17, -11, 23, 45, -20, -59, 63, 16, -41, -1, 72, 137, -60, 96, -61, 147, 90, 50, 228, 72, 60, 94, 36, 95, 152, 17, 363, 42, -75, 46, 120, -33, 150, 8, 172, 148, -4, 132, 75, 35, 276, 17, 192, 124, 53, 104, -24, 105, 115, -34, 98, 9, 0, 73, -12, 20, 88, -44, 106, 31, -32, -63, 127, -18, 210, -99, 5, -13, 134, 11, 88, 51, 53, 46, 103, 137, -18, 41, 109, 131, 214, 22, -41, -43, 35, -42, 120, -49, 9, 43, 17, 34, -31, 149, 161, 75, 24, 27, 19, -51, -17, -16, 16, -16, -65, -14, -7, 9, -26, -9, 29, 14, -6, 10}
, {19, -32, 58, 9, -137, -28, 2, -50, 69, 71, 148, 10, -23, -26, 20, -17, 23, 235, 115, 17, -63, 1, 15, 156, 100, -5, 236, -14, 49, 99, 113, -52, -41, 58, 38, 6, 3, 60, -102, -68, -42, 38, 89, 81, 12, 146, 55, -8, -67, 38, 15, -5, 171, 2, 129, -24, -50, -79, 80, -1, 22, 0, 42, -36, -50, -23, -12, 1, -26, 41, -33, -255, 21, -5, -209, -137, 56, -109, 28, -4, 41, 43, 79, 130, -85, 201, -16, -86, 157, 54, -218, -91, 79, -10, 13, -69, 63, -37, 5, 16, 16, 2, 23, -8, 96, -14, 39, -53, -61, 47, 145, 36, -7, 23, -3, -64, -57, 5, -30, 151, 24, 0, 53, 32, 30, 115, 5, -33, -12, -9, -10, -78, 12, -77, -34, 60, -5, -43, 33, 136, 82, -60, -64, 85, -18, -18, -4, 10, 56, -34, -17, -72, 50, 58, 46, -89, -34, 44, -25, 56, -57, -18, 32, 141, -19, 59, 160, 61, 102, 60, -23, 24, 57, 74, -15, -50, -71, -20, -34, 125, 2, 25, 107, -75, 44, 151, -113, 10, 7, 84, -8, -157, -170, -35, -10, 42, 6, 225, 64, 76, 57, 93, -110, 104, -64, 98, -58, -179, 37, -87, 84, 47, 39, -3, -13, 37, 59, -30, 82, 76, -22, 17, -57, 99, 20, 79, -102, -62, -52, -64, 67, 102, 32, -65, 42, 66, 1, -35, 23, 73, 199, 119, -56, -29, -25, -36, -61, 32, -175, -74, -80, -53, 126, -141, -60, 114, 169, 33, -95, 3, -34, 12, -55, 3, -102, 48, -46, -11, -149, -32, 67, -14, 20, -49, -61, -25, 22, -61, 4, 57, -14, 59, 1, 47, -33, 48, 48, -2, -75, -99, -93, 40, -49, 63, 58, -40, 64, 76, -77, 50, -75, 35, 16, -92, -15, -95, -3, 76, -3, 134, -16, -29, 20, 122, -108, -17, -94, -20, -2, -32, -110, 112, -113, -40, -93, 35, 41, 49, 120, 75, -122, -16, 67, 17, 77, -87, 22, 70, 53, 41, -34, 34, 54, -80, -1, 14, 86, -51, 74, -85, 13, 13, 11, 129, 0, 44, -50, 0, -33, 19, -45, -43, -6, -45, -29, 37, 17, -38, -16, -38, 0, -34, -8, -26, -57, 0, -77, 17, 27, -23, 47, 0, 41, 11, 74, 40, 37, 7, -81, 75, 64, 27, -51, -20, 76, 52, 45, -6, -27, -6, 171, 112, -11, 64, -39, 52, -25, -4, -40, 35, -55, 14, 9, -48, 105, 39, -19, 101, 21, -138, 47, 26, -4, -82, -66, 34, 4, -25, 19, -86, 136, -36, 38, 200, 113, 8, 87, 43, -3, 106, 87, 56, 13, 127, 108, 90, 168, 158, 1, 153, 50, 27, 6, -28, 22, 130, 143, 4, 42, 39, 157, 152, 5, -7, 48, -13, -8, -10, 70, 15, 122, 63, -24, -3, 49, -84, -49, 76, -42, -53, -18, -71, 48, -61, -52, 25, -53, 99, -185, -16, -44, -14, 29, -89, -18, 113, 61, 6, 46, -43, 43, -20, -140, 35, -46, 11, 99, -79, 92, -22, 25, 27, 10, 52, 3, -102, 28, 122, -23, -52, -58, -47, 53, -59, 117, 7, 24, 65, 107, -50, -53, -64, -88, -85, 30, 26, -29, -16, 1, -2, 49, -26, 3, 6, 47, 76, 73, -39, -20, 45, -41, 16, 76, 1, -74, 5, 2, -16, 15, -63, -80, 49, 67, -72, -40, 34, -94, 63, 163, 17, 28, 159, 5, 125, -23, -75, 132, -8, -35, 74, 38, 57, 76, 17, -39, 44, 21, 53, 111, -10, 37, -10, 96, -10, 13, -58, 8, -13, 56, 65, -70, -90, -8, 25, 18, 78, 60, -10, 43, -66, -24, 97, 23, -1, -131, 4, 2, 41, -41, -6, -169, 74, 8, 24, 20, -44, 5, 6, 98, -78, -6, 6, 72, 126, 16, 33, 40, 119, -42, 96, 29, 22, -6, 73, 81, -7, 8, 28, -12, -5, 23, 16, 65, 37, -39, 63, 71, 36, 28, 71, 22, 42, 18, 51, 99, -12, 60, 6, 20, 4, 1, 55, 11, 6, 13, -51, -120, 30, -8, -18, -5, -88, -11, -12, -87, -26, -63, -3, -166, -6, -59, 27, -90, -104, -31, -94, -285, -81, -68, -278, 32, -169, 27, 30, 56, -28, -56, 12, 47, 18, -43, 73, 39, -7, -14, -1, -51, 58, -71, 57, -125, 60, 9, -81, -1, 35, -31, 47, 7, 10, 46, 7, -167, 13, -77, 18, -61, 1, -104, -48, -150, 20, 2, -29, -48, -41, 24, -8, -124, -57, -231, -58, -24, 19, -28, 37, -200, 93, -52, -39, -156, -108, -13, -250, -63, -124, -5, -57, -40, 45, 19, -14, 14, -8, 1, 6, -13, 15, -6, -4, -22, 16, -23, -70}
, {55, -54, -44, 34, -19, -20, 14, -33, 36, -33, -21, 19, 47, -13, -6, 39, -2, -133, 26, 52, 48, -10, -30, -60, -100, -25, -74, -21, 1, -141, -25, 116, 45, 57, -4, 54, 5, -38, -83, 46, -43, 6, -95, -49, 37, -28, 37, 102, 8, -4, 1, -22, -45, -101, -2, 0, -19, -48, -120, -3, 50, 62, 16, 124, 39, -3, -33, -55, -73, -34, -4, 109, -87, 41, 118, -22, -64, 21, -23, -9, 9, -41, 64, -21, -31, -25, -2, -30, -11, -1, 128, -59, 15, -92, 2, 11, -143, 129, 32, -4, 1, 31, 104, 94, -29, 51, 19, 53, 112, 3, -7, 33, 75, 81, 31, 28, 56, 18, 76, 23, 104, 55, 55, 119, 17, -43, 61, 110, 42, 29, 57, 41, 20, 2, -55, -16, -46, -11, 50, 40, 123, 73, 51, -15, -15, 21, 11, -52, 80, 82, 158, 38, -42, 2, 85, 8, 4, 28, 42, 22, -31, 35, -4, -92, 143, -95, -160, -39, -95, -31, 78, -164, -40, -127, -27, 21, 15, 47, 9, -64, 2, -184, 16, -85, 11, -30, 42, 34, -21, -17, -79, 104, 15, -37, -13, -2, -32, 77, -14, 19, -52, 41, -71, 40, -24, -30, -105, 24, 47, -89, 23, 60, -40, -23, -33, -50, -69, -120, -12, -8, -60, -77, -121, 81, -43, 96, 18, 75, 5, 64, -7, 45, -149, 91, 93, -88, 113, -15, 50, 64, -91, -174, 40, -148, -8, -57, -125, -35, 22, -73, 94, -41, 193, 19, -88, 54, -69, -18, -6, -31, 32, -107, -106, -25, 128, -51, 75, 27, 276, 4, -159, -7, -18, 3, -24, -70, 25, -34, -127, -39, -74, -34, 75, -108, 25, -37, -34, -36, -38, 37, 104, 157, 35, 35, 22, 25, 41, 37, -38, 38, 53, -40, 25, 48, 36, 110, 115, 86, 12, 44, 9, 14, 26, 46, 22, 18, 27, 70, 245, 44, -97, 145, -34, -41, -49, -5, 36, 48, -68, 33, -160, -33, 107, 26, 54, -18, 72, 61, 98, 45, 52, 81, 68, 128, -4, 115, 63, 58, -102, 145, 113, 113, 83, 98, 131, 14, -48, 30, -41, 121, -68, -18, -19, -42, -99, 43, 61, 123, 40, 46, 13, 25, -71, 19, -65, 27, -21, 63, -105, 39, 7, -80, 37, 45, 28, -31, 32, 19, 14, -26, -85, -35, -35, 136, -39, 37, -5, -5, 28, 6, -31, 30, -25, -11, 1, -59, 14, 4, -10, -26, 24, -24, 10, -15, 72, 33, 74, 8, -85, -54, 39, -29, -16, 52, 3, -65, 118, -2, -51, -26, -8, 117, 124, 90, 27, -44, 22, -10, -15, 7, -16, 5, -33, 80, 19, 7, 95, 61, -15, -6, -40, 59, 88, 18, 63, 92, -88, 72, -68, 16, -131, 57, 72, -16, -19, -33, -21, 111, 7, 101, 88, -43, 51, 41, -18, 126, -106, -18, 131, -185, 8, -18, -8, 116, -85, 98, 1, -40, 59, -10, -119, 37, -68, 28, 48, -189, 67, 17, -20, 82, -13, 49, 13, -27, 22, 6, 22, -24, -41, 33, -10, 5, -16, -43, -11, -65, 56, -49, -10, -17, 38, 12, 33, 23, -12, -23, 3, -55, 43, 13, 8, -42, 71, 41, 8, -48, -20, 1, 49, -38, 41, 2, -150, 10, 74, -63, 69, 10, 0, -33, -22, 13, -30, -7, 29, -15, -30, -22, 95, 62, -63, -114, -131, -13, 22, -67, 121, 10, 174, -52, -118, 86, 58, -19, 33, 13, -193, -43, -140, 102, -11, 16, 12, 35, 109, -16, -18, 57, -84, 25, 31, -7, -174, 41, -4, 11, -7, 42, 4, 9, 67, 64, 6, 37, -5, 22, -57, -2, -79, -48, 8, -9, -18, -14, 26, -16, 53, 107, 7, -28, 107, -33, -24, -29, -12, 12, -8, -19, -26, -257, 62, -31, 32, 63, -19, -6, 129, 11, -12, 10, -19, 6, 108, -52, 64, -155, -18, 15, 73, 77, 94, 14, 1, 93, -37, 31, -118, -73, -38, -16, 3, 8, 35, 52, -29, 18, 25, -17, 44, 6, -21, -88, 23, -48, 3, 4, 2, -10, 41, 20, 36, 51, 31, 52, 136, 42, -101, -27, 52, -60, 67, -22, -22, 78, -11, 4, 10, 16, 17, 95, 24, 109, -100, -45, -98, 87, 86, 60, 15, 131, -17, -30, 20, 147, -14, 3, 289, 43, -7, -53, -53, -29, -1, 61, 131, 33, -31, -35, 264, 112, 98, 101, 54, 243, 30, -46, -45, 37, 97, -34, 60, 44, -57, -38, 26, -25, 22, -2, 86, -49, 20, -39, -38, 31, 8, 11, -14, 29, -23, 18, -1, -38, 14, -47, 109, -66, 10, -48, 25, 15, -3, 16, 11, 31, -7, 16, -13, 12, 10, 17, -39, 13, 10, 19}
, {0, -111, 59, -1, 97, 7, 25, -56, -76, 69, -36, 10, 69, 18, -20, 37, 32, -99, 0, 10, -60, 79, -26, -52, -2, 44, -127, 36, 83, -37, -22, 54, 45, -82, 1, 34, -51, 10, 11, 73, 44, -27, -88, -136, 19, -53, -66, -3, 1, -40, 29, 44, -23, -8, -21, 92, -78, -48, -13, 66, 19, 64, 11, 182, 31, 15, -77, -120, 68, -25, -59, 48, -24, 40, 84, 6, -18, 76, 54, -62, -38, -44, 54, -23, 172, -40, 37, 83, 25, -18, 99, -63, -16, 3, -77, -57, -107, 48, 27, -25, -20, -46, -18, 24, -23, -95, 38, 44, 50, 71, -132, 23, 63, -10, -8, 53, 45, 13, 65, -7, 83, -61, -14, -2, -24, -26, -8, -63, 42, 62, 25, 49, -37, 8, 35, -53, -56, 34, -66, -83, -88, 113, 118, 42, 56, 36, -42, 9, -66, 37, 36, 44, 58, 23, -90, -3, -127, 26, 43, 45, -12, -28, 4, -5, 1, -75, -133, -35, -15, 57, -28, -35, -49, -27, 18, 33, 34, -48, -5, -106, 38, -70, 15, -33, 1, 0, 72, -54, 43, 33, -31, 11, -53, -20, -76, -116, 96, -53, 56, -22, -54, -110, 87, -74, 9, -91, -88, -24, 0, 51, -99, 18, -84, -53, -75, -36, -64, -65, -152, -109, -39, 8, -25, 137, 48, -66, 55, 47, 36, 45, -4, -39, -52, 71, -14, -78, 40, 50, 34, 60, -39, -71, 65, 30, -14, 89, 63, -139, 164, 18, -65, 63, 5, 84, 51, -141, -85, -8, 58, -56, -21, 45, -3, 32, 96, 18, 30, 3, 130, 15, -12, -129, 62, 47, 53, 57, 11, -11, -18, -16, 48, 7, 34, -40, -5, 34, 42, -58, 57, 0, 128, 70, 209, -96, 29, 34, -13, 49, -36, 5, 2, 22, -28, 93, -97, 103, 104, 36, -158, 107, 128, -75, 129, 135, -113, 32, 49, 161, 124, -95, -207, -61, 39, -18, -53, -10, -64, -12, -72, -73, 21, -85, -31, -22, -94, -81, -28, -148, 14, 52, 67, 14, -17, 19, -36, 126, 30, -236, -99, 146, 43, -85, -156, -212, 75, 10, 50, 39, -3, 33, 96, 93, -14, -3, -65, 77, 66, -49, 39, 47, 23, -141, -18, -17, 7, 102, 57, -22, 69, -47, -54, 41, -72, 7, 76, 87, 37, 62, 13, -103, -100, 51, -41, 25, 0, 59, -49, -37, 23, 13, 16, 28, 36, 33, 98, -22, 34, 108, 24, 43, 10, 28, -39, 72, 101, 18, 25, -8, -83, 127, 33, -21, 42, 67, -3, 68, -104, 79, 22, -43, 27, 50, 22, -120, 14, -74, -71, -94, -35, -157, -75, -53, -151, 6, -102, -31, -52, -92, -31, 16, -44, 13, 37, 26, 24, -80, -200, 17, 32, -11, -90, -40, 56, -49, -83, 41, -6, 20, -18, -25, -118, -38, 20, 41, -26, -83, -10, -44, 94, -31, 39, 34, -66, 4, 67, 29, 75, 4, 63, 27, 41, -149, 19, 7, 9, -7, -24, -21, -96, 62, 68, -48, -19, -32, 19, 39, -78, 36, -89, 0, 5, 21, -9, -7, 16, 55, 63, -33, 29, 55, -12, 55, -96, 50, -56, 16, -55, -58, -70, 76, 48, -69, 26, 44, 35, 8, 41, -13, -32, 24, 44, 5, 40, -1, -14, -28, -7, -23, -31, -19, 52, 123, -75, -5, 105, 108, 34, -28, 125, 27, 54, -59, -14, 18, 24, -4, -61, -62, 3, -60, -51, 127, 62, -74, 39, 51, -51, 1, -56, 63, -32, 28, 108, -62, 103, -41, -16, 52, -127, -25, -20, -27, -39, 17, 30, 32, -30, -5, 78, -68, 95, 61, -24, 58, -7, -16, -28, -36, -106, -40, -43, -71, -11, -11, 40, -47, 112, 0, -18, -43, 75, 18, -38, 20, -53, -114, -80, -8, -40, 7, 29, -26, -67, 47, -23, -51, -28, -35, -28, -2, -38, 56, 15, -69, 1, -133, 52, 9, 67, 37, 85, 35, 15, 8, -125, 18, -20, -36, -67, -38, -31, -34, -20, -52, -71, 0, -29, -48, 24, -6, -5, -25, 14, 92, -22, 41, 61, -17, -40, 82, 53, 142, -8, 27, -18, 114, 88, 38, 168, 53, 51, 50, 55, -59, 104, 90, 254, 36, 0, 95, 122, -20, 75, -54, -11, 60, 2, 99, -1, -29, 36, -7, 139, 31, 93, 56, 152, 48, 101, -49, -34, 89, -3, 133, 41, 8, -12, -13, 116, 89, 60, -13, 191, 88, 145, -89, -20, -17, 52, 75, 80, 27, 49, -3, 16, 148, 67, -57, 92, 68, 124, -19, 39, -15, 145, 18, 41, -66, 75, 111, -70, -18, 19, 89, 205, -49, -19, 57, -90, -6, 12, -53, -2, -51, -69, 12, 5, -24, 16, -22, 33, 4, 7, -72}
, {-63, -27, -91, -45, 17, -39, -70, 4, -40, -54, -105, 29, -35, 1, -36, -56, 14, -83, -143, 28, 77, -4, 17, 110, 2, -89, -3, -33, 103, -90, -144, -41, 2, -13, 9, -30, 47, 9, 63, -43, 28, 10, -72, 90, -44, -131, 4, 44, 21, -29, 24, 30, -72, -18, 34, 31, 4, 33, -44, 15, -19, -27, -36, 82, 32, -70, -24, -11, -5, -15, 14, -72, 4, 39, -197, -33, 43, -89, 9, 85, 11, -1, 8, -12, -97, 54, 3, -76, 13, 2, -149, -18, 20, -80, 51, -28, -98, -64, -3, -20, -5, -46, -89, -78, -186, -105, -4, -59, -89, -106, -40, 17, 14, -92, -95, 15, 23, -79, 10, -78, 25, -50, 5, 28, -13, -104, -89, -15, 25, -9, 15, 43, -15, 50, -53, -137, 32, -8, 8, 79, 92, -7, -12, 34, 69, 24, 22, 68, 21, 83, 134, -116, 119, 51, -98, 124, 13, -16, 12, -71, -13, -7, 46, 58, -32, 59, 80, -71, 86, -36, -128, 22, 4, -25, 9, -38, 10, 47, 3, 59, -75, 134, 3, -23, 13, 141, -84, -4, -55, 62, 35, -48, 126, 45, 7, 119, 86, 161, -25, 53, 39, 77, -24, 9, -31, 70, 60, -17, 15, 156, 42, -16, -52, -33, 44, 34, -131, -144, -31, -30, 67, -52, -26, 42, -2, -30, -119, -30, -69, -154, -42, 38, 84, -54, -25, 52, 133, -65, -54, -18, -71, -76, 101, -4, 34, 2, 17, -50, -8, 12, 21, -32, -15, 45, -24, 37, -76, 47, -9, -61, 66, 42, -37, -5, 8, 2, 48, -73, 75, 51, 28, -132, 25, -1, 110, 20, 51, -17, 102, -29, 98, -38, -45, 21, 41, 58, -12, -124, 52, 81, -45, -56, -138, 7, 146, 89, -17, 71, -151, 26, 36, -76, 71, -240, 23, 112, 26, -22, 19, 188, -78, 12, -22, 85, 20, 33, -106, -21, 73, 34, 17, -56, -24, 58, 25, -11, -29, 0, -46, -65, 10, -76, -46, -16, 39, 77, 22, -32, -100, -1, -50, 23, -11, 5, -99, -61, 3, -69, 24, -121, -9, -114, 25, -82, -43, -14, -4, 44, 114, 58, 27, -8, -133, -37, -56, 126, 15, 33, 79, -82, -94, -22, 4, 17, -45, 1, -41, 130, -44, -152, -32, 49, 47, 72, -74, 9, 46, 27, 30, -13, 85, 150, 45, 66, -12, 201, -133, 108, -36, -98, 12, -54, -93, -5, 13, 0, 56, 95, -82, -176, -49, 13, 13, -66, -139, 59, 240, 69, -107, 29, 18, -59, -86, 48, -160, -26, 100, -132, -11, -117, 58, 169, 52, -17, 39, -37, 32, -23, -34, -15, 21, 17, 3, 34, -16, 51, 83, 16, -42, 44, -85, -44, 36, -47, 54, 24, -72, -71, -56, 94, -43, -24, 7, 96, 89, -30, -12, 66, 6, 17, -139, 36, -112, 8, -52, 24, -107, -19, 121, 23, -105, -28, -76, 126, -74, 71, 79, -48, 37, 45, 19, -47, -58, 98, 66, -55, -50, -88, -75, 35, 60, 60, 130, 35, 18, -9, -44, -5, -82, 25, -74, -37, 17, 97, -29, -182, -71, -14, -53, -29, 13, -42, 24, -92, 65, -103, -62, 38, 166, 76, 49, -56, -29, 19, 0, -15, -84, -79, 140, -42, -27, -112, 0, 135, 12, -3, 21, -38, 31, -16, -51, 0, 46, -25, -24, -26, 0, -7, 2, 15, 1, 78, -41, 15, -83, -10, -47, 84, -6, -46, -69, 82, -17, -80, 78, 10, -102, -12, -62, 38, 25, 32, 59, -12, 181, 9, 32, 104, -103, 63, 32, 0, -90, -37, -64, 47, -21, -20, 202, -23, 156, 34, -16, -5, 55, -39, -8, -50, -146, -49, 0, -30, 70, -153, 11, -21, 53, 42, 75, -43, 237, 7, -20, 40, -25, 81, 75, -93, 7, -130, 4, 61, 16, 52, 174, -12, 68, 68, -99, 83, 20, 150, 173, -3, -25, 47, 56, 29, -49, 52, 108, -22, 117, 14, 156, -2, -50, 53, 93, -63, 16, -32, -27, 75, 60, -37, 121, 31, 112, -22, 26, -70, 8, 81, 7, -10, -13, 51, 12, 69, 59, 110, 12, 12, 0, 110, 19, 34, 33, 14, -12, -26, 29, -78, 96, 45, 155, 60, 4, 31, 67, 2, 51, -21, -11, 24, -26, 13, -23, -55, 15, 25, 64, 16, 110, 19, 150, 64, -70, -20, 1, 23, -11, 59, -69, -15, -4, -34, 19, -25, 53, -65, 0, 24, 83, -73, -10, -15, 96, -112, -23, 14, -74, -55, -161, -100, -43, -58, 25, -131, -52, -25, 113, 36, 30, -7, 42, -20, -74, 25, 95, 42, -16, 35, -102, 91, 22, 72, 45, -5, 4, -29, 13, -86, -40, 31, 14, -4, 12, 25, 36, -12, 16, 22}
, {42, -36, -9, -10, 17, 14, -4, -18, -42, -20, -67, 6, 21, -26, 13, 40, -7, -85, -115, -80, -6, -59, -52, -26, -14, -87, -117, 12, -45, 12, -9, -2, 48, -60, -70, 102, 96, 59, 80, -80, -97, 38, -23, -63, 62, -107, -49, 73, 0, 6, 30, -104, -89, -51, 81, 68, 48, 38, -13, 52, 1, 52, 94, -39, -8, -113, 29, 57, -42, 78, -22, -40, -137, -40, -94, -139, -89, -67, -180, 29, 11, 47, 140, 53, -20, 213, 27, 83, 149, 107, -102, 66, 72, 24, 46, 69, 173, 101, 23, -55, 15, 8, 71, 30, 60, 102, -35, 87, 60, 15, 84, -38, -37, 55, -65, -13, 28, -52, 10, 91, 20, -44, -65, -11, -12, -3, 19, 51, -86, -65, 3, -96, -45, -95, -85, -30, -35, -2, -90, -61, 40, -43, -40, -27, 24, -71, -17, 37, 56, 30, -77, 23, -49, 46, -13, -109, -101, 16, 7, -118, -54, 21, -12, -154, -78, -141, 46, 41, 75, -101, 10, -27, 42, -37, -49, -19, 130, -33, -47, -12, 51, 22, -145, 55, -21, -6, 53, 0, 27, -129, -18, -31, 150, -33, -7, 111, 157, -100, -24, 1, 46, -47, 11, 38, -39, 55, 19, 109, 67, 13, 103, 53, 63, 9, 21, 31, 100, -13, 171, 40, 17, 17, 39, 191, -76, 11, -13, -51, -78, -118, -101, 78, -89, 11, -13, -61, 5, -31, 27, -118, -4, 68, -30, -86, -45, -33, -27, 64, 86, -118, -12, 12, -134, 27, -27, 34, -36, 41, -14, 26, -51, 92, -17, -93, 35, 84, -41, -25, -193, 20, 67, -6, -31, -99, -208, 30, 47, -17, 72, -59, 71, -170, -121, 43, -36, -119, -251, 40, -25, -130, 48, 16, 59, 102, -72, -25, 79, -4, 99, 49, -194, 88, -15, 62, -85, -42, -41, 11, -19, -32, -9, 4, -58, 37, 0, -13, 48, 15, 28, 2, 22, 37, 34, 20, -29, 9, 11, 13, -32, -34, 17, 26, 4, 2, -8, 59, -76, -56, -24, -109, -134, -73, -75, -20, -26, -85, -82, -5, -35, 10, -26, -87, -16, 35, -136, 2, -69, -22, -1, 19, -120, -45, 84, -44, -38, -79, -60, 19, 30, 19, 0, -40, 21, 1, 13, -65, 114, -114, -83, -23, 61, 37, 24, 125, 82, 10, -97, -44, 51, -7, 46, -135, 42, 63, -111, -100, -35, -9, 15, 41, 90, -40, -128, -58, 21, -99, -12, -59, -70, -32, -55, -69, 24, -56, -15, 2, 23, -39, -41, -20, -1, -47, -61, -62, -39, -68, -86, 6, -125, -69, -5, -22, -65, -29, -5, -41, -37, 17, -65, -47, -11, 7, -18, -75, -24, -5, -15, -39, 7, -3, -67, -45, -93, 4, 25, -5, 30, -24, -47, -32, -16, -51, -50, 43, -18, 61, -30, -52, -85, 4, -6, -75, 38, -53, 12, -18, -31, -113, -92, 20, 11, -7, -3, 57, -18, 31, -38, -23, -104, 18, -35, 32, -32, -16, -5, 20, -7, 95, -9, -6, 28, -68, 12, -11, -17, -12, 34, -25, -13, -13, -10, 68, -52, -22, 90, 32, -82, -50, 14, -5, 78, -84, 48, 9, 70, -42, -27, -18, -55, 29, 11, -49, -103, 100, -8, -10, -65, 11, 21, -85, 26, -32, -85, -24, -97, 29, -36, -7, -16, 21, -40, -38, -27, 14, -28, -84, 49, -30, -20, -63, -52, 139, 95, -5, -10, 22, -49, 76, -38, 96, 99, -28, 2, 28, 24, -74, 73, 125, 105, 18, -6, 57, 91, 113, -46, -13, 91, 0, -84, -27, 43, -27, 56, 29, 26, -95, -67, -20, -24, -29, 96, -64, 3, 6, 39, 13, 22, -49, 28, 80, -67, 22, -11, 29, -29, -54, -146, -28, -174, -27, -72, -62, 27, -2, 25, -33, 11, -32, 81, 5, -6, 1, 40, 34, -66, 68, -106, 5, 27, -10, 63, 0, -54, 10, 25, 58, -40, -104, -5, -25, -153, 19, -70, 8, 36, 62, 35, -17, 7, -11, -31, 27, -29, 17, 11, 0, -3, 8, 28, 9, 8, 12, -19, -42, -17, -115, -35, -44, -22, -65, 32, -12, -33, 34, -99, -13, -93, -100, 15, 63, 25, 63, -7, -9, -60, 42, -30, -42, -135, -81, 8, 1, 55, 24, 70, -26, 7, 58, 65, 32, 78, 70, 56, 8, -39, 63, -108, 59, 48, 38, -7, 20, 11, 24, -59, 50, -29, -105, 9, -89, -46, -68, -25, 5, 13, -112, -41, -8, -13, -85, 11, 33, -66, -8, 8, -52, -54, -10, -21, -22, -21, -14, -10, 9, -24, -33, 19, 0, 39, -21, 10, 9, -30, 23, -24, -4, -20, -29, -8, 10, -12, -3, -29, 12, -14, -11, -14, -22, -53, -3, 51, -1, 19, -13}
, {-17, -35, -6, -21, 19, -26, 10, 3, -4, 13, -30, -5, 47, -9, -24, 22, -27, 15, -114, -50, -125, -7, -55, 14, 55, -34, -44, -126, 104, -47, -52, 89, 34, -65, -93, -75, -29, -64, 14, -152, -51, -108, -92, -126, 14, -73, -48, -106, -26, 41, -25, -25, 7, -45, -71, -202, -5, -31, 74, -75, -52, -11, 32, 1, 47, 17, -32, 23, -72, -17, 113, 15, 5, 51, 1, 26, 21, -53, -38, 75, 89, -26, -35, -84, -128, -29, 11, -95, 5, 141, -161, -41, 29, -102, -11, -41, 37, -18, -73, -12, 2, -14, -20, -3, -17, 3, -16, -20, -48, -16, 76, 27, 41, 39, 95, 19, 43, -10, -24, -51, -75, -20, 78, -35, 72, -73, -25, 53, 3, -6, 49, 10, 45, -57, -54, 40, 73, -23, 61, 4, 59, -13, -47, 155, -12, -69, 35, -24, 74, -10, 126, -116, 73, 16, -16, 60, 137, -18, -21, 112, -71, 23, -56, 55, 64, -5, 68, -55, 44, 59, 52, 11, 31, -64, -53, 12, 20, 41, 66, 65, -113, -100, 82, -59, 124, 0, -50, 83, 5, 39, 36, -29, 24, 21, -14, -35, 13, 68, 42, 19, -21, 139, -31, -4, -2, 70, 64, -64, 113, 77, -30, 23, 14, -36, 20, 22, 69, 48, -29, 5, 59, 70, 96, 84, 7, 39, 13, 30, 34, 6, -105, 95, -130, 31, -40, -35, 19, -62, -47, 62, 27, 29, 92, 31, 43, -8, 30, 90, -16, -14, 139, -42, 65, -21, 12, 94, -61, 30, 21, -22, 58, 26, -22, 79, 51, -6, 86, 119, 116, 28, -15, 1, 0, 17, 13, 1, 63, -58, 9, -78, 21, -1, 34, 12, -8, 26, -32, 20, -47, -23, 64, 63, 48, 92, 102, -14, 99, 10, -4, 89, 16, 87, 81, 11, 61, 25, -3, -89, -22, -13, 33, 37, -54, -32, -1, 33, 45, -57, 27, 20, 128, 0, -58, 39, 47, -20, -10, 71, 87, -60, 113, 79, 11, 18, -16, 44, -51, 78, -26, -56, 13, 57, -24, 83, -46, -27, 28, 165, 44, -11, 153, 69, 44, 89, 7, -30, 73, -35, -10, 97, -12, -23, 66, 11, 53, 30, -21, 234, 9, 68, -14, 4, -15, -131, 4, 44, -66, -118, 123, -63, 39, -46, -86, 141, -28, 46, 22, 44, 25, -67, -105, 110, -86, 30, -26, 56, -4, 57, -117, 57, 76, 6, 41, -27, -70, 63, -3, -9, 71, 47, 69, 4, 38, 21, 25, 55, 20, 62, 32, -88, -93, 42, 63, 34, 12, -60, -16, 21, 30, 52, 27, -16, 180, -94, -8, 78, -42, 43, -116, -9, 106, 65, 7, -9, -87, -60, 43, 140, -60, -7, -23, -48, -17, -42, -68, 8, -65, -15, -30, -11, 33, 63, -5, 37, 139, 28, -73, -36, -66, -9, -57, 95, -127, -109, -18, 124, -31, 4, -36, 129, 15, -13, 35, 63, -113, -61, -136, -6, -64, -56, -71, -44, -43, -78, -4, 171, 24, -15, -15, 156, 20, 42, 44, -17, -34, 85, -24, -20, -139, 61, 65, 3, -112, -53, -3, 33, -17, 215, 34, 35, -1, 76, 1, 28, -50, 36, 20, 20, -27, -49, 73, 5, -14, 41, 38, -16, 66, 23, 94, 53, 17, 10, -29, -55, -47, 76, 23, -106, -60, -27, -23, -25, -6, 17, -3, -15, 11, 13, 69, -44, 121, 4, 136, 32, -1, 124, 31, -13, 28, 37, 124, 83, 42, 2, -25, -19, 202, 173, 64, 35, -84, 97, 26, -26, 140, -58, 20, 116, -136, -34, 55, -55, 121, 66, 9, 137, -42, 79, 117, -49, 136, 34, 6, 48, -178, 61, 113, -52, 0, -96, 12, 140, 57, 28, 146, -37, 124, 76, -69, 1, 14, 20, 100, -90, -54, -41, -75, 143, -3, -43, 31, 2, 93, 190, -35, -22, 92, 79, -4, 7, -37, 154, 12, -59, 63, 55, 37, 91, 78, 174, 22, 83, 82, 124, 46, 50, -27, -45, 38, 48, 66, 15, 104, 100, 3, 7, 124, 6, 113, 2, -13, 72, 1, -42, 27, -57, 66, 38, 53, -38, 77, 27, -48, 51, -14, -9, 1, 51, 244, -76, 69, 51, 55, 183, -53, 75, 53, 18, -58, 113, 101, 46, 29, 33, 16, 3, 75, 64, 74, 147, 1, 3, 27, 158, -1, 35, 145, 75, -13, -9, 14, 34, 25, 57, -23, 142, -4, -93, 22, -18, 28, -43, 85, 2, 100, -72, 59, 44, 114, -42, 40, 32, -15, 21, 45, 20, -69, 63, 25, -43, 38, 29, 51, -16, 122, -2, 45, 62, 34, 32, -3, -60, -5, 48, -6, -75, -5, 25, -117, 8, 30, -33, -12, 61, -48, 28, 1, -13, -15, 9, -4, -2, 19, -78}
, {5, 36, 70, 11, 68, 41, 1, -1, -23, 60, 95, 20, -96, 9, 12, -14, 46, -74, 37, 51, 85, 25, 34, -156, -82, -77, -51, 25, -15, -79, -48, -57, 19, 7, -17, -3, 53, -10, 118, -28, 71, 42, 15, 55, 47, -100, 81, -15, 53, 29, 42, -17, 27, 19, 22, 46, 143, 60, 109, 43, 45, 142, 79, -49, 35, -8, 83, -95, -78, -66, 87, -39, 74, 5, -106, 41, 45, 127, 59, -87, 28, -49, -55, -24, -21, 17, -78, -78, 35, 19, 20, -23, 4, 56, 25, -2, 20, 81, -24, -10, 7, -17, 27, 36, 43, 75, -50, -15, 26, -30, 91, -35, 37, -32, -20, 83, 9, 28, 9, 42, -83, 47, 31, 48, -129, 51, -6, 6, 29, -12, -8, 43, 90, 20, 36, -2, 101, 17, -45, 56, -19, -22, 12, -42, 27, 68, 15, -12, 87, 42, -99, -7, 81, 7, 2, 86, 22, -42, 5, 102, -61, 43, 28, -33, -49, -41, 40, 31, -11, -6, -75, 55, -14, 7, 0, 51, -20, 19, 48, 19, -44, -27, -11, 45, 6, -31, -27, 77, 38, 57, 42, 39, -67, 11, -4, 24, -11, -3, 16, 2, 32, -57, -98, -10, 17, -100, 43, 17, -18, -42, 35, -3, 72, -7, -34, -37, -6, -46, -16, 23, -62, -21, 5, -52, -71, -74, -129, -13, -34, -94, -63, -48, -28, -22, 21, -52, 166, -83, -8, -58, -118, -109, -108, -47, 115, -158, -31, -23, -96, -1, 55, -124, -41, -41, -68, 21, 19, 157, 3, 56, 79, 83, -45, 74, -15, 33, 105, -58, 18, 65, -22, -31, -21, 18, 42, -192, -34, -8, -63, -11, -26, -140, 74, -62, 64, -45, 20, -28, 23, 5, -43, -339, -102, -198, -90, -55, -149, -141, 61, 2, 36, -114, 60, -51, 129, -39, 28, -21, 80, -65, -66, 33, 14, -172, 104, 50, 24, -37, -95, 71, 60, -13, 100, 18, 71, -7, 34, -6, 73, -33, 69, 82, -51, 13, 71, -5, 49, 21, -78, -75, -109, -33, -68, 85, -15, -113, 19, -55, 236, -155, -14, 100, -4, -24, -113, -131, -46, -153, -31, 24, -124, -144, 88, -119, 198, -167, -172, 179, -110, -51, -118, -52, 42, -136, -24, -25, -249, 33, 152, -80, 87, 50, -80, 54, -70, -13, -61, -22, 38, 2, -114, -49, 26, 31, 110, 16, 43, 52, -72, 31, -89, -19, 36, -21, -9, 132, 56, -57, 38, 20, -49, 22, 34, -42, -80, -12, -20, -43, 21, -50, -18, 33, 44, -95, 39, -125, -140, -15, -35, 46, -70, -116, -23, -37, 9, 66, -23, -1, 29, 45, 24, -12, 50, 49, 30, 32, -147, 60, -7, -82, 37, -8, 17, 18, -13, 42, 15, -12, 94, -85, -15, 62, -23, 30, 68, 39, 70, -8, -70, 56, -38, 51, -64, -54, 82, -103, 46, 39, -49, 6, 88, 22, 45, -33, -261, -24, -106, 12, -114, -32, 58, -112, 39, -78, -28, -59, 2, 2, 45, -60, -44, 47, -27, 67, 55, 52, 28, 35, -46, 2, 53, 16, 0, 16, -38, 59, -13, 49, 66, 14, 16, 17, -46, 28, -17, 14, 3, -11, -60, 25, -42, 54, -16, -6, 9, 27, 60, 136, 2, -14, -5, 88, 121, 1, -4, -22, -47, 39, -10, 4, 12, 7, -14, -4, 29, -132, -38, 23, -36, 13, 44, -33, -39, -50, -5, 44, -95, -62, -95, 39, -9, -126, -70, -64, 9, 14, 77, 34, 100, -72, 64, 72, 49, 21, -144, 73, 30, -89, 84, 30, -13, 58, -114, -74, 88, -105, 60, -45, -127, 33, -47, 37, 73, -56, 23, 75, 5, 39, 21, 5, 32, -124, -7, -87, 55, 47, -49, -137, 1, 16, 28, -52, 7, 4, 60, -25, 10, -12, 31, 57, -50, 42, -41, -182, 25, -3, -183, -118, -57, 22, 37, -142, -125, 29, -65, 132, -73, -43, -52, -104, -86, -18, -123, -136, -47, 7, 45, 82, 30, 3, -20, -24, -88, -57, 97, 19, 42, 46, -114, 16, 19, 30, 11, 29, -7, 39, 52, 14, -3, 11, -3, 39, -26, 16, -22, 83, 87, 37, -31, 74, -68, 86, 75, -128, 74, -8, 41, 9, -41, 10, 8, -16, 123, 55, 46, 20, -64, -23, 50, 14, 64, 27, 36, -72, -60, 1, -139, 5, -36, 50, 58, 32, -43, -73, -44, 30, 46, 22, -83, 2, -18, 42, -209, -4, -148, 48, -32, -24, -8, 12, -172, 20, 6, -69, -167, -41, 1, -115, -25, -117, -2, -84, -17, -45, -12, 26, -42, -13, 3, -73, -5, 5, -33, -5, -9, -35, 5, -37, -44, -43, 14, 9, -2, 70, 4, -14, 31, 13, 0, -1, -75, -1, 7, -49}
}
;

#undef INPUT_SAMPLES
#undef FC_UNITS
/**
  ******************************************************************************
  * @file    model.hh
  * @author  Pierre-Emmanuel Novac <penovac@unice.fr>, LEAT, CNRS, Université Côte d'Azur, France
  * @version 1.0.0
  * @date    08 july 2020
  * @brief   Template generating plain C code for the implementation of Convolutional Neural Networks on MCU
  */


#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MODEL_H__
#define __MODEL_H__

#ifndef SINGLE_FILE
#include "number.h"

 // InputLayer is excluded
#include "conv2d_10.h" // InputLayer is excluded
#include "conv2d_11.h" // InputLayer is excluded
#include "flatten_5.h" // InputLayer is excluded
#include "dense_10.h"
#endif


#define MODEL_INPUT_DIM_0 28
#define MODEL_INPUT_DIM_1 28
#define MODEL_INPUT_DIM_2 1
#define MODEL_INPUT_DIMS 28 * 28 * 1

#define MODEL_OUTPUT_SAMPLES 32

#define MODEL_INPUT_SCALE_FACTOR 8 // scale factor of InputLayer
#define MODEL_INPUT_ROUND_MODE ROUND_MODE_FLOOR
#define MODEL_INPUT_NUMBER_T int16_t
#define MODEL_INPUT_LONG_NUMBER_T int32_t

#define MODEL_OUTPUT_SCALE_FACTOR 8 // scale factor of last layer
#define MODEL_OUTPUT_ROUND_MODE ROUND_MODE_FLOOR
#define MODEL_OUTPUT_NUMBER_T int16_t
#define MODEL_OUTPUT_LONG_NUMBER_T int32_t

// node 0 is InputLayer so use its output shape as input shape of the model
// typedef  input_t[28][28][1];
typedef int16_t input_t[28][28][1];
typedef dense_10_output_type output_t;


void cnn(
  const input_t input,
  output_t output);

void reset(void);

#endif//__MODEL_H__


#ifdef __cplusplus
} // extern "C"
#endif
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
