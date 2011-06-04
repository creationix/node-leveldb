from os import system, remove
from os.path import abspath, exists, join
from shutil import rmtree

srcdir = abspath(".")
blddir = "build"
VERSION = "0.0.1"

leveldb_dir = join(srcdir, "deps/leveldb")
leveldb_src = ["/db/builder.cc", "/db/db_impl.cc", "/db/db_iter.cc", "/db/filename.cc", "/db/dbformat.cc", "/db/log_reader.cc", "/db/log_writer.cc", "/db/memtable.cc", "/db/repair.cc", "/db/table_cache.cc", "/db/version_edit.cc", "/db/version_set.cc", "/db/write_batch.cc", "/port/port_std.cc", "/table/block.cc", "/table/block_builder.cc", "/table/format.cc", "/table/iterator.cc", "/table/merger.cc", "/table/table.cc", "/table/table_builder.cc", "/table/two_level_iterator.cc", "/util/arena.cc", "/util/cache.cc", "/util/coding.cc", "/util/comparator.cc", "/util/crc32c.cc", "/util/env.cc", "/util/env_posix.cc", "/util/hash.cc", "/util/histogram.cc", "/util/logging.cc", "/util/options.cc", "/util/status.cc"]
leveldb_src = ["deps/leveldb" + path for path in leveldb_src]

node_leveldb_src = ["src/leveldb.cc", "src/DB.cc", "src/WriteBatch.cc", "src/Iterator.cc", "src/helpers.cc"]

build_config = join(leveldb_dir, 'build', 'build_config.h')

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")

  if not exists(build_config):
      system('cd %s && sh platform.env' % leveldb_dir)

def clean(ctx): 
  if exists("build"): rmtree("build")
  if exists(build_config): remove(build_config)

def build(bld):
  node_leveldb = bld.new_task_gen("cxx", "shlib", "node_addon")
  node_leveldb.source = leveldb_src + node_leveldb_src
  node_leveldb.name = "node_leveldb"
  node_leveldb.target = "leveldb"
  node_leveldb.uselib = ["pthread"]
  node_leveldb.includes = [leveldb_dir, leveldb_dir + '/include']
  node_leveldb.cxxflags = ['-O2', '-DNDEBUG', '-DLEVELDB_PLATFORM_STD']
