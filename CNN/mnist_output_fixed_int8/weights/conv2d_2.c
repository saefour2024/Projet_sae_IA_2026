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


const int8_t conv2d_2_bias[CONV_FILTERS] = {-4, -1, 0, 20, -2, -3, -3, 0}
;


const int8_t conv2d_2_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{3}
, {-22}
, {3}
}
, {{3}
, {17}
, {33}
}
, {{-8}
, {13}
, {-7}
}
}
, {{{-6}
, {-4}
, {-23}
}
, {{27}
, {60}
, {-18}
}
, {{46}
, {45}
, {-22}
}
}
, {{{-5}
, {-10}
, {-23}
}
, {{41}
, {29}
, {28}
}
, {{48}
, {26}
, {-10}
}
}
, {{{-62}
, {-92}
, {-25}
}
, {{-23}
, {0}
, {39}
}
, {{29}
, {48}
, {49}
}
}
, {{{11}
, {37}
, {16}
}
, {{13}
, {49}
, {23}
}
, {{23}
, {0}
, {-44}
}
}
, {{{-43}
, {-5}
, {-15}
}
, {{-17}
, {49}
, {24}
}
, {{28}
, {53}
, {31}
}
}
, {{{-10}
, {48}
, {30}
}
, {{26}
, {41}
, {0}
}
, {{-51}
, {-21}
, {14}
}
}
, {{{-8}
, {10}
, {43}
}
, {{-9}
, {19}
, {62}
}
, {{-29}
, {-21}
, {3}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS