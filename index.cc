#include "src/DB.cc"
#include "src/WriteBatch.cc"
#include "src/Iterator.cc"

using namespace node_leveldb;

v8::Persistent<FunctionTemplate> DB::persistent_function_template;
v8::Persistent<FunctionTemplate> WriteBatch::persistent_function_template;
v8::Persistent<FunctionTemplate> Iterator::persistent_function_template;

extern "C" {
  static void init(Handle<Object> target) {
    DB::Init(target);
    WriteBatch::Init(target);
    Iterator::Init(target);
  }
  NODE_MODULE(leveldb, init);
}

