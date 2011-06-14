#include "WriteBatch.h"

#include <node_buffer.h>
#include "helpers.h"

using namespace node_leveldb;

Persistent<FunctionTemplate> WriteBatch::persistent_function_template;

WriteBatch::WriteBatch() {
}

WriteBatch::~WriteBatch() {
}

void WriteBatch::Init(Handle<Object> target) {
  HandleScope scope;

  Local<FunctionTemplate> local_function_template = FunctionTemplate::New(New);
  persistent_function_template = Persistent<FunctionTemplate>::New(local_function_template);
  persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1);
  persistent_function_template->SetClassName(String::NewSymbol("WriteBatch"));

  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "put", Put);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "del", Del);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "clear", Clear);

  target->Set(String::NewSymbol("WriteBatch"), persistent_function_template->GetFunction());
}

Handle<Value> WriteBatch::New(const Arguments& args) {
  HandleScope scope;

  WriteBatch* writeBatch = new WriteBatch();
  writeBatch->Wrap(args.This());

  return args.This();
}

Handle<Value> WriteBatch::Put(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (args.Length() < 2 || (!args[0]->IsString() && !Buffer::HasInstance(args[0])) || (!args[1]->IsString() && !Buffer::HasInstance(args[1]))) {
    return ThrowException(Exception::TypeError(String::New("WriteBatch.put() expects key, value")));
  }

  WriteBatch* self = ObjectWrap::Unwrap<WriteBatch>(args.This());
  
  leveldb::Slice key = JsToSlice(args[0], self->strings);
  leveldb::Slice value = JsToSlice(args[1], self->strings);
  
  self->wb.Put(key, value);

  return args.This();
}

Handle<Value> WriteBatch::Del(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (args.Length() < 1 || (!args[0]->IsString() && !Buffer::HasInstance(args[0]))) {
    return ThrowException(Exception::TypeError(String::New("WriteBatch.del() expects key")));
  }

  WriteBatch* self = ObjectWrap::Unwrap<WriteBatch>(args.This());
  leveldb::Slice key = JsToSlice(args[0], self->strings);

  self->wb.Delete(key);

  return args.This();
}

Handle<Value> WriteBatch::Clear(const Arguments& args) {
  HandleScope scope;

  WriteBatch* self = ObjectWrap::Unwrap<WriteBatch>(args.This());

  self->wb.Clear();
  self->strings.clear();

  return args.This();
}
