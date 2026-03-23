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


const float conv2d_14_bias[CONV_FILTERS] = {0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0, 0x0.0p+0}
;


const float conv2d_14_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{-0x1.08c86e0000000p-2}
, {0x1.c57c480000000p-4}
, {0x1.7423300000000p-4}
}
, {{0x1.8b18b00000000p-4}
, {0x1.57cc340000000p-3}
, {0x1.1f31280000000p-4}
}
, {{0x1.a256400000000p-3}
, {0x1.82fd800000000p-5}
, {-0x1.16a82c0000000p-2}
}
}
, {{{0x1.4f01940000000p-3}
, {-0x1.a726000000000p-3}
, {0x1.5d6b880000000p-3}
}
, {{0x1.d353680000000p-3}
, {-0x1.388dd00000000p-5}
, {0x1.383e440000000p-3}
}
, {{0x1.d1b49c0000000p-3}
, {0x1.2de8580000000p-4}
, {0x1.7241980000000p-4}
}
}
, {{{-0x1.d964c80000000p-3}
, {0x1.c054800000000p-5}
, {0x1.3513a80000000p-3}
}
, {{0x1.7302480000000p-4}
, {-0x1.944b400000000p-4}
, {-0x1.84cab00000000p-6}
}
, {{0x1.5340e80000000p-4}
, {-0x1.c2d8140000000p-3}
, {0x1.7e2c800000000p-4}
}
}
, {{{0x1.177d100000000p-5}
, {0x1.d310000000000p-9}
, {-0x1.25bd200000000p-3}
}
, {{0x1.e234400000000p-3}
, {-0x1.1020dc0000000p-4}
, {0x1.563ae80000000p-3}
}
, {{-0x1.c426220000000p-3}
, {-0x1.8d56000000000p-8}
, {-0x1.b72d080000000p-3}
}
}
, {{{-0x1.e642a20000000p-3}
, {0x1.e563580000000p-3}
, {0x1.03caa00000000p-2}
}
, {{0x1.d48fb80000000p-4}
, {0x1.cf27f00000000p-5}
, {0x1.057a080000000p-2}
}
, {{-0x1.d89e3c0000000p-4}
, {-0x1.571b800000000p-8}
, {-0x1.279b760000000p-3}
}
}
, {{{0x1.c4cf240000000p-3}
, {-0x1.8a71a20000000p-3}
, {0x1.bfa59c0000000p-3}
}
, {{-0x1.3500c80000000p-4}
, {-0x1.7634a00000000p-4}
, {-0x1.7e59e00000000p-5}
}
, {{-0x1.908d760000000p-3}
, {0x1.3799e00000000p-4}
, {-0x1.fc14980000000p-4}
}
}
, {{{0x1.8690f80000000p-4}
, {-0x1.26ac7e0000000p-3}
, {0x1.c808900000000p-5}
}
, {{-0x1.231f4c0000000p-3}
, {0x1.76a5380000000p-4}
, {0x1.166c200000000p-4}
}
, {{0x1.5883600000000p-3}
, {-0x1.9410e00000000p-6}
, {0x1.cca9800000000p-8}
}
}
, {{{0x1.9955d80000000p-4}
, {-0x1.6446e40000000p-4}
, {0x1.9496d80000000p-4}
}
, {{0x1.e8b5d00000000p-3}
, {0x1.5008200000000p-4}
, {0x1.931b100000000p-3}
}
, {{-0x1.c3b1b60000000p-3}
, {0x1.e1e2000000000p-8}
, {0x1.10d3c40000000p-2}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS