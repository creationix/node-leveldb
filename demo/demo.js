var leveldb = require('../build/default/leveldb.node'),
    LevelDB = leveldb.LevelDB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

console.dir(LevelDB);
var path = __dirname + "/testdb";
var db = new LevelDB();
console.log("Opening...");
var status = db.openSync(path, {});
console.dir(status);
console.log("Closing...");
db.closeSync();
console.log("Destroying...");
var status = LevelDB.destroyDB(path, {});
console.dir(status);

