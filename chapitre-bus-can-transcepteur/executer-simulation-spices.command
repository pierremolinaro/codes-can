#!/bin/sh
set -x
DIR=`dirname $0` &&
cd $DIR &&

sed 's/plot v(5)/print v(5) > reseau-etoile-1-resistance.plot/' $DIR/reseau-etoile-1-resistance.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift reseau-etoile-1-resistance.plot 1e7
rm $DIR/reseau-etoile-1-resistance.plot

sed 's/plot v(5)/print v(5) > reseau-etoile-resistances-reparties.plot/' $DIR/reseau-etoile-resistances-reparties.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift reseau-etoile-resistances-reparties.plot 1e7
rm $DIR/reseau-etoile-resistances-reparties.plot

