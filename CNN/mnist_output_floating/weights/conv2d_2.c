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


const float conv2d_2_bias[CONV_FILTERS] = {0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0}
;


const float conv2d_2_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{-0x1.83a4e00000000p-5}
, {0x1.e0ff900000000p-3}
, {0x1.9d95bc0000000p-3}
}
, {{0x1.9af3000000000p-4}
, {0x1.0a677c0000000p-3}
, {0x1.a695940000000p-3}
}
, {{0x1.3a86ec0000000p-3}
, {0x1.4d51e40000000p-3}
, {0x1.ed3ce00000000p-4}
}
}
, {{{0x1.4772000000000p-10}
, {-0x1.7ab6180000000p-3}
, {0x1.a1ca900000000p-3}
}
, {{-0x1.92f2c80000000p-3}
, {0x1.3db8ac0000000p-3}
, {-0x1.4e3bd00000000p-5}
}
, {{-0x1.3c16e80000000p-5}
, {0x1.8e7af00000000p-5}
, {-0x1.7211000000000p-6}
}
}
, {{{0x1.6332340000000p-3}
, {-0x1.8403b00000000p-6}
, {0x1.f635580000000p-3}
}
, {{-0x1.eebc180000000p-5}
, {-0x1.7980880000000p-4}
, {0x1.9b75780000000p-3}
}
, {{0x1.3f8b380000000p-4}
, {0x1.3384900000000p-5}
, {-0x1.3e32980000000p-5}
}
}
, {{{0x1.cf228c0000000p-3}
, {-0x1.50489c0000000p-3}
, {-0x1.e7a3980000000p-3}
}
, {{-0x1.1683980000000p-2}
, {-0x1.36ad940000000p-3}
, {0x1.e50bf80000000p-3}
}
, {{-0x1.62c4b80000000p-3}
, {-0x1.c7f7940000000p-3}
, {0x1.c9ea200000000p-4}
}
}
, {{{-0x1.bd6eb00000000p-3}
, {0x1.60e3d80000000p-4}
, {-0x1.00f7180000000p-2}
}
, {{-0x1.6db5000000000p-3}
, {-0x1.c838400000000p-7}
, {-0x1.29bd980000000p-5}
}
, {{0x1.4edc6c0000000p-3}
, {-0x1.137b3a0000000p-2}
, {-0x1.eb2a080000000p-3}
}
}
, {{{-0x1.f7871c0000000p-3}
, {-0x1.0cabe00000000p-2}
, {0x1.f858480000000p-4}
}
, {{0x1.a4fb4c0000000p-3}
, {0x1.b4e8500000000p-5}
, {-0x1.c76ea60000000p-3}
}
, {{0x1.15ee700000000p-3}
, {0x1.eb45a00000000p-3}
, {-0x1.ec8a9e0000000p-3}
}
}
, {{{-0x1.0cc4480000000p-2}
, {0x1.0333480000000p-3}
, {0x1.7143200000000p-4}
}
, {{0x1.0385500000000p-4}
, {-0x1.1467440000000p-2}
, {-0x1.2f1a900000000p-3}
}
, {{-0x1.d681f40000000p-3}
, {0x1.8b8b5c0000000p-3}
, {-0x1.0e873e0000000p-2}
}
}
, {{{0x1.a4fc000000000p-8}
, {-0x1.0ccc0e0000000p-3}
, {-0x1.cbc6000000000p-3}
}
, {{0x1.5f86c80000000p-3}
, {-0x1.ea9c200000000p-3}
, {0x1.8070280000000p-4}
}
, {{-0x1.8ca42a0000000p-3}
, {-0x1.2605a80000000p-3}
, {0x1.fb32080000000p-3}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS