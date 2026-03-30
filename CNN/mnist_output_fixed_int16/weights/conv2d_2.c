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


const int16_t conv2d_2_bias[CONV_FILTERS] = {-27, -8, 4, 166, -14, -23, -23, 7}
;


const int16_t conv2d_2_kernel[CONV_FILTERS][CONV_KERNEL_SIZE_Y][CONV_KERNEL_SIZE_X][INPUT_CHANNELS / CONV_GROUPS] = {{{{25}
, {-171}
, {24}
}
, {{30}
, {137}
, {268}
}
, {{-61}
, {107}
, {-50}
}
}
, {{{-43}
, {-27}
, {-181}
}
, {{221}
, {485}
, {-144}
}
, {{368}
, {361}
, {-173}
}
}
, {{{-40}
, {-76}
, {-182}
}
, {{329}
, {234}
, {229}
}
, {{385}
, {215}
, {-76}
}
}
, {{{-496}
, {-731}
, {-195}
}
, {{-184}
, {2}
, {314}
}
, {{234}
, {390}
, {394}
}
}
, {{{95}
, {300}
, {133}
}
, {{111}
, {398}
, {187}
}
, {{190}
, {3}
, {-349}
}
}
, {{{-340}
, {-40}
, {-120}
}
, {{-132}
, {398}
, {192}
}
, {{229}
, {430}
, {254}
}
}
, {{{-74}
, {387}
, {241}
}
, {{211}
, {333}
, {3}
}
, {{-405}
, {-162}
, {119}
}
}
, {{{-62}
, {85}
, {345}
}
, {{-71}
, {152}
, {496}
}
, {{-226}
, {-165}
, {27}
}
}
}
;

#undef INPUT_CHANNELS
#undef CONV_FILTERS
#undef CONV_KERNEL_SIZE_X
#undef CONV_KERNEL_SIZE_Y
#undef CONV_GROUPS