var leveldb = require('../build/default/leveldb.node'),
    LevelDB = leveldb.LevelDB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

var db = new LevelDB();
db.open("test.db", {}, function (err, status) {
  if (err) throw err;
  console.log(status);
});

