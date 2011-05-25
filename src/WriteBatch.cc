#include "leveldb/write_batch.h"

#include "helpers.h"

namespace node_leveldb {

  WriteBatch::WriteBatch() {
    this->wb = new leveldb::WriteBatch();
  }

  WriteBatch::~WriteBatch() {
    delete this->wb;
  }

  void WriteBatch::Init(Handle<Object> target) {
    v8::HandleScope scope; // used by v8 for garbage collection

    // Our constructor
    v8::Local<FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
    WriteBatch::persistent_function_template = v8::Persistent<FunctionTemplate>::New(local_function_template);
    WriteBatch::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
    WriteBatch::persistent_function_template->SetClassName(v8::String::NewSymbol("WriteBatch"));

    // Instance methods
    NODE_SET_PROTOTYPE_METHOD(WriteBatch::persistent_function_template, "put", Put);
    NODE_SET_PROTOTYPE_METHOD(WriteBatch::persistent_function_template, "del", Del);
    NODE_SET_PROTOTYPE_METHOD(WriteBatch::persistent_function_template, "clear", Clear);

    // Binding our constructor function to the target variable
    target->Set(String::NewSymbol("WriteBatch"), WriteBatch::persistent_function_template->GetFunction());
  }

  // This is our constructor function. It instantiate a C++ WriteBatch object and returns a Javascript handle to this object.
  Handle<Value> WriteBatch::New(const Arguments& args) {
    HandleScope scope;

    WriteBatch* WriteBatch_instance = new WriteBatch();
      
    // Wrap our C++ object as a Javascript object
    WriteBatch_instance->Wrap(args.This());
      
    // Our constructor function returns a Javascript object which is a wrapper for our C++ object, 
    // This is the expected behavior when calling a constructor function with the new operator in Javascript.
    return args.This();
  }

  Handle<Value> WriteBatch::Put(const Arguments& args) {
    HandleScope scope;

    // Check args
    if (!(args.Length() == 2 && Buffer::HasInstance(args[0]) && Buffer::HasInstance(args[1]))) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Buffer, Buffer)")));
    }

    WriteBatch* self = ObjectWrap::Unwrap<WriteBatch>(args.This());
    leveldb::Slice key = JsToSlice(args[0]);
    leveldb::Slice value = JsToSlice(args[1]);

    self->wb->Put(key, value);

    return Undefined();
  }

  Handle<Value> WriteBatch::Del(const Arguments& args) {
    HandleScope scope;

    // Check args
    if (!(args.Length() == 1 && Buffer::HasInstance(args[0]))) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Buffer)")));
    }

    WriteBatch* self = ObjectWrap::Unwrap<WriteBatch>(args.This());
    leveldb::Slice key = JsToSlice(args[0]);

    self->wb->Delete(key);

    return Undefined();
  }

  Handle<Value> WriteBatch::Clear(const Arguments& args) {
    HandleScope scope;

    if (!(args.Length() == 0)) {
      return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected none")));
    }

    WriteBatch* self = ObjectWrap::Unwrap<WriteBatch>(args.This());

    self->wb->Clear();

    return Undefined();
  }

}
