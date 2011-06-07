// Mock implementation in JS to help guide API
// This is meant to mirror the C++ API, but be JavaScripty

function DB() {}
DB.prototype.open = function open(name, options, callback) { /*...*/ };
DB.prototype.close = function close(callback) { /*...*/ };
DB.prototype.put = function put(key, value, options, callback) { /*...*/ };
DB.prototype.del = function del(key, options, callback) { /*...*/ };
DB.prototype.write = function write(updates, options, callback) { /*...*/ };
DB.prototype.get = function get(key, options, callback) { /*...*/ };
DB.prototype.newIterator = function newIterator(options) { /*...*/ };
DB.prototype.getSnapshot = function getSnapshot() { /*...*/ };
DB.prototype.releaseSnapshot = function releaseSnapshot(snapshot) { /*...*/ };
DB.prototype.getProperty = function getProperty(name) { /*...*/ };
DB.prototype.getApproximateSizes = function releaseSnapshot(ranges) { /*...*/ };
DB.openDB = function openDB(name, options, callback) { /*...*/ }; // Convience wrapper around constructor and open
DB.destroyDB = function destroyDB(name, options, callback) { /*...*/ };
DB.repairDB = function repairDB(name, options, callback) { /*...*/ };

function WriteBatch() {}
WriteBatch.prototype.put = function put(key, value) { /*...*/ };
WriteBatch.prototype.del = function del(key) { /*...*/ };
WriteBatch.prototype.clear = function clear() { /*...*/ };

function Iterator(options) { /*...*/ }
Iterator.prototype.isValid  = function isValid() { /*...*/ };
Iterator.prototype.seekToFirst  = function seekToFirst(callback) { /*...*/ };
Iterator.prototype.seekToLast  = function seekToLast(callback) { /*...*/ };
Iterator.prototype.seek  = function seek(target, callback) { /*...*/ };
Iterator.prototype.next  = function next(callback) { /*...*/ };
Iterator.prototype.prev  = function prev(callback) { /*...*/ };
Iterator.prototype.getKey  = function getKey(callback) { /*...*/ };
Iterator.prototype.getValue  = function getValue(callback) { /*...*/ };
Iterator.prototype.getStatus  = function getStatus() { /*...*/ };
Iterator.prototype.registerCleanup = function registerCleanup(callback, arg1, arg2) { /*...*/ };


// See options.h (in leveldb) for better descriptions

// "Options" have the following fields
//   comparator - a callback used for sorting keys
//   create_if_missing - boolean
//   error_if_exists - boolean
//   paranoid_checks - boolean
//   env - hook to replace fs interaction
//   info_log - writable stream
//   write_buffer_size - integer bytes
//   max_open_files - integer
//   block_cache - buffer
//   block_size - integer bytes
//   block_restart_interval - integer
//   compression - flag to disable compression

// "ReadOptions" have the follownig fields
//   verify_checksums - boolean
//   fill_cache - boolean
//   snapshot - snapshot object

// "WriteOptions" have the following fields
//   sync - boolean (false is fire and forget)
//   post_write_snapshot - snapshot object

// Sample usage ////////////////////////////////////////////////////////////////

// Create a new LevelDB instance
var db = new DB();

// `options` is always optional and defaults are assumed if left out

// Opening a database
db.open("/tmp/testdb", options, function (err) {
  // err will be an exception object if there is a problem
});

// Closing a database
db.close(function (err) { /*...*/ });

// Putting a value in the database
db.put(key, value, options, function (err) { /*...*/ });

// Deleting a value from the database
db.del(key, options, function (err) { /*...*/ });

// Write a batch of updates in one call
db.write(updates, options, function (err) { /*...*/ });

// Getting a value from the database
db.get(key, options, function (err, value) { /*...*/ });

// get an iterator
var iterator = db.newIterator(options);

// get a handle to the current state
var snapshot = db.getSnapshot();
// release it when done
db.releaseSnapshot(snapshot);

var value = db.getProperty(name);

var sized = db.getApproximateSizes(ranges);

DB.destroyDB(name, options, function (err) { /*...*/ });
DB.repairDB(name, options, function (err) { /*...*/ });
