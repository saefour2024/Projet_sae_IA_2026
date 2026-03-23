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


const float conv2d_9_bias[CONV_FILTERS] = {0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0}
;


const float conv2d_9_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{-0x1.38504c0000000p-3}
, {0x1.0a82280000000p-3}
, {0x1.486fa00000000p-3}
}
, {{-0x1.22f2300000000p-4}
, {-0x1.4333680000000p-3}
, {0x1.d1bf880000000p-3}
}
, {{0x1.0a83ec0000000p-2}
, {0x1.a303580000000p-3}
, {0x1.7c353c0000000p-3}
}
}
, {{{-0x1.38a4c00000000p-7}
, {-0x1.cbbeb80000000p-3}
, {0x1.c55e8c0000000p-3}
}
, {{0x1.6aac100000000p-3}
, {-0x1.19e2a80000000p-5}
, {0x1.3657f80000000p-4}
}
, {{-0x1.d638c80000000p-5}
, {-0x1.423e440000000p-4}
, {0x1.165a2c0000000p-2}
}
}
, {{{-0x1.2d9bc00000000p-7}
, {0x1.0f96480000000p-3}
, {-0x1.4ee5100000000p-3}
}
, {{-0x1.09a4ea0000000p-2}
, {-0x1.257b080000000p-5}
, {0x1.d2701c0000000p-3}
}
, {{0x1.af05680000000p-4}
, {0x1.d3a0b00000000p-3}
, {-0x1.640b000000000p-9}
}
}
, {{{0x1.8ca8480000000p-3}
, {0x1.c925d00000000p-3}
, {0x1.54d0b80000000p-4}
}
, {{0x1.85b5800000000p-7}
, {0x1.5467e00000000p-5}
, {-0x1.cb0c980000000p-4}
}
, {{-0x1.42b0c60000000p-3}
, {-0x1.dac0e20000000p-3}
, {-0x1.e3c36c0000000p-4}
}
}
, {{{-0x1.9965d00000000p-3}
, {-0x1.10a2560000000p-3}
, {-0x1.12702e0000000p-2}
}
, {{0x1.e587400000000p-5}
, {-0x1.95ab540000000p-3}
, {-0x1.38fca80000000p-4}
}
, {{0x1.5ea7580000000p-4}
, {0x1.b42d980000000p-3}
, {-0x1.b325040000000p-3}
}
}
, {{{-0x1.b2e6e80000000p-5}
, {0x1.09f6b00000000p-5}
, {0x1.c5149c0000000p-3}
}
, {{0x1.3979200000000p-6}
, {0x1.22a1a00000000p-6}
, {0x1.69ff8c0000000p-3}
}
, {{-0x1.f16f180000000p-3}
, {0x1.e651700000000p-4}
, {-0x1.84b0540000000p-4}
}
}
, {{{-0x1.809afc0000000p-3}
, {0x1.3789600000000p-3}
, {-0x1.4c8ce00000000p-3}
}
, {{-0x1.ce217e0000000p-3}
, {-0x1.01011c0000000p-2}
, {0x1.d8aa880000000p-3}
}
, {{0x1.c30d540000000p-3}
, {0x1.a43d900000000p-5}
, {0x1.d9f4800000000p-4}
}
}
, {{{-0x1.d2d9a40000000p-3}
, {-0x1.06e90a0000000p-2}
, {-0x1.153c1e0000000p-2}
}
, {{0x1.eb1f980000000p-3}
, {0x1.c51ffc0000000p-3}
, {0x1.5b272c0000000p-3}
}
, {{-0x1.1184b40000000p-4}
, {0x1.f34f000000000p-8}
, {-0x1.1238a00000000p-5}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS