DB   = require('../build/default/leveldb.node').DB
uuid = require('node-uuid')
td   = require("twisted-deferred")

start = Date.now()

entryCount = 1000
readCount = 0

defer = td.toDeferred
DeferredList = td.DeferredList

console.log("Creating test database")
path = "/tmp/iterator.db"
db = new DB()

d = defer(db.open.bind(db), path, {create_if_missing: true})
d.addCallback () ->
   console.log('!! creating ' + entryCount + ' random key entries')
   deferreds = []
   for i in [0 .. entryCount]
       id = uuid()
       
       put_d = defer db.put.bind(db), id, JSON.stringify({id: id, name: 'Bob', age: 33})
       deferreds.push(put_d)

   return DeferredList(deferreds)

d.addCallback () ->
   console.log('created in ' + (Date.now() - start) + 'ms')
   console.log('!! iterating db in key order')

   # reset the start counter
   start = Date.now()

   # iterate over the test database
   iterator = db.newIterator({})

   iterator.seekToFirst()

   while iterator.valid()
       key = iterator.key().toString('utf8')
       
       if lastKey && lastKey > key
           console.log('found sorting error')
       
       lastKey = key
       readCount++

       iterator.next()

   console.log('read sequential ' + readCount + ' db contents in ' + (Date.now() - start) + 'ms')

d.addCallback () ->
   console.log 'Start Seek test'

   deferred = new td.Deferred()
   iterator = db.newIterator({})
   testUUID = uuid()
   iterator.seek "" + testUUID, () ->
      console.log('looking for first key after: ' + testUUID)
      # if we found something the report
      if (iterator.valid())
         console.log('FOUND: ' + iterator.key().toString('utf-8'))

      deferred.callback()

   deferred

d.addCallback () ->
   console.log "success"

d.addErrback (err) ->
   console.log err.message.stack

d.addBoth () ->
   db.close()
   DB.destroyDB(path, {})

