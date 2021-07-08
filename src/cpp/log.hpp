#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/async.h"

extern std::shared_ptr<spdlog::async_logger> logger;

std::shared_ptr<spdlog::async_logger> get_logger (std::string name);

template<class... U>
void trace(U&&... u) { if(logger) logger->trace(std::forward<U>(u)...); }

template<class... U>
void debug(U&&... u) { if(logger) logger->debug(std::forward<U>(u)...); }

template<class... U>
void info(U&&... u) { if(logger) logger->info(std::forward<U>(u)...); }

template<class... U>
void warn(U&&... u) { if(logger) logger->warn(std::forward<U>(u)...); }

template<class... U>
void warning(U&&... u) { if(logger) logger->warn(std::forward<U>(u)...); }

template<class... U>
void error(U&&... u) { if(logger) logger->error(std::forward<U>(u)...); }

template<class... U>
void critical(U&&... u) { if(logger) logger->critical(std::forward<U>(u)...); }

// std::string logger_init (const std::string &name,unsigned level);
void logger_init (const std::string &config_json);
