lib = require('../build/default/leveldb.node');

for (i in lib) {
   exports[i] = lib[i];
}

