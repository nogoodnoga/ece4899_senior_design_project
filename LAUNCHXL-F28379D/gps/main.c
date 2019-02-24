/***************************************************************************
| Project           : gps
|
| Program name      : main.c
|
| Author            : qrogers
|
| Date created      : 31 DEC 2018
|
| Purpose           : GPS Module - Read logitude and latitude
|
| Revision History  :
|
| Date          Author      Ref     Revision
| 31 DEC 2018   qrogers      1      Initial Release
|
***************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "device.h"
#include "driverlib.h"
#include "tinygps.h"

void main(void)
{
   uint16_t recieved_buff[255];
   uint16_t received_char;
   uint32_t gps_age;
   float    gps_latitude, gps_longitude;
   float    dest_latitude  =  39.7392;
   float    dest_longitude = -104.9903;

   Device_init();

   Device_initGPIO();

   // GPIO19 is the SCI RX pin. (Pin 3)
   GPIO_setMasterCore(19, GPIO_CORE_CPU1);
   GPIO_setPinConfig(GPIO_19_SCIRXDB);
   GPIO_setDirectionMode(19, GPIO_DIR_MODE_IN);
   GPIO_setPadConfig(19, GPIO_PIN_TYPE_STD);
   GPIO_setQualificationMode(19, GPIO_QUAL_ASYNC);

   // GPIO18 is the SCI TX pin. (Pin 4)
   GPIO_setMasterCore(18, GPIO_CORE_CPU1);
   GPIO_setPinConfig(GPIO_18_SCITXDB);
   GPIO_setDirectionMode(18, GPIO_DIR_MODE_OUT);
   GPIO_setPadConfig(18, GPIO_PIN_TYPE_STD);
   GPIO_setQualificationMode(18, GPIO_QUAL_ASYNC);

   Interrupt_initModule();
   Interrupt_initVectorTable();

   SCI_performSoftwareReset(SCIB_BASE);

   SCI_setConfig(SCIB_BASE, DEVICE_LSPCLK_FREQ, 9600, (SCI_CONFIG_WLEN_8   |
                                                       SCI_CONFIG_STOP_ONE |
                                                       SCI_CONFIG_PAR_NONE));
   SCI_resetChannels(SCIB_BASE);
   SCI_resetRxFIFO(SCIB_BASE);
   SCI_resetTxFIFO(SCIB_BASE);
   SCI_clearInterruptStatus(SCIB_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
   SCI_enableFIFO(SCIB_BASE);
   SCI_enableModule(SCIB_BASE);
   SCI_performSoftwareReset(SCIB_BASE);

   while(1)
   {
     SCI_readCharArray(SCIB_BASE, recieved_buff, 255);

      for (received_char = 0; received_char < 255; received_char++)
      {
         if(gps_encode((char)recieved_buff[received_char]))
         {
            gps_f_get_position(&gps_latitude, &gps_longitude, &gps_age);

            float distance = gps_distance_between(gps_latitude,  gps_longitude,
                                                  dest_latitude, dest_longitude);

            float bearing = gps_course_to(gps_latitude,  gps_longitude,
                                          dest_latitude, dest_longitude);

         }
      }
   }
}
