var leveldb = require('../build/default/leveldb.node'),
    LevelDB = leveldb.LevelDB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

var db = new LevelDB();
console.log("Opening...");
var status = db.openSync(__dirname + "/testdb", {});
console.dir(status);
console.log("Closing...");
db.closeSync();
console.log("done");
