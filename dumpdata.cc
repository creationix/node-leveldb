#include <iostream>
#include "leveldb/db.h"

using namespace std;

int main(int argc, char** argv) {
  leveldb::DB* db;
  leveldb::Options options;
  if (argc < 2) {
    cerr << "Usage:\n\t" << argv[0] << " path/to/database\n";
    return 1;
  }
  cout << "Opening database at " << argv[1] << endl;
  leveldb::Status status = leveldb::DB::Open(options, argv[1], &db);
  if (!status.ok()) {
    cerr << status.ToString() << endl;
    return 1;
  }
  cout << "--------------------------------------------------------------------------------" << endl;
  leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    cout << it->key().ToString() << ": "  << it->value().ToString() << endl;
  }
  if (!it->status().ok()) {
    cerr << it->status().ToString() << endl;
    return 1;
  }
  delete it;
  cout << "--------------------------------------------------------------------------------" << endl;
  delete db;
}
