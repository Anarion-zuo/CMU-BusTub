//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_replacer.cpp
//
// Identification: src/buffer/lru_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"
#include <algorithm>

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) {
  if (usedList.empty()) {
    *frame_id = INVALID_PAGE_ID;
    return false;
  }
  latch.lock();
  LRUEntry entry = usedList.back();
  usedList.pop_back();
  latch.unlock();
  *frame_id = entry.frameId;
  return true;
}

void LRUReplacer::Pin(frame_id_t frame_id) {
  latch.lock();
  auto frameIt = std::find(usedList.begin(), usedList.end(), LRUEntry{frame_id});
  if (frameIt == usedList.end()) {
    // frame not exists!
    latch.unlock();
    return;
  }
  // remove frameIt
  usedList.erase(frameIt);
  latch.unlock();
}

void LRUReplacer::Unpin(frame_id_t frame_id) {
  latch.lock();
  auto frameIt = std::find(usedList.begin(), usedList.end(), LRUEntry{frame_id});
  if (frameIt != usedList.end()) {
    // frame exists!
    latch.unlock();
    return;
  }
  usedList.push_front(LRUEntry{frame_id});
  latch.unlock();

}

size_t LRUReplacer::Size() { return usedList.size(); }

bool LRUReplacer::LRUEntry::operator==(const LRUReplacer::LRUEntry &rhs) const { return frameId == rhs.frameId; }
bool LRUReplacer::LRUEntry::operator!=(const LRUReplacer::LRUEntry &rhs) const { return !(rhs == *this); }
}  // namespace bustub
