class WriteBatch : ObjectWrap {
  private:
  public:
    WriteBatch() {}
    ~WriteBatch() {}

    static void Init(Handle<Object> target) {
      v8::HandleScope scope; // used by v8 for garbage collection

      // Our constructor
      v8::Local<FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
      WriteBatch::persistent_function_template = v8::Persistent<FunctionTemplate>::New(local_function_template);
      WriteBatch::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
      WriteBatch::persistent_function_template->SetClassName(v8::String::NewSymbol("WriteBatch"));

      // Our getters and setters

      // Our methods

      // Binding our constructor function to the target variable
      target->Set(String::NewSymbol("WriteBatch"), WriteBatch::persistent_function_template->GetFunction());
    }

    // This is our constructor function. It instantiate a C++ WriteBatch object and returns a Javascript handle to this object.
    static Handle<Value> New(const Arguments& args) {
      HandleScope scope;
      WriteBatch* WriteBatch_instance = new WriteBatch();
      // Set some default values
      
      // Wrap our C++ object as a Javascript object
      WriteBatch_instance->Wrap(args.This());
      
      // Our constructor function returns a Javascript object which is a wrapper for our C++ object, 
      // This is the expected behavior when calling a constructor function with the new operator in Javascript.
      return args.This();
    }

};

v8::Persistent<FunctionTemplate> WriteBatch::persistent_function_template;

