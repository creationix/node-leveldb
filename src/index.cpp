#include <v8.h>
#include <node.h>

#include "../deps/leveldb/include/db.h"

using namespace node;
using namespace v8;

#include "LevelDB.cpp"
#include "WriteBatch.cpp"
#include "Iterator.cpp"

extern "C" {
  static void init(Handle<Object> target) {
    LevelDB::Init(target);
    WriteBatch::Init(target);
    Iterator::Init(target);
  }
  NODE_MODULE(leveldb, init);
}

