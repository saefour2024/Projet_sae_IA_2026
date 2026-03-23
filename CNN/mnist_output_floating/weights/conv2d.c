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


const float conv2d_bias[CONV_FILTERS] = {0x1.0769760000000p-6, -0x1.570ee00000000p-6, 0x1.ed85f60000000p-6, -0x1.85f8620000000p-8, -0x1.f2c0900000000p-7, -0x1.cacc2c0000000p-7, -0x1.16fab80000000p-3, 0x1.77921c0000000p-6}
;


const float conv2d_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{-0x1.20fa860000000p-1}
, {0x1.836cbc0000000p-4}
, {0x1.8b098e0000000p-3}
}
, {{-0x1.a8fab60000000p-2}
, {0x1.1a71d40000000p-4}
, {0x1.6daf540000000p-2}
}
, {{-0x1.ae9c340000000p-2}
, {-0x1.9e09980000000p-7}
, {0x1.bad0b80000000p-3}
}
}
, {{{-0x1.3b68580000000p-5}
, {0x1.3763b40000000p-7}
, {-0x1.802e2a0000000p-4}
}
, {{0x1.1e856a0000000p-2}
, {0x1.bdf00c0000000p-3}
, {0x1.5e59680000000p-3}
}
, {{-0x1.07e3340000000p-6}
, {0x1.13fcfe0000000p-2}
, {-0x1.e80ba00000000p-2}
}
}
, {{{0x1.bf7bba0000000p-4}
, {0x1.a5de660000000p-2}
, {0x1.151d540000000p-1}
}
, {{-0x1.1709620000000p-1}
, {-0x1.091b520000000p-4}
, {-0x1.22f9800000000p-3}
}
, {{-0x1.5bf1620000000p-1}
, {-0x1.d42f120000000p-2}
, {-0x1.b4f1020000000p-2}
}
}
, {{{-0x1.44a7c20000000p-3}
, {-0x1.99f0540000000p-5}
, {-0x1.7c23a00000000p-3}
}
, {{0x1.ed977c0000000p-3}
, {0x1.617f3c0000000p-2}
, {0x1.293c8c0000000p-2}
}
, {{0x1.1e6ed40000000p-4}
, {-0x1.3923a40000000p-4}
, {-0x1.08f28a0000000p-11}
}
}
, {{{-0x1.0ddede0000000p-4}
, {-0x1.0137520000000p-1}
, {-0x1.4b72c00000000p-2}
}
, {{0x1.01a4940000000p-4}
, {-0x1.31f5380000000p-5}
, {-0x1.842b500000000p-2}
}
, {{0x1.64f3d60000000p-2}
, {0x1.d0ba620000000p-2}
, {0x1.da8d8c0000000p-4}
}
}
, {{{-0x1.f24b940000000p-2}
, {-0x1.632dd60000000p-2}
, {-0x1.ccde100000000p-4}
}
, {{-0x1.f4dfcc0000000p-2}
, {0x1.cfcd9c0000000p-4}
, {0x1.0b115c0000000p-2}
}
, {{-0x1.0814120000000p-4}
, {0x1.afa9940000000p-3}
, {0x1.ff31080000000p-3}
}
}
, {{{-0x1.55a6f20000000p-4}
, {-0x1.161f3e0000000p-8}
, {0x1.30787e0000000p-6}
}
, {{0x1.7a6cc60000000p-3}
, {0x1.0cb1500000000p-1}
, {0x1.17e7a00000000p-4}
}
, {{-0x1.6c87ce0000000p-2}
, {0x1.2ed41a0000000p-3}
, {0x1.9241440000000p-4}
}
}
, {{{0x1.b9d2260000000p-3}
, {0x1.e4aee20000000p-3}
, {0x1.376fda0000000p-2}
}
, {{0x1.831b9e0000000p-7}
, {0x1.2e34f00000000p-2}
, {0x1.02595a0000000p-2}
}
, {{-0x1.676bd40000000p-1}
, {-0x1.29504e0000000p-1}
, {-0x1.4d69ac0000000p-1}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS