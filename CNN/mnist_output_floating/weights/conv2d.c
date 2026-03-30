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


const float conv2d_bias[CONV_FILTERS] = {-0x1.29df2c0000000p-5, -0x1.25744e0000000p-5, -0x1.16cc960000000p-7, -0x1.eb797e0000000p-8, -0x1.b8f04e0000000p-9, -0x1.c465080000000p-7, -0x1.de62340000000p-8, -0x1.eab5b00000000p-6}
;


const float conv2d_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{0x1.1f56120000000p-3}
, {-0x1.462bb40000000p-2}
, {-0x1.a558f20000000p-2}
}
, {{0x1.583dc00000000p-2}
, {0x1.71a6640000000p-7}
, {-0x1.0396cc0000000p-1}
}
, {{0x1.a331480000000p-2}
, {0x1.5547c40000000p-2}
, {0x1.1277320000000p-6}
}
}
, {{{-0x1.02693a0000000p-2}
, {0x1.0ad7300000000p-2}
, {-0x1.0a2fd60000000p-2}
}
, {{0x1.23ab940000000p-2}
, {0x1.2142580000000p-2}
, {0x1.7877740000000p-2}
}
, {{-0x1.da413e0000000p-4}
, {0x1.24d6920000000p-2}
, {-0x1.e0a4f20000000p-3}
}
}
, {{{-0x1.ef94520000000p-3}
, {-0x1.2676440000000p-1}
, {-0x1.63d58e0000000p-2}
}
, {{-0x1.b3df460000000p-3}
, {0x1.fc9b7e0000000p-6}
, {-0x1.c63c120000000p-3}
}
, {{0x1.c60e440000000p-2}
, {0x1.2307ae0000000p-2}
, {0x1.2823ac0000000p-2}
}
}
, {{{-0x1.6632880000000p-3}
, {-0x1.2599060000000p-3}
, {-0x1.63f08a0000000p-5}
}
, {{0x1.57db760000000p-2}
, {0x1.afaa240000000p-2}
, {0x1.370f800000000p-3}
}
, {{-0x1.3682300000000p-6}
, {-0x1.7849ac0000000p-4}
, {0x1.41b4c80000000p-4}
}
}
, {{{0x1.36b9d60000000p-2}
, {0x1.6827600000000p-2}
, {-0x1.20d9f40000000p-3}
}
, {{0x1.b7cb640000000p-3}
, {0x1.5d2b5a0000000p-3}
, {-0x1.0dfc8e0000000p-1}
}
, {{-0x1.306b9a0000000p-4}
, {-0x1.c62bb00000000p-2}
, {-0x1.ebd46e0000000p-2}
}
}
, {{{0x1.8c343a0000000p-4}
, {0x1.657ff40000000p-2}
, {0x1.f2443c0000000p-2}
}
, {{-0x1.49251e0000000p-2}
, {0x1.2960c20000000p-3}
, {-0x1.0264440000000p-7}
}
, {{-0x1.abb9780000000p-1}
, {-0x1.17f72a0000000p-1}
, {-0x1.0c1d800000000p-1}
}
}
, {{{0x1.2706ea0000000p-2}
, {0x1.52a9d00000000p-3}
, {0x1.e05bb80000000p-4}
}
, {{0x1.dd49400000000p-4}
, {0x1.16daf00000000p-3}
, {0x1.4611a20000000p-5}
}
, {{-0x1.ba10f80000000p-3}
, {-0x1.5cde760000000p-5}
, {-0x1.0d9e560000000p-3}
}
}
, {{{-0x1.336a140000000p-2}
, {0x1.3ed3360000000p-7}
, {-0x1.0560c80000000p-6}
}
, {{0x1.5358ec0000000p-3}
, {0x1.296d8c0000000p-2}
, {0x1.0fd4e20000000p-2}
}
, {{-0x1.71b3380000000p-2}
, {0x1.a1845c0000000p-5}
, {0x1.54cffe0000000p-3}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS