#!/usr/bin/python

import sys
import os
import re
import commands
from optparse import OptionParser

FILE_MATCHER = re.compile('File \'(.*)\'')
EXECUTED_MATCHER = re.compile('Lines executed:([0-9.]+)% of ([0-9]+)')

def FindObjectFiles(directory):
  results = []
  for root, dirs, files in os.walk(directory):
    for file in files:
      (name, ext) = os.path.splitext(file)
      if ext == '.o':
        results.append(os.path.join(root, file))
  return results

def ParseGcovOutput(output, srcRoot):
  results = []
  item = {}

  lines = output.splitlines()
  for line in lines:
    if line == '':
      if 'file' in item:
        results.append(item)
      item = {}
      continue

    m = FILE_MATCHER.match(line)
    if m != None:
      path = m.group(1)
      if os.path.realpath(path).startswith(os.path.realpath(srcRoot)):
        item['file'] = os.path.relpath(path, srcRoot)
      continue

    m = EXECUTED_MATCHER.match(line)
    if m != None:
      item['executed'] = float(m.group(1))
      item['executable'] = int(m.group(2))
      continue

  return results

def GetExecuted(item):
  return item['executed']

def GenerateCovReport(buildDirectory, srcRoot, outPath):
  # find object files
  objectFiles = FindObjectFiles(buildDirectory)

  # generate command
  command = 'gcov';
  for file in objectFiles:
    command = command + ' \'' + file + '\''

  # execute command
  (status, output) = commands.getstatusoutput(command)

  if status != 0:
    raise IOError('gcov command failed.')

  results = ParseGcovOutput(output, srcRoot)
  results = sorted(results, key=GetExecuted, reverse=True)

  print '    %    executable'
  print 'executed      lines path'
  for result in results:
    print '  %6.2f      %5d %s' % (result['executed'], result['executable'], result['file'])

def main(argv):
  parser = OptionParser()
  parser.add_option('-d', '--dir', dest='dir', default='.',
                   help='Directory for searching object files', metavar='DIR')
  parser.add_option('-s', '--source', dest='source', default='.',
                   help='Root directory of sources', metavar='DIR')
  parser.add_option('-o', '--out', dest='out', default='coverage.txt',
                   help='Output file name', metavar='FILE')
  (options, args) = parser.parse_args()

  GenerateCovReport(options.dir, options.source, options.out)

  return 0

if __name__ == '__main__':
  sys.exit(main(sys.argv))
