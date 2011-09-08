var leveldb = require('../build/default/leveldb.node'),
    DB = leveldb.DB,
    Iterator = leveldb.Iterator;

var path = "/tmp/large.db";

var index = 0;
var in_flight = 0;
var found = 0;
var not_found = 0;

var db = new DB();
db.open(path, function(err) {
    if (err) throw err;
    
    setInterval(function() {
        console.log('Index: ' + index + '   In flight: ' + in_flight + '   Found: ' + found + '   Not found: ' + not_found);
        refresh();
    }, 100);

    refresh();
});

function refresh() {
    while (in_flight < 10000) {
        var key = "row" + index;
        
        db.get(key, (function(index) { return function(err, value) {
            in_flight -= 1;
            if (value) {
                var obj = JSON.parse(value);
                if (obj.index != index) {
                    console.log('key: ' + key + ' = ' + value);
                }
                found += 1;
            }
            else {
                not_found += 1;
            }
        }})(index));
        
        in_flight += 1;
        index += 1;
    }
}

