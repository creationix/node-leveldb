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
};

v8::Persistent<FunctionTemplate> WriteBatch::persistent_function_template;

