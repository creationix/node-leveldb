class LevelDB : ObjectWrap {
  private:
  public:
    LevelDB() {}
    ~LevelDB() {}
    static void Init(Handle<Object> target) {
    }
};

v8::Persistent<FunctionTemplate> LevelDB::persistent_function_template;

