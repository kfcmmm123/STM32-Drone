#ifndef __INT_VL53L1X_H__
#define __INT_VL53L1X_H__

#include "vl53l1_platform.h"
#include "VL53L1X_api.h"
#include "VL53L1X_calibration.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Initialize the VL53L1X sensor
 * 
 */
void Int_VL53L1X_Init(void);

/**
 * @brief Get the distance from VL53L1X sensor
 * 
 */
uint16_t Int_VL53L1X_Get_Distance(void);

#endif // __INT_VL53L1X_H__
