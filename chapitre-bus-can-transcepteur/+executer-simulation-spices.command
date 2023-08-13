#!/bin/sh
set -x
DIR=`dirname $0` &&
cd $DIR &&

sed 's/plot v(6)/print v(6) > dom-etoile-1-resistance.plot/' $DIR/dom-etoile-1-resistance.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift dom-etoile-1-resistance.plot 1e7
rm $DIR/dom-etoile-1-resistance.plot

sed 's/plot v(6)/print v(6) > dom-etoile-resistances-reparties.plot/' $DIR/dom-etoile-resistances-reparties.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift dom-etoile-resistances-reparties.plot 1e7
rm $DIR/dom-etoile-resistances-reparties.plot

sed 's/plot v(6)/print v(6) > dom-etoile-1-resistance-branches-inegales.plot/' $DIR/dom-etoile-1-resistance-branches-inegales.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift dom-etoile-1-resistance-branches-inegales.plot 1e7
rm $DIR/dom-etoile-1-resistance-branches-inegales.plot

sed 's/plot v(6)/print v(6) > dom-etoile-resistances-reparties-branches-inegales.plot/' $DIR/dom-etoile-resistances-reparties-branches-inegales.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift dom-etoile-resistances-reparties-branches-inegales.plot 1e7
rm $DIR/dom-etoile-resistances-reparties-branches-inegales.plot




sed 's/plot v(6)/print v(6) > rec-etoile-1-resistance.plot/' $DIR/rec-etoile-1-resistance.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift rec-etoile-1-resistance.plot 1e7
rm $DIR/rec-etoile-1-resistance.plot


sed 's/plot v(6)/print v(6) > rec-etoile-resistances-reparties.plot/' $DIR/rec-etoile-resistances-reparties.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift rec-etoile-resistances-reparties.plot 1e7
rm $DIR/rec-etoile-resistances-reparties.plot


sed 's/plot v(6)/print v(6) > rec-etoile-1-resistance-branches-inegales.plot/' $DIR/rec-etoile-1-resistance-branches-inegales.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift rec-etoile-1-resistance-branches-inegales.plot 1e7
rm $DIR/rec-etoile-1-resistance-branches-inegales.plot

sed 's/plot v(6)/print v(6) > rec-etoile-resistances-reparties-branches-inegales.plot/' $DIR/rec-etoile-resistances-reparties-branches-inegales.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift rec-etoile-resistances-reparties-branches-inegales.plot 1e7
rm $DIR/rec-etoile-resistances-reparties-branches-inegales.plot

