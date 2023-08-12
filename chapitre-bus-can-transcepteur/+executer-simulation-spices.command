#!/bin/sh
set -x
DIR=`dirname $0` &&
cd $DIR &&

sed 's/plot v(6)/print v(6) > etoile-1-resistance.plot/' $DIR/etoile-1-resistance.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift etoile-1-resistance.plot 1e7
rm $DIR/etoile-1-resistance.plot

sed 's/plot v(6)/print v(6) > etoile-1-resistance-branches-inegales.plot/' $DIR/etoile-1-resistance-branches-inegales.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift etoile-1-resistance-branches-inegales.plot 1e7
rm $DIR/etoile-1-resistance-branches-inegales.plot

sed 's/plot v(6)/print v(6) > etoile-resistances-reparties.plot/' $DIR/etoile-resistances-reparties.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift etoile-resistances-reparties.plot 1e7
rm $DIR/etoile-resistances-reparties.plot

sed 's/plot v(6)/print v(6) > etoile-resistances-reparties-branches-inegales.plot/' $DIR/etoile-resistances-reparties-branches-inegales.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift etoile-resistances-reparties-branches-inegales.plot 1e7
rm $DIR/etoile-resistances-reparties-branches-inegales.plot

