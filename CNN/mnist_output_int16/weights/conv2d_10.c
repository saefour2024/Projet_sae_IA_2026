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