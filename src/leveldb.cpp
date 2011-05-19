#include <v8.h>
#include <node.h>

#include "writebatch.h"
#include "iterator.h"

using namespace v8;

v8::Persistent<FunctionTemplate> WriteBatch::persistent_function_template;
extern "C" {
  static void init(Handle<Object> target) {
    WriteBatch::Init(target);
  }
  NODE_MODULE(writebatch, init);
}

