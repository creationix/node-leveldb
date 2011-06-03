#include "helpers.h"

namespace node_leveldb {

leveldb::Options JsToOptions(Local<Value> val) {
  // Copy the v8 options over an leveldb options object
  Local<Object> obj = Object::Cast(*val);
  leveldb::Options options;
  if (obj->Has(String::New("create_if_missing"))) {
    options.create_if_missing = obj->Get(String::New("create_if_missing"))->BooleanValue();
  }
  if (obj->Has(String::New("error_if_exists"))) {
    options.error_if_exists = obj->Get(String::New("error_if_exists"))->BooleanValue();
  }
  if (obj->Has(String::New("paranoid_checks"))) {
    options.paranoid_checks = obj->Get(String::New("paranoid_checks"))->BooleanValue();
  }
  if (obj->Has(String::New("write_buffer_size"))) {
    options.write_buffer_size = obj->Get(String::New("write_buffer_size"))->Int32Value();
  }
  if (obj->Has(String::New("max_open_files"))) {
    options.max_open_files = obj->Get(String::New("max_open_files"))->Int32Value();
  }
  if (obj->Has(String::New("block_size"))) {
    options.block_size = obj->Get(String::New("block_size"))->Int32Value();
  }
  if (obj->Has(String::New("block_restart_interval"))) {
    options.block_restart_interval = obj->Get(String::New("block_restart_interval"))->Int32Value();
  }
  return options;
}

leveldb::ReadOptions JsToReadOptions(Local<Value> val) {
  leveldb::ReadOptions options;
  Local<Object> obj = Object::Cast(*val);
  if (obj->Has(String::New("verify_checksums"))) {
    options.verify_checksums = obj->Get(String::New("verify_checksums"))->BooleanValue();
  }
  if (obj->Has(String::New("fill_cache"))) {
    options.fill_cache = obj->Get(String::New("fill_cache"))->BooleanValue();
  }
  return options;
}

leveldb::WriteOptions JsToWriteOptions(Local<Value> val) {
  leveldb::WriteOptions options;
  Local<Object> obj = Object::Cast(*val);
  if (obj->Has(String::New("sync"))) {
    options.sync = obj->Get(String::New("sync"))->BooleanValue();
  }
  return options;
}

leveldb::Slice JsToSlice(Local<Value> val) {
  Local<Object> obj = Object::Cast(*val);
  leveldb::Slice slice(BufferData(obj), BufferLength(obj));
  return slice;
}

Handle<Value> processStatus(leveldb::Status status) {
  if (status.ok()) return String::New(status.ToString().c_str());
  return ThrowException(Exception::Error(String::New(status.ToString().c_str())));
}


char* BufferData(Buffer *b) {
  return Buffer::Data(b->handle_);
}

size_t BufferLength(Buffer *b) {
  return Buffer::Length(b->handle_);
}

char* BufferData(Local<Object> buf_obj) {
  return Buffer::Data(buf_obj);
}

size_t BufferLength(Local<Object> buf_obj) {
  return Buffer::Length(buf_obj);
}

} // node_leveldb
