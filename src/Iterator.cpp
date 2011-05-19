class Iterator : ObjectWrap {
  private:
  public:
    Iterator() {}
    ~Iterator() {}
    static void Init(Handle<Object> target) {
    }
};

v8::Persistent<FunctionTemplate> Iterator::persistent_function_template;

