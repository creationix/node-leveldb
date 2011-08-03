#include "Iterator.h"
#include "helpers.h"

using namespace node_leveldb;

Persistent<FunctionTemplate> Iterator::persistent_function_template;

Iterator::Iterator() {
  this->it = NULL;
}

Iterator::~Iterator() {
}

void Iterator::Init(Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> local_function_template = FunctionTemplate::New(New);
  persistent_function_template = Persistent<FunctionTemplate>::New(local_function_template);
  persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1);
  persistent_function_template->SetClassName(String::NewSymbol("Iterator"));

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
