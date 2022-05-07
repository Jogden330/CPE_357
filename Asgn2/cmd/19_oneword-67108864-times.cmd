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

if ( ! -x $LIBDIR/kenormous ) then
  echo "Unable to find $LIBDIR
  exit 1
endif

#-----------------------------------------------------------------------
set timeout = 300
set args    = ( )
#-----------------------------------------------------------------------

#echo "Trying to run: $LIBDIR/kenormous 67108864 | timelimit $timeout $cmd $args"

# The run
$LIBDIR/kenormous 67108864 kenormous | timelimit $timeout $cmd $args

# exit with the status left behind by cmd...
exit $status
