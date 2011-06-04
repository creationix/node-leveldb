#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <v8.h>
#include <node.h>

using namespace v8;
using namespace node;

namespace node_leveldb {

class Iterator : ObjectWrap {
  public:
    Iterator();
    ~Iterator();

    static Persistent<FunctionTemplate> persistent_function_template;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments& args);
};

} // node_leveldb

#endif ITERATOR_H_
