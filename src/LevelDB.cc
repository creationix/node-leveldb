#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "leveldb/db.h"

using namespace node;
using namespace v8;

class LevelDB : ObjectWrap {
  private:
    leveldb::DB* db;

    // Helper to convert a vanilla JS object into a leveldb::Options instance
    static leveldb::Options processOptions(Local<Object> opts) {
      // Copy the v8 options over an leveldb options object
      leveldb::Options options;
      if (opts->Has(String::New("create_if_missing"))) {
        options.create_if_missing = opts->Get(String::New("create_if_missing"))->BooleanValue();
      }
      if (opts->Has(String::New("error_if_exists"))) {
        options.error_if_exists = opts->Get(String::New("error_if_exists"))->BooleanValue();
      }
      if (opts->Has(String::New("paranoid_checks"))) {
        options.paranoid_checks = opts->Get(String::New("paranoid_checks"))->BooleanValue();
      }
      if (opts->Has(String::New("write_buffer_size"))) {
        options.write_buffer_size = opts->Get(String::New("write_buffer_size"))->Int32Value();
      }
      if (opts->Has(String::New("max_open_files"))) {
        options.max_open_files = opts->Get(String::New("max_open_files"))->Int32Value();
      }
      if (opts->Has(String::New("block_size"))) {
        options.block_size = opts->Get(String::New("block_size"))->Int32Value();
      }
      if (opts->Has(String::New("block_restart_interval"))) {
        options.block_restart_interval = opts->Get(String::New("block_restart_interval"))->Int32Value();
      }
      return options;
    }

    // Helper to convert a vanilla JS object into a leveldb::ReadOptions instance
    static leveldb::ReadOptions JsToReadOptions(Local<Value> val) {
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

    // Helper to convert a vanilla JS object into a leveldb::WriteOptions instance
    static leveldb::WriteOptions JsToWriteOptions(Local<Value> val) {
      leveldb::WriteOptions options;
      Local<Object> obj = Object::Cast(*val);
      if (obj->Has(String::New("sync"))) {
        options.sync = obj->Get(String::New("sync"))->BooleanValue();
      }
      return options;
    }

    // Helper to convert a leveldb::Status instance to a V8 return value
    static Handle<Value> processStatus(leveldb::Status status) {
      if (status.ok()) return String::New(status.ToString().c_str());
      return ThrowException(Exception::TypeError(String::New(status.ToString().c_str())));
    }

    static char* BufferData(Buffer *b) {
      return Buffer::Data(b->handle_);
    }

    static size_t BufferLength(Buffer *b) {
      return Buffer::Length(b->handle_);
    }

    static char* BufferData(Local<Object> buf_obj) {
      HandleScope scope;
      return Buffer::Data(buf_obj);
    }

    static size_t BufferLength(Local<Object> buf_obj) {
      HandleScope scope;
      return Buffer::Length(buf_obj);
    }

    static leveldb::Slice JsToSlice(Local<Value> val) {
      Local<Object> obj = Object::Cast(*val);
      leveldb::Slice slice(BufferData(obj), BufferLength(obj));
      return slice;
    }


  public:
    LevelDB() {}
    ~LevelDB() {}


    // Holds our constructor function
    static Persistent<FunctionTemplate> persistent_function_template;

    static void Init(Handle<Object> target) {
      HandleScope scope; // used by v8 for garbage collection

      // Our constructor
      Local<FunctionTemplate> local_function_template = FunctionTemplate::New(New);
      LevelDB::persistent_function_template = Persistent<FunctionTemplate>::New(local_function_template);
      LevelDB::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
      LevelDB::persistent_function_template->SetClassName(String::NewSymbol("LevelDB"));

      // Instance methods
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "open", Open);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "close", Close);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "put", Put);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "del", Del);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "write", Write);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "get", Get);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "newIterator", NewIterator);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "getSnapshot", GetSnapshot);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "releaseSnapshot", ReleaseSnapshot);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "getProperty", GetProperty);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "getApproximateSizes", GetApproximateSizes);

      // Static methods
      NODE_SET_METHOD(LevelDB::persistent_function_template, "destroyDB", DestroyDB);
      NODE_SET_METHOD(LevelDB::persistent_function_template, "repairDB", RepairDB);

      // Binding our constructor function to the target variable
      target->Set(String::NewSymbol("LevelDB"), LevelDB::persistent_function_template->GetFunction());
    }

    // This is our constructor function. It instantiate a C++ LevelDB object and returns a Javascript handle to this object.
    static Handle<Value> New(const Arguments& args) {
      HandleScope scope;

      LevelDB* LevelDB_instance = new LevelDB();

      // Wrap our C++ object as a Javascript object
      LevelDB_instance->Wrap(args.This());

      // Our constructor function returns a Javascript object which is a wrapper for our C++ object,
      // This is the expected behavior when calling a constructor function with the new operator in Javascript.
      return args.This();
    }

    static Handle<Value> Open(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 2 && args[0]->IsObject() && args[1]->IsString())) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, String)")));
      }

      // Get this and arguments
      LevelDB* self = ObjectWrap::Unwrap<LevelDB>(args.This());
      Local<Object> options = Object::Cast(*args[0]);
      String::Utf8Value name(args[1]);

      // Do actual work
      return processStatus(leveldb::DB::Open(processOptions(options), *name, &(self->db)));
    }

    static Handle<Value> Close(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 0)) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected ()")));
      }

      LevelDB* self = ObjectWrap::Unwrap<LevelDB>(args.This());

      delete self->db;

      return Undefined();
    }

    static Handle<Value> DestroyDB(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsObject())) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (String, Object)")));
      }

      String::Utf8Value name(args[0]);
      Local<Object> options = Object::Cast(*args[1]);

      return processStatus(leveldb::DestroyDB(*name, processOptions(options)));
    }

    static Handle<Value> RepairDB(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsObject())) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (String, Object)")));
      }

      String::Utf8Value name(args[0]);
      Local<Object> options = Object::Cast(*args[1]);

      return processStatus(leveldb::RepairDB(*name, processOptions(options)));
    }

    static Handle<Value> Put(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 3 && args[0]->IsObject() && Buffer::HasInstance(args[1]) && Buffer::HasInstance(args[2]))) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, Buffer, Buffer)")));
      }

      LevelDB* self = ObjectWrap::Unwrap<LevelDB>(args.This());
      leveldb::WriteOptions options = JsToWriteOptions(*args[0]);
      leveldb::Slice key = JsToSlice(*args[1]);
      leveldb::Slice value = JsToSlice(*args[2]);

      return processStatus(self->db->Put(options, key, value));
    }


    static Handle<Value> Del(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 2 && args[0]->IsObject() && Buffer::HasInstance(args[1]))) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, Buffer)")));
      }

      LevelDB* self = ObjectWrap::Unwrap<LevelDB>(args.This());
      leveldb::WriteOptions options = JsToWriteOptions(*args[0]);
      leveldb::Slice key = JsToSlice(*args[1]);

      return processStatus(self->db->Delete(options, key));
    }

    static Handle<Value> Get(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 2 && args[0]->IsObject() && Buffer::HasInstance(args[1]))) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (Object, Buffer)")));
      }

      LevelDB* self = ObjectWrap::Unwrap<LevelDB>(args.This());
      leveldb::ReadOptions options = JsToReadOptions(*args[0]);
      leveldb::Slice key = JsToSlice(*args[1]);

      // Read value from database
      std::string value;
      leveldb::Status status = self->db->Get(options, key, &value);
      if (!status.ok()) return ThrowException(Exception::TypeError(String::New(status.ToString().c_str())));

      // Convert string to real JS Buffer
      int length = value.length();
      Buffer *slowBuffer = Buffer::New(length);
      memcpy(BufferData(slowBuffer), value.c_str(), length);
      Local<Function> bufferConstructor = Local<Function>::Cast(Context::GetCurrent()->Global()->Get(String::New("Buffer")));
      Handle<Value> constructorArgs[3] = { slowBuffer->handle_, Integer::New(length), Integer::New(0) };
      Local<Object> actualBuffer = bufferConstructor->NewInstance(3, constructorArgs);

      return scope.Close(actualBuffer);
    }

    static Handle<Value> Write(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
    }

    static Handle<Value> NewIterator(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> GetSnapshot(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> ReleaseSnapshot(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> GetProperty(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> GetApproximateSizes(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::Error(String::New("TODO: IMPLEMENT ME")));
    }

};

