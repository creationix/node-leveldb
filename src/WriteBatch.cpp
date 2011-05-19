class WriteBatch : ObjectWrap {
  private:
  public:
    WriteBatch() {}
    ~WriteBatch() {}
    static void Init(Handle<Object> target) {
    }
};

v8::Persistent<FunctionTemplate> WriteBatch::persistent_function_template;

