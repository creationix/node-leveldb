var leveldb = require('../build/default/leveldb.node'),
    DB = leveldb.DB,
    Iterator = leveldb.Iterator,
    WriteBatch = leveldb.WriteBatch;

var path = __dirname + "/testdb";

var db = new DB();

// Opening
console.log("Opening...");
db.open(path, {create_if_missing: true, paranoid_checks: true}, function(err) {
    if (err) throw err;
    else console.log("ok");
    
    // Putting  TODO: async
    console.log("\nPutting...");
    var key = new Buffer("Hello");
    var value = new Buffer("World");
    var status = db.put({}, key, value);
    console.log(status);
    
    console.log("\nClosing...")
    db.close(function(err) {
        if (err) throw err;
        console.log('ok');
    });
});

