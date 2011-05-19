import Options, glob
from os.path import join, dirname, abspath, exists
from shutil import copy, rmtree
from os import unlink
import sys, os

srcdir = "."
blddir = "build"
VERSION = "0.0.1"
srcdir = abspath(srcdir)
leveldbdir = srcdir + "/deps/leveldb"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")

def clean(ctx): 
  if exists("build"): rmtree("build")

def build(bld):
  node_leveldb = bld.new_task_gen("cxx", "shlib", "node_addon")
  node_leveldb.source = bld.glob("index.cc")
  node_leveldb.name = "leveldb"
  node_leveldb.target = "leveldb"
  node_leveldb.uselib = ["leveldb"]
  node_leveldb.includes = [leveldbdir + '/include']
  node_leveldb.cxxflags = ['-O2', '-m32']


