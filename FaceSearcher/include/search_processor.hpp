#pragma once

#include <mutex>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>
#include <condition_variable>

#include "face_searcher.hpp"

TG_EXPORT void Start(const SearchConfig& config);

TG_EXPORT void Stop();