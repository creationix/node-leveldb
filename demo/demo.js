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

var key = new Buffer("Hello");

console.log("\nPutting...");
for (var i = 0; i < 100; i++) {
  db.put({}, new Buffer("item" + i), new Buffer("value" + i));
  if (i%2) {
    db.del({}, new Buffer("item" + i));
  }
}
var status = db.put({}, key, new Buffer("World"));
console.log(status);

console.log("\nGetting...");
var value = db.get({}, key);
console.dir(value.__proto__);
console.dir(Buffer.prototype);
console.dir(value);
console.log("OK");

//console.log("\nDeleting...");
//var status = db.del({}, key);
//console.log(status);

console.log("\nClosing...");
db.close();
console.log("OK");

//console.log("\nRepairing...");
//var status = LevelDB.repairDB(path, {});
//console.log(status);

//console.log("\nDestroying...");
//var status = LevelDB.destroyDB(path, {});
//console.log(status);

