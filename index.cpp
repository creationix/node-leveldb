#include <v8.h>
#include <node.h>

#include "leveldb/db.h"

using namespace node;
using namespace v8;

#include "src/LevelDB.cpp"
#include "src/WriteBatch.cpp"
#include "src/Iterator.cpp"

extern "C" {
  static void init(Handle<Object> target) {
    LevelDB::Init(target);
    WriteBatch::Init(target);
    Iterator::Init(target);
  }
  NODE_MODULE(leveldb, init);
}

