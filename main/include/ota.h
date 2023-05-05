#ifndef __OTA_H__
#define __OTA_H__

#include "common.h"

#define ESP_OK_HASH 0x100

extern const uint8_t server_cert_pem_start[] asm("_binary_github_cer_start");
extern const uint8_t hash[] asm("_binary_hash_start");

#endif
