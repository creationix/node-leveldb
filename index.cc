
#include "src/LevelDB.cc"
#include "src/WriteBatch.cc"
#include "src/Iterator.cc"

extern "C" {
  static void init(Handle<Object> target) {
    LevelDB::Init(target);
    WriteBatch::Init(target);
    Iterator::Init(target);
  }
  NODE_MODULE(leveldb, init);
}

