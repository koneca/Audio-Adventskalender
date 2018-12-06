#!/bin/bash
# script by flo, 2017

function convert_dir {
    idx=0
    srcdir=`ls "$1" | grep "^$3"`
    if [ "$srcdir" != "" ]; then
	if [ -d "$2/$3" ]; then
	    rm -rf "$2/$3/*"
	else
	    mkdir "$2/$3"
	fi
	
	idx=0
	declare -a pids

	echo "Directory '$1/$srcdir'.."
	
	cd "$1/$srcdir"
	for f in *; do
	    echo "  converting '$f'.."
	    sox --buffer 131072 --multi-threaded --no-glob "$f" --clobber -r32000 -b 16 -e signed-integer --no-glob "../../$2/$3/$idx.WAV" remix - gain -n -1.5 bass +1 loudness -1 pad 0 0 dither &
	    pids[$idx]=$!
	    idx=$(($idx+1))
	done
	cd - >/dev/null

	# wait for processes to finish
	for pid in ${pids[*]}; do
	    echo -en "\r  waiting for $idx background processes to finish.. "
	    wait $pid
	    idx=$(($idx-1))
	done
	echo "done."
	unset pids
    fi
}

if [ ! $# -eq "2" ]; then
    echo "Usage: $0  "
    exit 1
fi

if [ ! -d "$2" ]; then
    # create destination directory if not exists
    mkdir "$2"
fi

for i in `seq 0 8`; do
    convert_dir "$1" "$2" $i
done

echo "Final output size: `du -sh --si \"$2\" | sed 's/\t.*//'`"