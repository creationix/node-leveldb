var DB = require('../build/default/leveldb.node').DB;

console.log("Creating test database");
var path = "/tmp/large.db";
var db = new DB();
db.open({create_if_missing: true}, path);

console.log("Serializing and inserting 1,000,000 rows...");
var start = Date.now();
for (var i = 0; i < 1000000; i++) {
  db.put({}, new Buffer("row" + i), new Buffer(JSON.stringify({
    index: i,
    name: "Tim",
    age: 28
  })));
}
var delta = Date.now() - start;
console.log("Completed in %d ms", delta);
console.log("%s inserts per second", Math.floor(1000000000 / delta));

var data = new Buffer(4096);
for (i = 0; i < 4096; i++) {
  data[i] = Math.floor(Math.random() * 256);
}
console.log("Inserting the same 4k random bytes into 100,000 rows");
start = Date.now();
for (i = 0; i < 100000; i++) {
  db.put({}, new Buffer("binary" + i), data);
}
delta = Date.now() - start;
console.log("Completed in %d ms", delta);
console.log("%s inserts per second", Math.floor(100000000 / delta));

console.log("Closing and destroying database");
db.close();
DB.destroyDB(path, {});

