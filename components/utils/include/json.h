/**
 * @file json.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-14
 * 
 * @copyright Copyright (c) 2021
 * 
 * TODO: Make struct to save configs received from server 
 * 
 */

#ifndef __JSON_H__
#define __JSON_H__

#include "stdint.h"
#include "cJSON.h"
#include "esp_log.h"
#include "system_defines.h"
#include "string.h"



int8_t json_load_config(const char *jsonstring, system_defs *sys);

/**   */ 
char *json_create_config_string(system_defs *sys, uint8_t with_samplerates);

#endif /* __JSON_H__ */