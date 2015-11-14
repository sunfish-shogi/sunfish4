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

def ParseGcovOutput(output, srcRoot, exceptDirectory):
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
      fullpath = os.path.realpath(path)
      relpath = os.path.relpath(path, srcRoot)
      if (fullpath.startswith(os.path.realpath(srcRoot)) and
          (exceptDirectory == None or
           not relpath.startswith(exceptDirectory))):
        item['file'] = relpath
        item['gcov'] = os.path.realpath(os.path.basename(path) + '.gcov')
      continue

    m = EXECUTED_MATCHER.match(line)
    if m != None:
      item['executed'] = float(m.group(1))
      item['executable'] = int(m.group(2))
      continue

  return results

def PrintResult(results):
  maxSrcLength = 0
  for result in results:
    maxSrcLength = max(maxSrcLength, len(result['file']))

  print '    %    executable source'
  print 'executed      lines file'
  fmt = '  %6.2f      %5d %-' + str(maxSrcLength) + 's %s'
  for result in results:
    print fmt % (
        result['executed'], result['executable'],
        result['file'], result['gcov'])

def GetExecuted(item):
  return item['executed']

def GenerateCovReport(buildDirectory, srcRoot, exceptDirectory, outPath):
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

  results = ParseGcovOutput(output, srcRoot, exceptDirectory)
  results = sorted(results, key=GetExecuted, reverse=True)

  # print result
  PrintResult(results)

def main(argv):
  parser = OptionParser()
  parser.add_option('-d', '--dir', dest='dir', default='.',
                   help='Directory for searching object files', metavar='DIR')
  parser.add_option('-s', '--source', dest='source', default='.',
                   help='Root directory of sources', metavar='DIR')
  parser.add_option('-e', '--except', dest='except_', default=None,
                    help='Directory to except', metavar='DIR')
  parser.add_option('-o', '--out', dest='out', default='coverage.txt',
                   help='Output file name', metavar='FILE')
  (options, args) = parser.parse_args()

  GenerateCovReport(options.dir, options.source, options.except_, options.out)

  return 0

if __name__ == '__main__':
  sys.exit(main(sys.argv))
