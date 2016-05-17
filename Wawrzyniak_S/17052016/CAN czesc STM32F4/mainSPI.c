#include "init.h"
//#include <stm32f4_discovery.h>

// LIS302DL register map
#define LIS302DL_WHO_AM_I 0x0F

#define LIS302DL_CTRL_REG1 0x20
#define LIS302DL_CTRL_REG2 0x21
#define LIS302DL_CTRL_REG3 0x22

#define LIS302DL_STATUS_REG 0x27

#define LIS302DL_OUT_X 0x29
#define LIS302DL_OUT_Y 0x2B
#define LIS302DL_OUT_Z 0x2D

// LIS302DL bits
// communication
#define DUMMY_BYTE 0x00
#define LIS302DL_R 0x80
#define LIS302DL_W 0x00
#define LIS302DL_M 0x40
#define LIS302DL_S 0x00

// CTRL_REG1
#define LIS302DL_XEN 0x01
#define LIS302DL_YEN 0x02
#define LIS302DL_ZEN 0x04
#define LIS302DL_STM 0x08
#define LIS302DL_STP 0x10
#define LIS302DL_FS  0x20
#define LIS302DL_PD  0x40
#define LIS302DL_DR  0x80

// CTRL_REG2
#define LIS302DL_HP_COEFF1 0x01
#define LIS302DL_HP_COEFF2 0x02
#define LIS302DL_HP_FF_WU1 0x04
#define LIS302DL_HP_FF_WU2 0x08
#define LIS302DL_FDS 	   0x10
#define LIS302DL_BOOT      0x40
