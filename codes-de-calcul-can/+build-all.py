#! /usr/bin/env python3
# -*- coding: UTF-8 -*-

#------------------------------------------------------------------------------
# https://docs.python3.org/2/library/subprocess.html#module-subprocess

import subprocess
import sys
import os

#------------------------------------------------------------------------------

scriptDir = os.path.dirname (os.path.abspath (sys.argv [0]))
os.chdir (scriptDir)
for name in sorted (os.listdir (".")) :
  if os.path.splitext(name)[1] == ".cpp" :
    print ("Compilation de " + name)
    returncode = subprocess.call (["g++", "-O3", "-std=c++14", name, "-o", os.path.splitext(name)[0]])
    if returncode != 0 :
      sys.exit (returncode)

#------------------------------------------------------------------------------
