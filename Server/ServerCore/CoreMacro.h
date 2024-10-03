#pragma once

#define OUT

#define USE_LOCKS(InCount) std::mutex Locks[InCount];
#define USE_LOCK USE_LOCKS(1)
#define WRITE_LOCKS(InIndex) std::lock_guard<std::mutex> LockGuard##InIndex(Locks[InIndex]);
#define WRITE_LOCK WRITE_LOCKS(0)
