#ifndef ITERATOR_H_
#define ITERATOR_H_

#include <v8.h>
#include <node.h>

#include "leveldb/iterator.h"

using namespace v8;
using namespace node;

namespace node_leveldb {

class Iterator : ObjectWrap {
  public:
    Iterator();
    ~Iterator();

    static Persistent<FunctionTemplate> persistent_function_template;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments& args);
    
    static Handle<Value> Valid(const Arguments& args);
    static Handle<Value> SeekToFirst(const Arguments& args);
    static Handle<Value> SeekToLast(const Arguments& args);
    static Handle<Value> Seek(const Arguments& args);
    static Handle<Value> Next(const Arguments& args);
    static Handle<Value> Prev(const Arguments& args);
    
    static Handle<Value> key(const Arguments& args);
    static Handle<Value> value(const Arguments& args);
    static Handle<Value> status(const Arguments& args);

  private:
    leveldb::Iterator* it;    
    
    struct SeekParams {
       SeekParams(Iterator* it, leveldb::Slice k, Handle<Function> cb) {
          self = it;
          key = k;
          callback = Persistent<Function>::New(cb);

          self->Ref();
          ev_ref(EV_DEFAULT_UC);
       }

       ~SeekParams() {
          self->Unref();
          ev_unref(EV_DEFAULT_UC);
          callback.Dispose();
       }

       virtual void Callback(Handle<Value> result = Handle<Value>());

       Iterator* self;
       leveldb::Slice key;
       Persistent<Function> callback;
       
    };

    static void EIO_BeforeSeek(SeekParams *params);
    static int EIO_Seek(eio_req *req);

    static void EIO_BeforeSeekToLast(SeekParams *params);
    static int EIO_SeekToLast(eio_req *req);

    static void EIO_BeforeSeekToFirst(SeekParams *params);
    static int EIO_SeekToFirst(eio_req *req);

    static int EIO_AfterSeek(eio_req *req);
};

} // node_leveldb

#endif

