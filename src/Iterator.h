#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <v8.h>
#include <node.h>

#include "leveldb/iterator.h"

using namespace v8;
using namespace node;

namespace node_leveldb {

class Iterator : ObjectWrap {
  private:
    leveldb::Iterator* it;
    
  public:
    Iterator();
    ~Iterator();

    static Persistent<FunctionTemplate> persistent_function_template;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments& args);
    
    static Handle<Value> Valid(const Arguments& args);
};

} // node_leveldb

#endif ITERATOR_H_
