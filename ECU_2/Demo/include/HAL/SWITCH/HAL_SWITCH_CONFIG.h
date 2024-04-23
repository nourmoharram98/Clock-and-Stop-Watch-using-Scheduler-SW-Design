/**
 * @file HAL_SWITCH_CONFIG.h
 * @author Nour Moharram
 * @brief carry the enumeration and any additional configurations that not need to be compiled for 
 * the switches in the system
 * @version 0.1
 * @date 2024-02-17
 * 
 */

#ifndef HAL_SWITCH_SWITCH_CONFIG_H_
#define HAL_SWITCH_SWITCH_CONFIG_H_

/**
 * @brief enumeration for number of switches
 *
 */
typedef enum
{
	SWITCH_EDIT,
	SWITCH_UP,
	SWITCH_DOWN,
	SWITCH_LEFT,
	SWITCH_RIGHT,
	SWITCH_MODE,    /**< SWITCH_NUMONE */
	SWITCH_OK,    /**< SWITCH_NUMTWO */
	Number_Of_Switches/**< Number_Of_Switches */
}switch_IDs_t;



#endif /* HAL_SWITCH_SWITCH_CONFIG_H_ */
