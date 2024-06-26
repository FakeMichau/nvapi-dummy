#pragma once
#include <iostream>
#include <chrono>
#include <cstdint>
#include <string>
#include <source_location>
#include "../include/nvapi.h"

void log_time();
void log(std::string log);
void prepareLogging(std::string fileName);
void closeLogging();
NvAPI_Status Ok(const std::source_location& location = std::source_location::current());
NvAPI_Status Error(const std::source_location& location = std::source_location::current());
NvAPI_Status Error(NvAPI_Status status, const std::source_location& location = std::source_location::current());
