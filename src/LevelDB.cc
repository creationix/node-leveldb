#include <v8.h>
#include <node.h>

#include "leveldb/db.h"

//using namespace node;
using namespace v8;

class LevelDB : node::ObjectWrap {
  private:
    leveldb::DB* db;

  public:
    LevelDB() {}
    ~LevelDB() {}
    

    // Holds our constructor function
    static v8::Persistent<v8::FunctionTemplate> persistent_function_template;
    
    static void Init(v8::Handle<v8::Object> target) {
      v8::HandleScope scope; // used by v8 for garbage collection

      // Our constructor
      v8::Local<v8::FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
      LevelDB::persistent_function_template = v8::Persistent<v8::FunctionTemplate>::New(local_function_template);
      LevelDB::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
      LevelDB::persistent_function_template->SetClassName(v8::String::NewSymbol("LevelDB"));

      // Our getters and setters

      // Our methods
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "openSync", OpenSync);
      NODE_SET_PROTOTYPE_METHOD(LevelDB::persistent_function_template, "closeSync", CloseSync);

      // Binding our constructor function to the target variable
      target->Set(v8::String::NewSymbol("LevelDB"), LevelDB::persistent_function_template->GetFunction());
    }

    // This is our constructor function. It instantiate a C++ LevelDB object and returns a Javascript handle to this object.
    static v8::Handle<v8::Value> New(const v8::Arguments& args) {
      v8::HandleScope scope;
      LevelDB* LevelDB_instance = new LevelDB();
      // Set some default values
      
      
      // Wrap our C++ object as a Javascript object
      LevelDB_instance->Wrap(args.This());
      
      // Our constructor function returns a Javascript object which is a wrapper for our C++ object, 
      // This is the expected behavior when calling a constructor function with the new operator in Javascript.
      return args.This();
    }
    

    // notification.send();
    // This is a method part of the constructor function's prototype
    static v8::Handle<v8::Value> OpenSync(const v8::Arguments& args) {
      v8::HandleScope scope;
      // Extract C++ object reference from "this" aka args.This() argument
      LevelDB* db_instance = node::ObjectWrap::Unwrap<LevelDB>(args.This());
      
      // Convert first argument to V8 String
      v8::String::Utf8Value name(args[0]);

      // TODO: parse options from user and not hard-code them
      leveldb::Options options;
      options.create_if_missing = true;
      
      leveldb::Status status = leveldb::DB::Open(options, *name, &(db_instance->db));

      if (status.ok()) {
        return String::New(status.ToString().c_str());
      } else {
        return ThrowException(Exception::TypeError(String::New(status.ToString().c_str())));
      }
    }
    
    // notification.send();
    // This is a method part of the constructor function's prototype
    static v8::Handle<v8::Value> CloseSync(const v8::Arguments& args) {
      v8::HandleScope scope;
      // Extract C++ object reference from "this" aka args.This() argument
      LevelDB* db_instance = node::ObjectWrap::Unwrap<LevelDB>(args.This());
      
      delete db_instance->db;
      
      return Undefined();
    }


};

