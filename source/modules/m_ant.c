#include <stdint.h>
#include "nrf_error.h"
#include "app_error.h"
#include "m_ant.h"

// Local Defines

#define THINGY_ANT_CHANNEL_TYPE  PARAMETER_TX_NOT_RX

#define THINGY_ANT_DEVICE_NUMBER (uint16_t)(NRF_FICR->DEVICEID[0]) // Based on internal Device ID
#define THINGY_ANT_DEVICE_TYPE   42
#define THINGY_ANT_TX_TYPE       1
#define THINGY_ANT_PERIOD        8192 // 4Hz
#define THINGY_ANT_RF_FREQ       78


// ANT Page Data

static uint8_t temperature_sign = 0; /** 0 if positive, 1 if negative */
static uint16_t temperature__deg_c_mag_times_100 = 0; /** Magnitude of temperature times 100 */
static uint16_t battery_mv = 0; /** Voltage */
static uint8_t humidity_sign = 0; /** 0 if positive, 1 if negative */
static uint16_t humidity_percent_mag = 0; /** Magnitude of humidity */

static uint8_t tx_buffer[ANT_STANDARD_DATA_PAYLOAD_SIZE] = { 0 }; /** TX buffer passed to SD to send ANT message */
static volatile ant_page_number_t page = 0; /** Next page number to send */

static ant_request_sensor_data_handler_t request_sensor_data_functions[NUM_PAGE_TYPES]; /** Functions used to request fresh data for ANT module from sensor modules */

// Local Function Declarations

/**@brief Sends the next data page over ANT. */
static uint32_t m_ant_send();

/**@brief Goes to the next page in the rotation. */
static void m_ant_change_page();

// Public Funtion Implementations

uint32_t m_ant_init(ant_request_sensor_data_handler_t* p_request_sensor_data_handlers)
{
    uint32_t err_code;

    request_sensor_data_functions[THINGY_ANT_PAGE_BATTERY] = p_request_sensor_data_handlers[THINGY_ANT_PAGE_BATTERY];
    request_sensor_data_functions[THINGY_ANT_PAGE_TEMP] = p_request_sensor_data_handlers[THINGY_ANT_PAGE_TEMP];
    request_sensor_data_functions[THINGY_ANT_PAGE_HUMIDITY] = p_request_sensor_data_handlers[THINGY_ANT_PAGE_HUMIDITY];

    err_code = sd_ant_channel_assign(THINGY_ANT_CHANNEL_NUMBER, THINGY_ANT_CHANNEL_TYPE, THINGY_ANT_NETWORK_NUMBER, 0);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ant_channel_id_set(0, THINGY_ANT_DEVICE_NUMBER, THINGY_ANT_DEVICE_TYPE, THINGY_ANT_TX_TYPE);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ant_channel_period_set(0, THINGY_ANT_PERIOD); // 4 HZ
    APP_ERROR_CHECK(err_code);

    err_code = sd_ant_channel_radio_freq_set(THINGY_ANT_CHANNEL_NUMBER, THINGY_ANT_RF_FREQ);
    APP_ERROR_CHECK(err_code);

    return NRF_SUCCESS;
}

uint32_t m_ant_open()
{
   uint32_t err_code;

   err_code = sd_ant_channel_open(THINGY_ANT_CHANNEL_NUMBER);
   APP_ERROR_CHECK(err_code);

   return NRF_SUCCESS;
}

uint32_t m_ant_close()
{
   uint32_t err_code;

   err_code = sd_ant_channel_close(THINGY_ANT_CHANNEL_NUMBER);
   APP_ERROR_CHECK(err_code);

   return NRF_SUCCESS;
}

void m_ant_evt_handler(ant_evt_t* p_ant_evt)
{
      switch (p_ant_evt->event)
      {
      case EVENT_TX: // A message was just sent
         m_ant_send(); // Send the next message
         m_ant_change_page();
         break;
      default:
         break;
      }
}

void m_ant_update_temp(float temp_deg_c)
{
   if(temp_deg_c >= 0)
   {
      temperature_sign =  0x00;
      temperature__deg_c_mag_times_100 = (uint16_t)(temp_deg_c * 100.0f);
   }
   else
   {
      temperature_sign =  0x01;
      temperature__deg_c_mag_times_100 = (uint16_t)(temp_deg_c * -100.0f);
   }

}

void m_ant_update_humidity(int16_t humid)
{
      if(humid >= 0)
      {
         humidity_sign =  0x00;
         humidity_percent_mag = (uint16_t)(humid);
      }
      else
      {
         humidity_sign =  0x01;
         humidity_percent_mag = (uint16_t)(humid * -1);
      }
}

void m_ant_update_battery(uint16_t voltage)
{
   battery_mv = voltage;
}

// Local Function Implementations

static void m_ant_change_page()
{
    page = (page + 1) % NUM_PAGE_TYPES;
    request_sensor_data_functions[page](page);
}

static uint32_t m_ant_send()
{
      tx_buffer[0] = (uint8_t)page;

      switch(page)
      {
      case THINGY_ANT_PAGE_BATTERY:
         tx_buffer[1] = (uint8_t)battery_mv;
         tx_buffer[2] = (uint8_t)(battery_mv>>8);
         break;

      case THINGY_ANT_PAGE_TEMP:
       tx_buffer[1] = temperature_sign;
       tx_buffer[2] = (uint8_t)temperature__deg_c_mag_times_100;
       tx_buffer[3] = (uint8_t)(temperature__deg_c_mag_times_100 >> 8);
         break;

      case THINGY_ANT_PAGE_HUMIDITY:
        tx_buffer[1] = humidity_sign;
        tx_buffer[2] = (uint8_t)humidity_percent_mag;
        tx_buffer[3] = (uint8_t)(humidity_percent_mag >> 8);
        break;

      default: // Not valid data
         tx_buffer[0] = 0xFF;
         tx_buffer[1] = 0xFF;
         tx_buffer[2] = 0xFF;
         tx_buffer[3] = 0xFF;
         tx_buffer[4] = 0xFF;
         tx_buffer[5] = 0xFF;
         tx_buffer[6] = 0xFF;
         tx_buffer[7] = 0xFF;
         break;
      }

      sd_ant_broadcast_message_tx(THINGY_ANT_CHANNEL_NUMBER, ANT_STANDARD_DATA_PAYLOAD_SIZE, tx_buffer);

      return NRF_SUCCESS;
}
