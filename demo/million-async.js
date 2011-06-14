var leveldb = require('../build/default/leveldb.node'),
    DB = leveldb.DB,
    WriteBatch = leveldb.WriteBatch;

console.log("Creating test database");
var path = "/tmp/large.db";
var db = new DB();

var batchSize = 1000;
var totalSize = 1000000;

db.open(path, {create_if_missing: true}, function() {
    
    console.log("Serializing and inserting 1,000,000 rows...");
    var start = Date.now();
    
    function doBatch(i, cb) {
        if (i % 10000 == 0)
            console.log("i = " + i);
        
        var wb = new WriteBatch();
        
        for (var j = 0; j < batchSize; j += 1) {
            var key = "row" + i;
            var value = JSON.stringify({
                index: i,
                name: "Tim",
                age: 28
            });
            wb.put(key, value);
            i += 1;
        }
        
        db.write(wb, function() {
            if (i < totalSize)
                doBatch(i, cb);
            else
                cb();
        });
    }
    
    doBatch(0, function() {
        var delta = Date.now() - start;
        console.log("Completed in %d ms", delta);
        console.log("%s inserts per second", Math.floor(totalSize * 1000 / delta));

        console.log("\nClosing...");
        db.close(function() {
            console.log("Done");
        });
    });
});
