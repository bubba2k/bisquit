import os
import ycm_core

flags = [
  '-Wall',
  '-Wextra',
  '-Wno-long-long',
  '-Wno-variadic-macros',
  '-std=c++20',
  ]

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', ]

def FlagsForFile( filename, **kwargs ):
  return {
  'flags': flags,
  'do_cache': True
  }
