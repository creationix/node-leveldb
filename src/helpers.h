#ifndef HELPERS_H_
#define HELPERS_H_

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "leveldb/db.h"

using namespace node;
using namespace v8;

namespace node_leveldb {

  // Helper to convert vanilla JS objects into leveldb objects
  leveldb::Options JsToOptions(Local<Value> val);
  leveldb::ReadOptions JsToReadOptions(Local<Value> val);
  leveldb::WriteOptions JsToWriteOptions(Local<Value> val);
  leveldb::Slice JsToSlice(Local<Value> val);

  // Helper to convert a leveldb::Status instance to a V8 return value
  Handle<Value> processStatus(leveldb::Status status);

  // Helpers to work with buffers
  char* BufferData(Buffer *b);
  size_t BufferLength(Buffer *b);
  char* BufferData(Local<Object> buf_obj);
  size_t BufferLength(Local<Object> buf_obj);

} // node_leveldb

#endif  // HELPERS_H_
