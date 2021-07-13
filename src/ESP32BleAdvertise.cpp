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

#include "ESP32BleAdvertise.h"
#include "esp32-hal-log.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"

#define MAX_MANUFACTURER_DATA_SIZE 20
#define MAX_SERVICE_DATA_SIZE 11

esp_ble_adv_data_t adv_data; // data that will be advertised
byte dataBuffer[50];
byte dataBuffer2[50];


// Standard parameters
static esp_ble_adv_data_t _adv_config = {
        .set_scan_rsp        = false,
        .include_name        = true,
        .include_txpower     = true,
        .min_interval        = 512,
        .max_interval        = 1024,
        .appearance          = 0,
        .manufacturer_len    = 0,
        .p_manufacturer_data = NULL,
        .service_data_len    = 0,
        .p_service_data      = NULL,
        .service_uuid_len    = 0,
        .p_service_uuid      = NULL,
        .flag                = (ESP_BLE_ADV_FLAG_NON_LIMIT_DISC|ESP_BLE_ADV_FLAG_BREDR_NOT_SPT)
};



// 
static esp_ble_adv_params_t _adv_params = {
        .adv_int_min         = 512,
        .adv_int_max         = 1024,
        .adv_type            = ADV_TYPE_NONCONN_IND,         // Excelent description of this parameter here: https://www.esp32.com/viewtopic.php?t=2267
        .own_addr_type       = BLE_ADDR_TYPE_PUBLIC,
        .peer_addr           = {0x00, },
        .peer_addr_type      = BLE_ADDR_TYPE_PUBLIC,
        .channel_map         = ADV_CHNL_ALL,
        .adv_filter_policy   = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static void _on_gap(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param){
    if(event == ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT){
        esp_ble_gap_start_advertising(&_adv_params);
    }
}

static bool _init_gap(const char * name, esp_ble_adv_data_t* adv_data){
    if(!btStarted() && !btStart()){
        log_e("btStart failed");
        return false;
    }
    esp_bluedroid_status_t bt_state = esp_bluedroid_get_status();
    if(bt_state == ESP_BLUEDROID_STATUS_UNINITIALIZED){
        if (esp_bluedroid_init()) {
            log_e("esp_bluedroid_init failed");
            return false;
        }
    }
    if(bt_state != ESP_BLUEDROID_STATUS_ENABLED){
        if (esp_bluedroid_enable()) {
            log_e("esp_bluedroid_enable failed");
            return false;
        }
    }
    if(esp_ble_gap_set_device_name(name)){
        log_e("gap_set_device_name failed");
        return false;
    }
    if(esp_ble_gap_config_adv_data(adv_data)){
        log_e("gap_config_adv_data failed");
        return false;
    }
    if(esp_ble_gap_register_callback(_on_gap)){
        log_e("gap_register_callback failed");
        return false;
    }
    return true;
}

static bool _stop_gap()
{
    if(btStarted()){
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        btStop();
    }
    return true;
}

/*
 * BLE Arduino
 *
 * */


SimpleBLE::SimpleBLE()
{
    local_name = "esp32";
    adv_data = {
        .set_scan_rsp        = false,
        .include_name        = true,
        .include_txpower     = true,
        .min_interval        = 512,
        .max_interval        = 1024,
        .appearance          = 0,
        .manufacturer_len    = 0,
        .p_manufacturer_data = NULL,  //manufacturer data is what we will use to broadcast our info
        .service_data_len    = 0,
        .p_service_data      = NULL,
        .service_uuid_len    = 0,
        .p_service_uuid      = NULL,
        .flag                = (ESP_BLE_ADV_FLAG_NON_LIMIT_DISC)
    };
}

SimpleBLE::~SimpleBLE(void)
{
    clearAdvertiseData();
    _stop_gap();
}

bool SimpleBLE::begin(String localName)
{
    if(localName.length()){
        local_name = localName;
    }
    return _init_gap(local_name.c_str(), &_adv_config);
}

void SimpleBLE::end()
{
    _stop_gap();
}

bool SimpleBLE::advertise(String data) {
    data.getBytes(dataBuffer, data.length()+1);
    return advertise(dataBuffer, data.length());
}

bool SimpleBLE::advertise(byte* data, int size) {
    clearAdvertiseData();
    fillManufacturerData(data, size);
    return _init_gap(local_name.c_str(), &adv_data);
}

bool SimpleBLE::serviceAdvertise(String data) {
    data.getBytes(dataBuffer, data.length()+1);
    return serviceAdvertise(dataBuffer, data.length());
}

bool SimpleBLE::serviceAdvertise(byte* data, int size) {
    clearAdvertiseData();
    fillServiceData(data, size);
    return _init_gap(local_name.c_str(), &adv_data);
}

/* Advertising both Manufacturer Data and Service Data was not possible, I will continue testing and improving 
bool SimpleBLE::advertise(String data_man, String data_ser) {
    data_man.getBytes(dataBuffer, data_man.length()+1);
    data_ser.getBytes(dataBuffer2, data_ser.length()+1);
    return advertise(dataBuffer, data_man.length(), dataBuffer2, data_ser.length());
}

bool SimpleBLE::advertise(byte* data_man, int size_man, byte* data_ser, int size_ser) {
    clearAdvertiseData();
    fillManufacturerData(data_man, size_man);
    fillServiceData(data_ser, size_ser);
    return _init_gap(local_name.c_str(), &adv_data);
} */

void SimpleBLE::clearAdvertiseData() {
    if(adv_data.p_manufacturer_data != NULL) {
        free(adv_data.p_manufacturer_data);
        adv_data.p_manufacturer_data = NULL;
        adv_data.manufacturer_len = 0;
    }
    if(adv_data.p_service_data != NULL) {
        free(adv_data.p_service_data);
        adv_data.p_service_data = NULL;
        adv_data.service_data_len = 0;
    }
}

void SimpleBLE::fillManufacturerData(byte* data, int size) {
    if(size > MAX_MANUFACTURER_DATA_SIZE)
        size = MAX_MANUFACTURER_DATA_SIZE;
    adv_data.p_manufacturer_data = (uint8_t *) malloc(size*sizeof(uint8_t));
    adv_data.manufacturer_len = size;
    memcpy(adv_data.p_manufacturer_data, data, size);
}

void SimpleBLE::fillServiceData(byte* data, int size) {
    if(size > MAX_SERVICE_DATA_SIZE)
        size = MAX_SERVICE_DATA_SIZE;
    adv_data.p_service_data = (uint8_t *) malloc(size*sizeof(uint8_t));
    adv_data.service_data_len = size;
    memcpy(adv_data.p_service_data, data, size);
}

