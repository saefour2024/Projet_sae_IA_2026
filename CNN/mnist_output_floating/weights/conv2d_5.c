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


const float conv2d_5_bias[CONV_FILTERS] = {0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0}
;


const float conv2d_5_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{-0x1.fb97900000000p-3}
, {-0x1.b786000000000p-9}
, {0x1.17b4600000000p-3}
}
, {{-0x1.58e3740000000p-3}
, {0x1.d515180000000p-3}
, {0x1.0e36040000000p-2}
}
, {{-0x1.c5a3880000000p-4}
, {-0x1.4bb0600000000p-6}
, {-0x1.569d4a0000000p-3}
}
}
, {{{0x1.12c6240000000p-2}
, {0x1.9a46b80000000p-3}
, {-0x1.08de600000000p-4}
}
, {{-0x1.5a7d9c0000000p-4}
, {0x1.c150080000000p-4}
, {0x1.2e8cc00000000p-5}
}
, {{-0x1.3860380000000p-3}
, {-0x1.45e9e80000000p-3}
, {-0x1.0ac0fc0000000p-2}
}
}
, {{{0x1.44c8a00000000p-3}
, {-0x1.037e100000000p-3}
, {-0x1.fba3fc0000000p-3}
}
, {{-0x1.e102820000000p-3}
, {-0x1.50b1540000000p-3}
, {-0x1.34de2e0000000p-3}
}
, {{-0x1.5b50300000000p-4}
, {-0x1.0a61f80000000p-2}
, {-0x1.1b41e00000000p-5}
}
}
, {{{0x1.0a02840000000p-2}
, {0x1.018e740000000p-2}
, {-0x1.33fd000000000p-5}
}
, {{-0x1.4171dc0000000p-4}
, {0x1.9556500000000p-3}
, {0x1.714b180000000p-4}
}
, {{0x1.59d8d00000000p-5}
, {0x1.00e8780000000p-2}
, {-0x1.48dc600000000p-6}
}
}
, {{{0x1.0436d00000000p-5}
, {0x1.2b6a740000000p-3}
, {0x1.1165b80000000p-3}
}
, {{0x1.5bf0900000000p-4}
, {-0x1.4418f40000000p-3}
, {-0x1.d6bdc00000000p-6}
}
, {{-0x1.d1961a0000000p-3}
, {-0x1.dd7ef80000000p-3}
, {0x1.5f55700000000p-5}
}
}
, {{{-0x1.d9afaa0000000p-3}
, {-0x1.cd67fc0000000p-4}
, {-0x1.9dd4000000000p-9}
}
, {{-0x1.d3702e0000000p-3}
, {-0x1.dd52100000000p-3}
, {0x1.ea3ba80000000p-3}
}
, {{0x1.12f4600000000p-5}
, {-0x1.de23520000000p-3}
, {0x1.29410c0000000p-3}
}
}
, {{{-0x1.9675a80000000p-5}
, {-0x1.73a8500000000p-4}
, {-0x1.dbcdc00000000p-6}
}
, {{0x1.24c4280000000p-3}
, {0x1.71bf640000000p-3}
, {-0x1.57f5c00000000p-7}
}
, {{-0x1.31a0c80000000p-5}
, {-0x1.26c4800000000p-8}
, {-0x1.a267c40000000p-4}
}
}
, {{{0x1.7e2af80000000p-3}
, {0x1.ae7e800000000p-8}
, {-0x1.2edf000000000p-9}
}
, {{-0x1.36bda00000000p-4}
, {-0x1.cfae580000000p-3}
, {-0x1.731ab80000000p-4}
}
, {{0x1.12a4cc0000000p-2}
, {-0x1.e2c8580000000p-3}
, {-0x1.3248c80000000p-3}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS