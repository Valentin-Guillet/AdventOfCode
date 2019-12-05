
day=$1

if [ -z $day ]
then
	day=`ls | egrep -o "[0-9]+" | sort -nr | head -1`
	((day++))
fi

mkdir "Day$day"
sed "s/Day0Input/Day"$day"Input/" Day0/Day0.cpp > "Day$day/Day$day.cpp" 
ln -s ../Day0/main "Day$day/main"
