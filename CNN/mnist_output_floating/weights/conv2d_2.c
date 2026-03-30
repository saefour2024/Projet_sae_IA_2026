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


const float conv2d_2_bias[CONV_FILTERS] = {-0x1.adb5ca0000000p-5, -0x1.fb423c0000000p-7, 0x1.2723680000000p-7, 0x1.4d8bd00000000p-2, -0x1.bb61960000000p-6, -0x1.6994fa0000000p-5, -0x1.6e05be0000000p-5, 0x1.c676180000000p-7}
;


const float conv2d_2_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{0x1.9ee8de0000000p-5}
, {-0x1.54b7f20000000p-2}
, {0x1.8b9df80000000p-5}
}
, {{0x1.eaa62c0000000p-5}
, {0x1.126afe0000000p-2}
, {0x1.0ca67c0000000p-1}
}
, {{-0x1.e5742e0000000p-4}
, {0x1.ae150c0000000p-3}
, {-0x1.8f32f80000000p-4}
}
}
, {{{-0x1.56e47a0000000p-4}
, {-0x1.aa91b40000000p-5}
, {-0x1.68b80a0000000p-2}
}
, {{0x1.bb72860000000p-2}
, {0x1.e5488e0000000p-1}
, {-0x1.1f5abe0000000p-2}
}
, {{0x1.70191a0000000p-1}
, {0x1.69d9c00000000p-1}
, {-0x1.59753e0000000p-2}
}
}
, {{{-0x1.3baa8c0000000p-4}
, {-0x1.2cf5d80000000p-3}
, {-0x1.6b3e000000000p-2}
}
, {{0x1.49a1d80000000p-1}
, {0x1.d4cd680000000p-2}
, {0x1.cb74d20000000p-2}
}
, {{0x1.81dfb40000000p-1}
, {0x1.afb19a0000000p-2}
, {-0x1.2e77d60000000p-3}
}
}
, {{{-0x1.ef303a0000000p-1}
, {-0x1.6d263c0000000p+0}
, {-0x1.84d8600000000p-2}
}
, {{-0x1.6e30cc0000000p-2}
, {0x1.58261e0000000p-8}
, {0x1.3a74e20000000p-1}
}
, {{0x1.d5dbdc0000000p-2}
, {0x1.86df720000000p-1}
, {0x1.8a7f720000000p-1}
}
}
, {{{0x1.7caca60000000p-3}
, {0x1.2c48c20000000p-1}
, {0x1.0af36c0000000p-2}
}
, {{0x1.bef6440000000p-3}
, {0x1.8e1e340000000p-1}
, {0x1.771e260000000p-2}
}
, {{0x1.7c1d740000000p-2}
, {0x1.f700260000000p-8}
, {-0x1.5c97240000000p-1}
}
}
, {{{-0x1.53b9d40000000p-1}
, {-0x1.3d22720000000p-4}
, {-0x1.ddb8a80000000p-3}
}
, {{-0x1.0796f80000000p-2}
, {0x1.8ec0f60000000p-1}
, {0x1.8169580000000p-2}
}
, {{0x1.ca52f00000000p-2}
, {0x1.ae96a00000000p-1}
, {0x1.fd273c0000000p-2}
}
}
, {{{-0x1.27f9a60000000p-3}
, {0x1.838dc60000000p-1}
, {0x1.e2a4be0000000p-2}
}
, {{0x1.a703720000000p-2}
, {0x1.4d93a60000000p-1}
, {0x1.964a320000000p-8}
}
, {{-0x1.942f940000000p-1}
, {-0x1.4208220000000p-2}
, {0x1.dc2dce0000000p-3}
}
}
, {{{-0x1.ee75a40000000p-4}
, {0x1.5622720000000p-3}
, {0x1.59b9be0000000p-1}
}
, {{-0x1.1aa79e0000000p-3}
, {0x1.30eb6a0000000p-2}
, {0x1.f089020000000p-1}
}
, {{-0x1.c212380000000p-2}
, {-0x1.49825e0000000p-2}
, {0x1.b533560000000p-5}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS