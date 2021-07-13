// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _SIMPLE_BLE_H_
#define _SIMPLE_BLE_H_


#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_bt.h"

#include "Arduino.h"

struct ble_gap_adv_params_s;

class SimpleBLE {
    public:

        SimpleBLE(void);
        ~SimpleBLE(void);

        /**
         * Start BLE Advertising
         *
         * @param[in] localName  local name to advertise
         *
         * @return true on success
         *
         */
        bool begin(String localName=String());

        /**
         * Advertises data on Manufacturer Data field
         *
         * @param[in] data  String with the message to be transmitted
         *
         * @return true on success
         *
         */
        bool advertise(String data);

        /**
         * Advertises data on Manufacturer Data field
         *
         * @param[in] data  byte array with the message to be transmitted
         *
         * @param[in] size  size of the byte array
         *
         * @return true on success
         *
         */
        bool advertise(byte* data, int size);

        /**
         * Advertises data on Service Data field
         *
         * @param[in] data  String with the message to be transmitted
         *
         * @return true on success
         *
         */
        bool serviceAdvertise(String data);

        /**
         * Advertises data on Service Data field
         *
         * @param[in] data  byte array with the message to be transmitted
         *
         * @param[in] size  size of the byte array
         *
         * @return true on success
         *
         */
        bool serviceAdvertise(byte* data, int size);

        //bool advertise(byte* data_man, int size_man, byte* data_ser, int size_ser);

        //bool advertise(String data_man, String data_ser); 

        /**
         * Stop BLE Advertising
         *
         * @return none
         */
        void end(void);

        private:
            void clearAdvertiseData();

            void fillManufacturerData(byte* data, int size);

            void fillServiceData(byte* data, int size);


        

    private:
        String local_name;
    private:

};

#endif
