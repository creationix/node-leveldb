#include <v8.h>
#include <node.h>

#include "leveldb/db.h"

//using namespace node;
using namespace v8;

class LevelDB : node::ObjectWrap {
  private:
    leveldb::DB* db;

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

    static Handle<Value> processStatus(leveldb::Status status) {
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
      // Set some default values


      // Wrap our C++ object as a Javascript object
      LevelDB_instance->Wrap(args.This());

      // Our constructor function returns a Javascript object which is a wrapper for our C++ object,
      // This is the expected behavior when calling a constructor function with the new operator in Javascript.
      return args.This();
    }

    static Handle<Value> Open(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 2 && args[0]->IsString() && args[1]->IsObject())) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected (String, Object)")));
      }

      // Get this and arguments
      LevelDB* self = node::ObjectWrap::Unwrap<LevelDB>(args.This());
      String::Utf8Value name(args[0]);
      Local<Object> opts = Object::Cast(*args[1]);

      // Do actual work
      leveldb::Status status = leveldb::DB::Open(processOptions(opts), *name, &(self->db));

      // Return result or exception
      if (status.ok()) {
        return String::New(status.ToString().c_str());
      } else {
        return ThrowException(Exception::Error(String::New(status.ToString().c_str())));
      }
    }

    static Handle<Value> Close(const Arguments& args) {
      HandleScope scope;

      // Check args
      if (!(args.Length() == 0)) {
        return ThrowException(Exception::TypeError(String::New("Invalid arguments: Expected ()")));
      }

      LevelDB* self = node::ObjectWrap::Unwrap<LevelDB>(args.This());

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
      leveldb::Options options;


      leveldb::Status status = leveldb::DestroyDB(*name, options);

      if (status.ok()) {
        return String::New(status.ToString().c_str());
      } else {
        return ThrowException(Exception::TypeError(String::New(status.ToString().c_str())));
      }

    }

    static Handle<Value> RepairDB(const Arguments& args) {
      HandleScope scope;

      String::Utf8Value name(args[0]);
      leveldb::Options options;

      leveldb::Status status = leveldb::RepairDB(*name, options);

      if (status.ok()) {
        return String::New(status.ToString().c_str());
      } else {
        return ThrowException(Exception::TypeError(String::New(status.ToString().c_str())));
      }

    }

    static Handle<Value> Put(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> Del(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> Write(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> Get(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }

    static Handle<Value> NewIterator(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> GetSnapshot(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> ReleaseSnapshot(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> GetProperty(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }
    static Handle<Value> GetApproximateSizes(const Arguments& args) {
      HandleScope scope;
      return ThrowException(Exception::TypeError(String::New("TODO: IMPLEMENT ME")));
    }

};

