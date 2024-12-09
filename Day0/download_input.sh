#!/bin/bash

cookie_file=${XDG_CACHE_HOME:-$HOME/.cache}/AOC_session_cookie

if [ ! -f $cookie_file ]; then
	read -p "Please input your AOC session cookie: " session_cookie
	echo $session_cookie > $cookie_file
else
	session_cookie=$(cat $cookie_file)
fi

if [ -z "$1" ]; then
	echo "Usage: $0 [day_dir] (e.g Day7/)"
	exit 1
fi

year=$(basename $(pwd))
day=${1#Day}
day=${day%/}
curl -s https://adventofcode.com/$year/day/$day/input --cookie "session=$session_cookie" -o Day$day/input
