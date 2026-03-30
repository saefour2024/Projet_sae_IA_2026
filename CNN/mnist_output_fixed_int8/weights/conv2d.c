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


const int8_t conv2d_bias[CONV_FILTERS] = {-3, -3, -1, -1, -1, -1, -1, -2}
;


const int8_t conv2d_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{8}
, {-21}
, {-27}
}
, {{21}
, {0}
, {-33}
}
, {{26}
, {21}
, {1}
}
}
, {{{-17}
, {16}
, {-17}
}
, {{18}
, {18}
, {23}
}
, {{-8}
, {18}
, {-16}
}
}
, {{{-16}
, {-37}
, {-23}
}
, {{-14}
, {1}
, {-15}
}
, {{28}
, {18}
, {18}
}
}
, {{{-12}
, {-10}
, {-3}
}
, {{21}
, {26}
, {9}
}
, {{-2}
, {-6}
, {5}
}
}
, {{{19}
, {22}
, {-10}
}
, {{13}
, {10}
, {-34}
}
, {{-5}
, {-29}
, {-31}
}
}
, {{{6}
, {22}
, {31}
}
, {{-21}
, {9}
, {-1}
}
, {{-54}
, {-35}
, {-34}
}
}
, {{{18}
, {10}
, {7}
}
, {{7}
, {8}
, {2}
}
, {{-14}
, {-3}
, {-9}
}
}
, {{{-20}
, {0}
, {-2}
}
, {{10}
, {18}
, {16}
}
, {{-24}
, {3}
, {10}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS