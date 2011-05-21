var leveldb = require('../build/default/leveldb.node'),
    LevelDB = leveldb.LevelDB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

//console.dir(LevelDB);
//console.dir(LevelDB.prototype);

var path = __dirname + "/testdb";

var db = new LevelDB();

console.log("Opening...");
var status = db.open({create_if_missing: true, paranoid_checks: true}, path);
console.log(status);

console.log("\nPutting...");
var status = db.put({}, new Buffer("Hello"), new Buffer("World"));
console.log(status);

console.log("\nDeleting...");
var status = db.del({}, new Buffer("Hello"));
console.log(status);

console.log("\nClosing...");
db.close();
console.log("OK");

console.log("\nRepairing...");
var status = LevelDB.repairDB(path, {});
console.log(status);

console.log("\nDestroying...");
var status = LevelDB.destroyDB(path, {});
console.log(status);

