#ifndef NODE_LEVELDB_H
#define NODE_LEVELDB_H

#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "leveldb/db.h"
#include "helpers.h"

namespace node_leveldb {

  class DB : ObjectWrap {
  private:
    leveldb::DB* db;
    static Persistent<FunctionTemplate> persistent_function_template;

  public:
    DB();
    ~DB();

    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments& args);

    static Handle<Value> Open(const Arguments& args);
    static Handle<Value> Close(const Arguments& args);

    static Handle<Value> DestroyDB(const Arguments& args);
    static Handle<Value> RepairDB(const Arguments& args);

    static Handle<Value> Put(const Arguments& args);
    static Handle<Value> Del(const Arguments& args);
    static Handle<Value> Get(const Arguments& args);
    static Handle<Value> Write(const Arguments& args);

    static Handle<Value> NewIterator(const Arguments& args);

    static Handle<Value> GetSnapshot(const Arguments& args);
    static Handle<Value> ReleaseSnapshot(const Arguments& args);

    static Handle<Value> GetProperty(const Arguments& args);
    static Handle<Value> GetApproximateSizes(const Arguments& args);
  };

  class WriteBatch : node::ObjectWrap {
  private:
    leveldb::WriteBatch* wb;
    static v8::Persistent<FunctionTemplate> persistent_function_template;
  
  public:
    WriteBatch();
    ~WriteBatch();

    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments& args);

    static Handle<Value> Put(const Arguments& args);
    static Handle<Value> Del(const Arguments& args);
    static Handle<Value> Clear(const Arguments& args);

    friend class DB;
  };

}

#endif  // NODE_LEVELDB_H
