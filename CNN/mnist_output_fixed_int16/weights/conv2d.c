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


const int16_t conv2d_bias[CONV_FILTERS] = {-19, -19, -5, -4, -2, -8, -4, -16}
;


const int16_t conv2d_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{71}
, {-164}
, {-211}
}
, {{172}
, {5}
, {-260}
}
, {{209}
, {170}
, {8}
}
}
, {{{-130}
, {133}
, {-134}
}
, {{145}
, {144}
, {188}
}
, {{-60}
, {146}
, {-121}
}
}
, {{{-124}
, {-295}
, {-178}
}
, {{-109}
, {15}
, {-114}
}
, {{227}
, {145}
, {148}
}
}
, {{{-90}
, {-74}
, {-23}
}
, {{171}
, {215}
, {77}
}
, {{-10}
, {-48}
, {40}
}
}
, {{{155}
, {180}
, {-73}
}
, {{109}
, {87}
, {-270}
}
, {{-39}
, {-228}
, {-246}
}
}
, {{{49}
, {178}
, {249}
}
, {{-165}
, {74}
, {-5}
}
, {{-428}
, {-280}
, {-269}
}
}
, {{{147}
, {84}
, {60}
}
, {{59}
, {69}
, {20}
}
, {{-111}
, {-22}
, {-68}
}
}
, {{{-154}
, {4}
, {-9}
}
, {{84}
, {148}
, {135}
}
, {{-185}
, {26}
, {85}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS