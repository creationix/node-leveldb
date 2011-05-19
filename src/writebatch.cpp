#include <v8.h>
#include <node.h>

using namespace v8;

class WriteBatch : node:ObjectWrap {
  private:
  public:
    WriteBatch() {}
    ~WriteBatch() {}
    static void Init(Handle<Object> target) {
    }
};


