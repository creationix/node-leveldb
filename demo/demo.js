var leveldb = require('../build/default/leveldb.node'),
    DB = leveldb.DB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

var path = __dirname + "/testdb";

var db = new DB();

// Opening
console.log("Opening...");
var status = db.open({create_if_missing: true, paranoid_checks: true}, path);
console.log(status);

// Putting
console.log("\nPutting...");
var key = new Buffer("Hello");
var value = new Buffer("World");
var status = db.put({}, key, value);
console.log(status);

// Getting
console.log("\nGetting...");
if(value.toString() == db.get({}, key).toString()) {
  console.log("OK");
} else {
  console.log("FAIL");
}

// Bulk writing
console.log("\nBulk writing...");
var wb = new WriteBatch();
wb.put(new Buffer('foo'), new Buffer('bar'));
wb.put(new Buffer('booz'), new Buffer('baz'));
var status = db.write({}, wb);
console.log("Bulk writing: putting: " + status);

if (db.get({}, new Buffer('booz')).toString() == 'baz' &&
    db.get({}, new Buffer('foo')).toString() == 'bar') {
  var status = "OK";
} else {
  var status = "FAIL";
}
console.log("Bulk writing: getting: " + status);

// Deleting
console.log("\nDeleting...");
var status = db.del({}, key);
console.log(status);

// Closing
console.log("\nClosing...");
db.close();
console.log("OK");