#include "Com_tool.h"

/**
 * @brief Apply max/min limit to the value 
 * 
 * @return int16_t Limited value
 */
int16_t Com_limit(int16_t value, int16_t min, int16_t max)
{
    int16_t result = value; 
    if (value > max) 
    {
        result = max;
    }
    else if (value < min)
    {
        result = min;
    }
    return result;
}
