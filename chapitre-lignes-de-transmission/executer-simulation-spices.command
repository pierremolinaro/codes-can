#!/bin/sh
set -x
DIR=`dirname $0` &&
cd $DIR &&

sed 's/plot v(2)/print v(2) > ligne-sans-perte-ouverte.plot/' $DIR/ligne-sans-perte-ouverte.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-sans-perte-ouverte.plot 1e7
rm $DIR/ligne-sans-perte-ouverte.plot

sed 's/plot v(2)/print v(2) > ligne-sans-perte-terminee-par-rc.plot/' $DIR/ligne-sans-perte-terminee-par-rc.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-sans-perte-terminee-par-rc.plot 5e7
rm $DIR/ligne-sans-perte-terminee-par-rc.plot

sed 's/plot v(2)/print v(2) > ligne-sans-perte-terminee-par-rc-sur-2.plot/' $DIR/ligne-sans-perte-terminee-par-rc-sur-2.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-sans-perte-terminee-par-rc-sur-2.plot 5e7
rm $DIR/ligne-sans-perte-terminee-par-rc-sur-2.plot

sed 's/plot v(2)/print v(2) > ligne-sans-perte-terminee-par-2rc.plot/' $DIR/ligne-sans-perte-terminee-par-2rc.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-sans-perte-terminee-par-2rc.plot 5e7
rm $DIR/ligne-sans-perte-terminee-par-2rc.plot


sed 's/plot v(2)/print v(2) > ligne-avec-perte-ouverte.plot/' $DIR/ligne-avec-perte-ouverte.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-avec-perte-ouverte.plot 1e7
rm $DIR/ligne-avec-perte-ouverte.plot

sed 's/plot v(2)/print v(2) > ligne-avec-perte-terminee-par-rc.plot/' $DIR/ligne-avec-perte-terminee-par-rc.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-avec-perte-terminee-par-rc.plot 5e7
rm $DIR/ligne-avec-perte-terminee-par-rc.plot

sed 's/plot v(2)/print v(2) > ligne-avec-perte-terminee-par-rc-sur-2.plot/' $DIR/ligne-avec-perte-terminee-par-rc-sur-2.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-avec-perte-terminee-par-rc-sur-2.plot 5e7
rm $DIR/ligne-avec-perte-terminee-par-rc-sur-2.plot

sed 's/plot v(2)/print v(2) > ligne-avec-perte-terminee-par-2rc.plot/' $DIR/ligne-avec-perte-terminee-par-2rc.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ligne-avec-perte-terminee-par-2rc.plot 5e7
rm $DIR/ligne-avec-perte-terminee-par-2rc.plot

sed 's/plot v(2)/print v(2) > fermeture-sur-ligne-sans-perte-ouverte.plot/' $DIR/fermeture-sur-ligne-sans-perte-ouverte.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift fermeture-sur-ligne-sans-perte-ouverte.plot 5e6
rm $DIR/fermeture-sur-ligne-sans-perte-ouverte.plot


sed 's/plot v(2)/print v(2) > fermeture-sur-ligne-sans-perte-ouverte-avec-diodes.plot/' $DIR/fermeture-sur-ligne-sans-perte-ouverte-avec-diodes.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift fermeture-sur-ligne-sans-perte-ouverte-avec-diodes.plot 5e6
rm $DIR/fermeture-sur-ligne-sans-perte-ouverte-avec-diodes.plot

sed 's/plot v(2)/print v(2) > ouverture-sur-ligne-sans-perte-ouverte.plot/' $DIR/ouverture-sur-ligne-sans-perte-ouverte.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ouverture-sur-ligne-sans-perte-ouverte.plot 5e6
rm $DIR/ouverture-sur-ligne-sans-perte-ouverte.plot

sed 's/plot v(2)/print v(2) > fermeture-sur-ligne-sans-perte-ouverte-5k.plot/' $DIR/fermeture-sur-ligne-sans-perte-ouverte-5k.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift fermeture-sur-ligne-sans-perte-ouverte-5k.plot 5e6
rm $DIR/fermeture-sur-ligne-sans-perte-ouverte-5k.plot

sed 's/plot v(2)/print v(2) > ouverture-sur-ligne-sans-perte-ouverte-5k.plot/' $DIR/ouverture-sur-ligne-sans-perte-ouverte-5k.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ouverture-sur-ligne-sans-perte-ouverte-5k.plot 5e6
rm $DIR/ouverture-sur-ligne-sans-perte-ouverte-5k.plot

sed 's/plot v(2)/print v(2) > fermeture-sur-ligne-sans-perte-rc.plot/' $DIR/fermeture-sur-ligne-sans-perte-rc.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift fermeture-sur-ligne-sans-perte-rc.plot 5e6
rm $DIR/fermeture-sur-ligne-sans-perte-rc.plot

sed 's/plot v(2)/print v(2) > ouverture-sur-ligne-sans-perte-rc.plot/' $DIR/ouverture-sur-ligne-sans-perte-rc.cir > f.cir
/Applications/MacSpice.app/contents/MacOS/MacSpice -n -b $DIR/f.cir
rm $DIR/f.cir
$DIR/extraction-plot-spice.swift ouverture-sur-ligne-sans-perte-rc.plot 5e6
rm $DIR/ouverture-sur-ligne-sans-perte-rc.plot
