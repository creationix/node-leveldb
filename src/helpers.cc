#include "helpers.h"

namespace node_leveldb {

leveldb::Options JsToOptions(Handle<Value> val) {
  HandleScope scope;
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

leveldb::ReadOptions JsToReadOptions(Handle<Value> val) {
  HandleScope scope;
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

leveldb::WriteOptions JsToWriteOptions(Handle<Value> val) {
  HandleScope scope;
  leveldb::WriteOptions options;
  Local<Object> obj = Object::Cast(*val);
  if (obj->Has(String::New("sync"))) {
    options.sync = obj->Get(String::New("sync"))->BooleanValue();
  }
  return options;
}

leveldb::Slice JsToSlice(Handle<Value> val, std::vector<std::string> *strings) {
  HandleScope scope;
  if (val->IsString() && strings != NULL) {
    std::string str(*String::Utf8Value(val));
    strings->push_back(str);
    return leveldb::Slice(str.data(), str.length());
  }
  else if (Buffer::HasInstance(val)) {
    Local<Object> obj = Object::Cast(*val);
    return leveldb::Slice(BufferData(obj), BufferLength(obj));
  }
  else {
    return leveldb::Slice();
  }
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

char* BufferData(Handle<Object> buf_obj) {
  return Buffer::Data(buf_obj);
}

size_t BufferLength(Handle<Object> buf_obj) {
  return Buffer::Length(buf_obj);
}

} // node_leveldb
