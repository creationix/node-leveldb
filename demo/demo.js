var leveldb = require('../build/default/leveldb.node'),
    LevelDB = leveldb.LevelDB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

console.dir(LevelDB);
console.dir(LevelDB.prototype);
var path = __dirname + "/testdb";
var db = new LevelDB();
console.log("Opening...");
var status = db.openSync(path, {});
console.dir(status);
console.log("Closing...");
db.closeSync();
console.log("Repairing...");
var status = LevelDB.repairDBSync(path, {});
console.dir(status);
console.log("Destroying...");
var status = LevelDB.destroyDBSync(path, {});
console.dir(status);

