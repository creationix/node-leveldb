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
    console.log("ok");
    
    // Putting
    console.log("\nPutting...");
    var key = "Hello";
    var value = "World";
    db.put(key, value, function(err) {
        if (err) throw err;
        console.log("ok");
        
        // Getting
        console.log("\nGetting...");
        db.get(key, function(err, val) {
            if (err) throw err;
            if (val != value) throw "Expected: " + value + ", got: " + val;
            console.log("ok");

            // Deleting
            console.log("\nDeleting...");
            db.del(key, function(err) {
                if (err) throw err;
                console.log("ok");
            
                // Closing
                console.log("\nClosing...")
                db.close(function(err) {
                    if (err) throw err;
                    console.log('ok');
                });
            });
        });
    });
});

