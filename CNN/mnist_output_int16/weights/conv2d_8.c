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


const int16_t conv2d_8_bias[CONV_FILTERS] = {-6, -7, -29, -1, -4, -3, -20, -13}
;


const int16_t conv2d_8_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{-174}
, {-101}
, {99}
}
, {{23}
, {13}
, {39}
}
, {{87}
, {116}
, {17}
}
}
, {{{-52}
, {-58}
, {-99}
}
, {{63}
, {135}
, {171}
}
, {{63}
, {42}
, {94}
}
}
, {{{-72}
, {96}
, {177}
}
, {{-152}
, {164}
, {216}
}
, {{-211}
, {-1}
, {93}
}
}
, {{{175}
, {126}
, {67}
}
, {{-8}
, {125}
, {156}
}
, {{-240}
, {-270}
, {-149}
}
}
, {{{212}
, {21}
, {-51}
}
, {{117}
, {-10}
, {-140}
}
, {{73}
, {-141}
, {-206}
}
}
, {{{149}
, {140}
, {-4}
}
, {{-113}
, {-27}
, {-104}
}
, {{-156}
, {-41}
, {118}
}
}
, {{{53}
, {32}
, {-65}
}
, {{165}
, {231}
, {80}
}
, {{17}
, {103}
, {58}
}
}
, {{{-74}
, {151}
, {-27}
}
, {{120}
, {258}
, {-116}
}
, {{9}
, {34}
, {-185}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS