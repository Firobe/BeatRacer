#!/bin/bash
#Usage : osuConvert.sh [input file] [output ID]

mkdir tmp
mv "$1" tmp/
cd tmp
unzip "$1"
dir2ogg -Q *.mp3
mv *.ogg ../res/songs/$2.ogg
echo '[Name]' > ../res/tracks/$2.trk
echo $1 | sed -r 's/\.osz//g' | sed -r 's/[0-9]+ (.+)/\1/' >> ../res/tracks/$2.trk
echo '[Difficulties]' >> ../res/tracks/$2.trk
ls *.osu | sed 's/ //g' | sed -r 's/.*\[(.+)\].*/\1/' >> ../res/tracks/$2.trk
c=0
for i in *.osu; do
	../converter < "$i" > "../res/map/$2_$c.nt";
	let "c=c+1"
done
SIZE=`tail -1 ../res/map/$2_0.nt | sed -r 's/(.+),/\1/'`
echo 1,0,0:`echo $SIZE + 100 | bc -l` > ../res/map/$2.map
cd ..
rm -Rf tmp
