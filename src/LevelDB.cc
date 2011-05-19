#include <v8.h>
#include <node.h>

#include "leveldb/db.h"

using namespace node;
using namespace v8;

class LevelDB : ObjectWrap {
  private:
  public:
    LevelDB() {}
    ~LevelDB() {}

    static void Init(Handle<Object> target) {
      v8::HandleScope scope; // used by v8 for garbage collection

      // Our constructor
      v8::Local<FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
      LevelDB::persistent_function_template = v8::Persistent<FunctionTemplate>::New(local_function_template);
      LevelDB::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
      LevelDB::persistent_function_template->SetClassName(v8::String::NewSymbol("LevelDB"));

      // Our getters and setters

      // Our methods

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

};

v8::Persistent<FunctionTemplate> LevelDB::persistent_function_template;

