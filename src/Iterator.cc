#include "Iterator.h"
#include "helpers.h"
#include "iostream"

using namespace node_leveldb;

Persistent<FunctionTemplate> Iterator::persistent_function_template;

Iterator::Iterator() {
  this->it = NULL;
}

Iterator::~Iterator() {
    // cleanup the iterator instance
    if (this->it) {
      delete this->it;
      this->it = NULL;
    } // if
}

void Iterator::Init(Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> local_function_template = FunctionTemplate::New(New);
  persistent_function_template = Persistent<FunctionTemplate>::New(local_function_template);
  persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1);
  persistent_function_template->SetClassName(String::NewSymbol("Iterator"));
  

  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "valid", Valid);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "seekToFirst", SeekToFirst);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "seekToLast", SeekToLast);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "seek", Seek);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "next", Next);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "prev", Prev);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "key", key);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "value", value);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "status", status);

  target->Set(String::NewSymbol("Iterator"), persistent_function_template->GetFunction());
}

Handle<Value> Iterator::New(const Arguments& args) {
  HandleScope scope;
  
  Iterator* iterator = new Iterator();
  
  if (args.Length() >= 1 && args[0]->IsExternal()) {
      iterator->it = (leveldb::Iterator*) Local<External>::Cast(args[0])->Value();
  } // if
  
  iterator->Wrap(args.This());
  
  return args.This();
}

Handle<Value> Iterator::Valid(const Arguments& args) {
    HandleScope scope;

    Iterator* self = ObjectWrap::Unwrap<Iterator>(args.This());
    
    return self->it->Valid() ? True() : False();
}

Handle<Value> Iterator::SeekToFirst(const Arguments& args) {
    ObjectWrap::Unwrap<Iterator>(args.This())->it->SeekToFirst();
    
    return Undefined();
}

Handle<Value> Iterator::SeekToLast(const Arguments& args) {
    ObjectWrap::Unwrap<Iterator>(args.This())->it->SeekToLast();
    
    return Undefined();
}

Handle<Value> Iterator::Seek(const Arguments& args) {
    HandleScope scope;

    std::vector<std::string> strings;

    leveldb::Slice key = JsToSlice(args[0], &strings);
    ObjectWrap::Unwrap<Iterator>(args.This())->it->Seek(key);

    return Undefined();
}

Handle<Value> Iterator::Next(const Arguments& args) {
    ObjectWrap::Unwrap<Iterator>(args.This())->it->Next();
    
    return Undefined();
}

Handle<Value> Iterator::Prev(const Arguments& args) {
    ObjectWrap::Unwrap<Iterator>(args.This())->it->Prev();
    
    return Undefined();
}

Handle<Value> Iterator::key(const Arguments& args) {
    HandleScope scope;
    leveldb::Slice k = ObjectWrap::Unwrap<Iterator>(args.This())->it->key();
    
    return scope.Close(Bufferize(k.ToString()));
}

Handle<Value> Iterator::value(const Arguments& args) {
    HandleScope scope;
    leveldb::Slice v = ObjectWrap::Unwrap<Iterator>(args.This())->it->value();
    
    return scope.Close(Bufferize(v.ToString()));
}

Handle<Value> Iterator::status(const Arguments& args) {
    leveldb::Status status = ObjectWrap::Unwrap<Iterator>(args.This())->it->status();
    
    return processStatus(status);
}

