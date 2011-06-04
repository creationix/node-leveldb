#include "DB.h"
#include "WriteBatch.h"

#include <node_buffer.h>
#include "helpers.h"

using namespace node_leveldb;

Persistent<FunctionTemplate> DB::persistent_function_template;

DB::DB() {
  this->db = NULL;
}

DB::~DB() {
  if (this->db) {
    delete this->db;
    this->db = NULL;
  }
}

void DB::Init(Handle<Object> target) {
  HandleScope scope; // used by v8 for garbage collection

  // Our constructor
  Local<FunctionTemplate> local_function_template = FunctionTemplate::New(New);
  persistent_function_template = Persistent<FunctionTemplate>::New(local_function_template);
  persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1);
  persistent_function_template->SetClassName(String::NewSymbol("DB"));

  // Instance methods
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "open", Open);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "close", Close);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "put", Put);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "del", Del);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "write", Write);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "get", Get);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "newIterator", NewIterator);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "getSnapshot", GetSnapshot);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "releaseSnapshot", ReleaseSnapshot);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "getProperty", GetProperty);
  NODE_SET_PROTOTYPE_METHOD(persistent_function_template, "getApproximateSizes", GetApproximateSizes);

  // Static methods
  NODE_SET_METHOD(persistent_function_template, "destroyDB", DestroyDB);
  NODE_SET_METHOD(persistent_function_template, "repairDB", RepairDB);

  // Binding our constructor function to the target variable
  target->Set(String::NewSymbol("DB"), persistent_function_template->GetFunction());
}

Handle<Value> DB::New(const Arguments& args) {
  HandleScope scope;

  DB* db = new DB();
  db->Wrap(args.This());

  return args.This();
}

Handle<Value> DB::Open(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 2 && args[0]->IsObject() && args[1]->IsString())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, String)")));
  }

  // Get this and arguments
  DB* self = ObjectWrap::Unwrap<DB>(args.This());

  if (self->db) {
    delete self->db;
    self->db = NULL;
  }

  leveldb::Options options = JsToOptions(args[0]);
  String::Utf8Value name(args[1]);

  // Do actual work
  return processStatus(leveldb::DB::Open(options, *name, &(self->db)));
}

Handle<Value> DB::Close(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 0)) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected ()")));
  }

  DB* self = ObjectWrap::Unwrap<DB>(args.This());

  delete self->db;
  self->db = NULL;

  return Undefined();
}

Handle<Value> DB::DestroyDB(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (String, Object)")));
  }

  String::Utf8Value name(args[0]);
  leveldb::Options options = JsToOptions(args[1]);

  return processStatus(leveldb::DestroyDB(*name, options));
}

Handle<Value> DB::RepairDB(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (String, Object)")));
  }

  String::Utf8Value name(args[0]);
  leveldb::Options options = JsToOptions(args[1]);

  return processStatus(leveldb::RepairDB(*name, options));
}

Handle<Value> DB::Put(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 3 && args[0]->IsObject() && Buffer::HasInstance(args[1]) && Buffer::HasInstance(args[2]))) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, Buffer, Buffer)")));
  }

  DB* self = ObjectWrap::Unwrap<DB>(args.This());
  leveldb::WriteOptions options = JsToWriteOptions(args[0]);
  leveldb::Slice key = JsToSlice(args[1]);
  leveldb::Slice value = JsToSlice(args[2]);

  return processStatus(self->db->Put(options, key, value));
}

Handle<Value> DB::Del(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 2 && args[0]->IsObject() && Buffer::HasInstance(args[1]))) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, Buffer)")));
  }

  DB* self = ObjectWrap::Unwrap<DB>(args.This());
  leveldb::WriteOptions options = JsToWriteOptions(args[0]);
  leveldb::Slice key = JsToSlice(args[1]);

  return processStatus(self->db->Delete(options, key));
}

Handle<Value> DB::Get(const Arguments& args) {
  HandleScope scope;

  // Check args
  if (!(args.Length() == 2 && args[0]->IsObject() && Buffer::HasInstance(args[1]))) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, Buffer)")));
  }

  DB* self = ObjectWrap::Unwrap<DB>(args.This());
  leveldb::ReadOptions options = JsToReadOptions(args[0]);
  leveldb::Slice key = JsToSlice(args[1]);

  // Read value from database
  std::string value;
  leveldb::Status status = self->db->Get(options, key, &value);
  if (!status.ok()) return ThrowException(Exception::Error(String::New(status.ToString().c_str())));

  // Convert string to real JS Buffer
  int length = value.length();
  Buffer *slowBuffer = Buffer::New(length);
  memcpy(BufferData(slowBuffer), value.c_str(), length);
  Local<Function> bufferConstructor = Local<Function>::Cast(Context::GetCurrent()->Global()->Get(String::New("Buffer")));
  Handle<Value> constructorArgs[3] = { slowBuffer->handle_, Integer::New(length), Integer::New(0) };
  Local<Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

  return scope.Close(actualBuffer);
}

Handle<Value> DB::Write(const Arguments& args) {
  HandleScope scope;

  if (!(args.Length() == 2 && args[0]->IsObject() && args[1]->IsObject())) {
    return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, WriteBatch)")));
  }

  DB* self = ObjectWrap::Unwrap<DB>(args.This());
  leveldb::WriteOptions options = JsToWriteOptions(args[0]);
  Local<Object> wbObject = Object::Cast(*args[1]);

  WriteBatch* wb = ObjectWrap::Unwrap<WriteBatch>(wbObject);

  return processStatus(self->db->Write(options, wb->wb));
}

Handle<Value> DB::NewIterator(const Arguments& args) {
  HandleScope scope;
  return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
}

Handle<Value> DB::GetSnapshot(const Arguments& args) {
  HandleScope scope;
  return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
}

Handle<Value> DB::ReleaseSnapshot(const Arguments& args) {
  HandleScope scope;
  return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
}

Handle<Value> DB::GetProperty(const Arguments& args) {
  HandleScope scope;
  return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
}

Handle<Value> DB::GetApproximateSizes(const Arguments& args) {
  HandleScope scope;
  return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
}

