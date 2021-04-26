#!/bin/sh

if test "$WXHOME" = ""; then
  WXHOME=`sh findwx`
  if test "$WXHOME" = ""; then
    echo "Cannot find WXHOME in environment" 1>&2
    exit 1
  fi
  export WXHOME
fi

if (grep TMP_DEBUG osiris.m4 >/dev/null) ; then
  if ! (grep -i beta ./Version/OsirisVersion.h >/dev/null); then
    echo "TMP_DEBUG is defined for non-Beta" 
    exit 1
  fi
fi
set -x

for x in wxthings/lib wxplotctrl/lib BaseClassLib/lib OsirisMath/lib OsirisLib2.01/lib TestAnalysisDirectoryLCv2.11/bin fsa2xml/bin wxXml2/lib wxXsl/lib nwxZip/lib nwx/lib OsirisAnalysis/bin OpenFolder/bin GenerateLadderFile/bin GenerateILSFamily/bin BuildStandardControlFile/bin; do
(cd $x ; pwd ; aclocal; autoconf; automake -a)
done
(cd OsirisAnalysis; sh updatePlist.sh)
(cd OsirisLib2.01 ; sh blex.sh clean all )

aclocal
autoconf
automake -a
./configure
