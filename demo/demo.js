var leveldb = require('../build/default/leveldb.node'),
    LevelDB = leveldb.LevelDB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

var db = new LevelDB();
console.log("Opening...");
db.open(__dirname + "/testdb", {}, function (err, status) {
  if (err) throw err;
  console.log(status);
  console.log("Closing...");
  db.close(function (err, status) {
    if (err) throw err;
    console.log(status);
    console.log("Done");
  });
});

