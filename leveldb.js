// Mock implementation in JS to help guide API
// This is meant to mirror the C++ API, but be JavaScripty

function LevelDB() {
}

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

// Create a new LevelDB instance database
var db = new LevelDB();

var options = {
  create_if_missing: true,
//  error_if_missing: true
}

// Opening a database
db.open("/tmp/testdb", options, function (err, status) {
  // err will be an exception object if there is a problem
  // otherwise status will be a string message
});

// Closing a database
db.close();

// Getting a value from the database
db.get(key, options, function (err, value, status) { /*...*/ });

// Doing an async write (no disk I/O, fire and forget)
var status = db.put(key, value); // will throw on error

// Doing an async delete (no disk I/O, fire and forget)
var status = db.del(key) 

// Doing a "sync" (flushed) write
db.put(key, value, {sync: true}, function (err, status) { /*...*/ });


// TODO: figure out write_batch





