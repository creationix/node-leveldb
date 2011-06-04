#include <node.h>

#include "DB.h"
#include "WriteBatch.h"
#include "Iterator.h"

using namespace node_leveldb;

extern "C" {
  static void init(Handle<Object> target) {
    DB::Init(target);
    WriteBatch::Init(target);
    Iterator::Init(target);
  }
  NODE_MODULE(leveldb, init);
}
