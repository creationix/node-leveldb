class Iterator : ObjectWrap {
  private:
  public:
    Iterator() {}
    ~Iterator() {}
    static void Init(Handle<Object> target) {
      v8::HandleScope scope; // used by v8 for garbage collection

      // Our constructor
      v8::Local<FunctionTemplate> local_function_template = v8::FunctionTemplate::New(New);
      Iterator::persistent_function_template = v8::Persistent<FunctionTemplate>::New(local_function_template);
      Iterator::persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1); // 1 since this is a constructor function
      Iterator::persistent_function_template->SetClassName(v8::String::NewSymbol("Iterator"));

      // Our getters and setters

      // Our methods

      // Binding our constructor function to the target variable
      target->Set(String::NewSymbol("Iterator"), Iterator::persistent_function_template->GetFunction());
    }
};

v8::Persistent<FunctionTemplate> Iterator::persistent_function_template;

