# Node-LevelDB

The leveldb library provides a persistent key value store. Keys and values are arbitrary byte arrays. The keys are ordered within the key value store according to a user-specified comparator function.

This project is node bindings to this excellent library so that node programs can either implement their own custom databases or simply use it directly as a fast, simple key/value store.

While implementing nStore, I realized there are a couple things that V8 and node are not very good at.  They are heavy disk I/O and massive objects with either millions of keys or millions of sub objects.

Since LevelDB provides good primitives like MVCC and binary support (It was designed to back IndexDB in the Chrome browser), then it can be used as a base to implement things like CouchDB.

## Status

This project is still under heavy development in my free-time.  It was started by a long night where I couldn't sleep.  I am not a C++ coder and am learning as I go.

The I/O in this initial version is all blocking and doesn't make use of node's thread pool for disk I/O.  Since my use case is single-user applications, this is a perfectly fine compromise.

Eventually I'll need to make async versions of all the calls so that it can also be used in a highly concurrent server environment.

## API

The API is meant to be an almost one-to-one mapping to the underlying C++ library.  Essentially it's a key/value store, but has some extra nice things like sorted keys, snapshots and iterators.

### LevelDB

This is the main Class and probably the only one that a user needs to create manually.

    var LevelDB = require('leveldb.node').LevelDB
    var db = new LevelDB
    db.open({create_if_missing: true}, "path/to/my/db");
    db.put({sync: true}, new Buffer("myKey"), new Buffer("My Value!"));
    db.close();

I'll document more as this stabilizes.  In the mean-time, check out the demo script I use for testing.
<https://github.com/creationix/node-leveldb/blob/master/demo/demo.js>

## Compiling

I've only tested on my Ubuntu laptop, but it compiles cleanly using only the wscript file.  To compile simple type.

    node-waf configure build

Then to test, run the demo script

    node demo/demo.js

The leveldb library is bundled in the deps folder and is compiled in staticly.  The resulting node addon is about 4mb unstripped.  If you want to make it smaller, I've had success using the `strip` command on it which shrinks it down to about 300kb.

    strip build/default/leveldb.node

## Contributing

Since I am not experienced in the world of C++ and am still learning, I welcome contributions.  Mainly what I need right now is someone with experience to review my code and tell me things I'm doing stupid.  Maybe add a C++ best pratices note to this document.  Also once the initial blocking version of the library is done, I'll need to add async ability to all the functions that do disk I/O.

Currently Randall Leeds (@tilgovi and one of the CouchDB commiters) has expressed interest in helping and already has commit rights to the project.

## Roadmap

This is the short-term and long roadmap to track progress.

- Implement just enough to make a simple key/value store. (90% done)
- Implement bulk writes
- Implement Iterators
- Implement Snapstops
- Implement getProperty and getApproximateSizes
- ...
- Add async versions of all functions that use disk I/O
- ...
- Hand off project to a loyal maintainer and start my next adventure.

