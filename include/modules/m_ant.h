/* Header guard */
#ifndef THINGY_ANT_H__
#define THINGY_ANT_H__

#include "ant_parameters.h"
#include "ant_interface.h"
#include "ant_stack_handler_types.h"

/*Public definitions for the m_ant module*/
#define THINGY_ANT_NETWORK_NUMBER 0
#define THINGY_ANT_CHANNEL_NUMBER 0

#define NUM_PAGE_TYPES  3

typedef enum {
   THINGY_ANT_PAGE_BATTERY = 0,
   THINGY_ANT_PAGE_TEMP,
   THINGY_ANT_PAGE_HUMIDITY
} ant_page_number_t;

typedef void (*ant_request_sensor_data_handler_t) (ant_page_number_t page);

/*Public Interfaces into ANT module*/

/**@brief Function to initialize ANT module.
 *
 * @param[in] p_request_sensor_data_handlers Array of pointers to functions which ANT module can use to request its sensor data be updated.
 * 											 Array indexes corrospond to the ant_page_number_t enum
 *
 * @retval NRF_SUCCESS.
 */
uint32_t m_ant_init(ant_request_sensor_data_handler_t* p_request_sensor_data_handlers);

/**@brief Opens the ANT channel and starts broadcasting.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t m_ant_open();

/**@brief Closes ANT channel.
 *
 * @retval NRF_SUCCESS.
 */
uint32_t m_ant_close();

/**@brief Handler for ANT events.
 *
 * Events from ANT should be passed into this function for processing.
 *
 * @param[in] p_ant_evt The ANT event for processing.
 */
void m_ant_evt_handler(ant_evt_t* p_ant_evt);

/**@brief Sets the battery voltage value in the ANT module
 *
 * Battery level can be set in the ANT module using this function. The next time that the ANT module
 * sends the battery page, it will send this data.
 *
 * @param[in] voltage The battery volatage value in mV
 */
void m_ant_update_battery(uint16_t in_battery);

/**@brief Sets the temperature in the ANT module.
 *
 * Temperature can be set in the ANT module using this function. The next time that the ANT module
 * sends the temperature page, it will send this data.
 *
 * @param[in] temp The temperature value in Degrees C.
 */
void m_ant_update_temp(float temp_deg_c);

/**@brief Sets the humidity in the ANT module.
 *
 * Humidity can be set in the ANT module using this function. The next time that the ANT module
 * sends the humidity page, it will send this data.
 *
 * @param[in] temp The humidity value in percent.
 */
void m_ant_update_humidity(int16_t humidity_percent);


#endif //THINGY_ANT_H__
