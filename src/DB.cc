#include "DB.h"
#include "WriteBatch.h"

#include <node_buffer.h>
#include "helpers.h"

using namespace node_leveldb;

Persistent<FunctionTemplate> DB::persistent_function_template;

DB::DB() 
  : db(NULL)
{
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

  DB* self = new DB();
  self->Wrap(args.This());

  return args.This();
}

Handle<Value> DB::Open(const Arguments& args) {
  HandleScope scope;

  // Get this and arguments
  DB* self = ObjectWrap::Unwrap<DB>(args.This());

  // Required filename
  if (args.Length() < 1 || !args[0]->IsString()) {
    return ThrowException(Exception::TypeError(String::New("DB.open() expects a filename")));
  }
  String::Utf8Value name(args[0]);
  
  int pos = 1;

  // Optional options
  leveldb::Options options = leveldb::Options();
  if (pos < args.Length() && args[pos]->IsObject() && !args[pos]->IsFunction()) {
    options = JsToOptions(args[pos]);
    pos++;
  }

  // Optional callback
  Local<Function> callback;
  if (pos < args.Length() && args[pos]->IsFunction()) {
    callback = Local<Function>::Cast(args[pos]);
    pos++;
  }

  // Pass parameters to async function
  OpenParams *params = new OpenParams(self, *name, options, callback);
  EIO_BeforeOpen(params);

  return args.This();
}

void DB::EIO_BeforeOpen(OpenParams *params) {
  eio_custom(EIO_Open, EIO_PRI_DEFAULT, EIO_AfterOpen, params);
}

int DB::EIO_Open(eio_req *req) {
  OpenParams *params = static_cast<OpenParams*>(req->data);
  DB *self = params->self;

  // Close old DB, if open() is called more than once
  if (self->db) {
    delete self->db;
    self->db = NULL;
  }

  // Do the actual work
  params->status = leveldb::DB::Open(params->options, params->name, &self->db);

  return 0;
}

int DB::EIO_AfterOpen(eio_req *req) {
  OpenParams *params = static_cast<OpenParams*>(req->data);
  params->Callback();
  
  delete params;
  return 0;
}

Handle<Value> DB::Close(const Arguments& args) {
  HandleScope scope;

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
  if (self->db == NULL) {
    return ThrowException(Exception::Error(String::New("DB has not been opened")));
  }
  
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
  if (self->db == NULL) {
    return ThrowException(Exception::Error(String::New("DB has not been opened")));
  }
  
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
  if (self->db == NULL) {
    return ThrowException(Exception::Error(String::New("DB has not been opened")));
  }
  
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
  if (self->db == NULL) {
    return ThrowException(Exception::Error(String::New("DB has not been opened")));
  }
  
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


//
// Implementation of Params, which are passed from JS thread to EIO thread and back again
//

DB::Params::Params(DB* self, Handle<Function> cb)
  : self(self)
{
  self->Ref();
  ev_ref(EV_DEFAULT_UC);
  callback = Persistent<Function>::New(cb);
}

DB::Params::~Params() {
  self->Unref();
  ev_unref(EV_DEFAULT_UC);
  callback.Dispose();
}

void DB::Params::Callback() {
  if (!callback.IsEmpty()) {
    TryCatch try_catch;
    if (status.ok()) {
      // no error, callback with no arguments
      callback->Call(self->handle_, 0, NULL);
    } else {
      // error, callback with first argument as error object
      Local<String> result = String::New(status.ToString().c_str());
      Local<Value> argv[] = { Exception::Error(result) };
      callback->Call(self->handle_, 1, argv);
    }
    if (try_catch.HasCaught()) {
        FatalException(try_catch);
    }
  }
}


