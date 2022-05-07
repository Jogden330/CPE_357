#!/bin/csh -f

#
# Run the given command against the set of arguments given below
#
# config stuff
if ( $#argv != 2 ) then
  echo "usage: $0 <cmd> <datadir>"
  exit -1
endif

set cmd     = $argv[1]
set datadir = $argv[2]

if ( -d /tmp/man1 ) then
  set mandir = /tmp/man1
else if ( -d /usr/share/man/man1 ) then
  set mandir = /usr/share/man/man1
else
  set mandir = /usr/man/man1
endif

if ( -f /usr/share/dict/words ) then
  set dict=/usr/share/dict/words
else if ( -f /usr/dict/words ) then
  set dict=/usr/dict/words
else
  set dict=/home/pn-cs357/Given/Asgn2/words
endif

# the arguments---only change these 
#-----------------------------------------------------------------------
set timeout = 300
set args    = ( $dict $mandir/* )
#-----------------------------------------------------------------------

# The run
timelimit $timeout $cmd $args

# exit with the status left behind by cmd...
exit $status
