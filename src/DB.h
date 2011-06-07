#ifndef DB_H_
#define DB_H_

#include <v8.h>
#include <node.h>

#include "leveldb/db.h"

using namespace v8;
using namespace node;

namespace node_leveldb {

class DB : ObjectWrap {
public:
  DB();
  virtual ~DB();

  static void Init(Handle<Object> target);

private:
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


  struct Params {
    Params(DB* self, Handle<Function> cb);
    virtual ~Params();
    virtual void Callback();

    DB* self;
    Persistent<Function> callback;
    leveldb::Status status;
  };

  struct OpenParams : Params {
    OpenParams(DB *self, std::string name, leveldb::Options options, Handle<Function> callback)
      : Params(self, callback), name(name), options(options) {}

    std::string name;
    leveldb::Options options;
  };

  static void EIO_BeforeOpen(OpenParams *params);
  static int EIO_Open(eio_req *req);
  static int EIO_AfterOpen(eio_req *req);
  
  leveldb::DB* db;
  static Persistent<FunctionTemplate> persistent_function_template;
};

} // node_leveldb

#endif  // DB_H_
