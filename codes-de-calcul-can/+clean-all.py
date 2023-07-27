#! /usr/bin/env python3
# -*- coding: UTF-8 -*-

#------------------------------------------------------------------------------*
# https://docs.python3.org/2/library/subprocess.html#module-subprocess

import subprocess
import sys
import os

#------------------------------------------------------------------------------*

scriptDir = os.path.dirname (os.path.abspath (sys.argv [0]))
os.chdir (scriptDir)
for name in sorted (os.listdir (".")) :
  if (os.path.splitext(name)[1] == ".cpp") and os.path.isfile (os.path.splitext(name)[0]) :
    print ("Suppression de " + os.path.splitext(name)[0])
    returncode = subprocess.call (["rm", os.path.splitext(name)[0]])
    if returncode != 0 :
      sys.exit (returncode)

#------------------------------------------------------------------------------*
