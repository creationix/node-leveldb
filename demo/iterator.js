var DB = require('../build/default/leveldb.node').DB,
    uuid = require('node-uuid'),
    start = Date.now(),
    entryCount = 1000000,
    lastKey,
    readCount = 0,
    ii;

console.log("Creating test database");
var path = "/tmp/iterator.db";
var db = new DB();
db.open({create_if_missing: true}, path);

console.log('!! creating ' + entryCount + ' random key entries');
for (ii = 0; ii < entryCount; ii++) {
    var id = uuid();
    
    db.put({}, new Buffer(id), new Buffer(JSON.stringify({
        id: id,
        name: 'Bob',
        age: 33
    })));
} // for

console.log('created in ' + (Date.now() - start) + 'ms');
console.log('!! iterating db in key order');

// reset the start counter
start = Date.now();

// iterate over the test database
var iterator = db.newIterator({});

for (iterator.seekToFirst(); iterator.valid(); iterator.next()) {
    var key = iterator.key().toString('utf8');
    
    if (lastKey && lastKey > key) {
        console.log('found sorting error');
    } // if
    
    lastKey = key;
    readCount++;
} // for

console.log('read sequential ' + readCount + ' db contents in ' + (Date.now() - start) + 'ms');

// do some random seeking
for (ii = 0; ii < 100; ii++) {
    var testUUID = uuid();
    iterator.seek(new Buffer(testUUID));
    
    console.log('looking for first key after: ' + testUUID);
    
    // if we found something the report 
    if (iterator.valid()) {
        console.log('FOUND: ' + iterator.key());
    } // if
} // for

db.close();
DB.destroyDB(path, {});