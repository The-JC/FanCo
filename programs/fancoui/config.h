#ifndef CONFIG_H
#define CONFIG_H
#pragma once

#include <string>

#define FANS 18
#define PLOTLEN 120

const uint8_t addresses[FANS] = { 0x04, 0x05, 0x06, 0x07 };

#define GPIOHOST "localhost";
#define GPIOPORT "8888";

#endif // CONFIG_H
