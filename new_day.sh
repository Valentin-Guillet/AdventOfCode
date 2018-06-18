
day=$1

if [ -z $day ]
then
	day=`ls | egrep -o "[0-9]+" | sort -nr | head -1`
	((day++))
fi

mkdir "Day$day"
cp Day0/Day0.cpp "Day$day/Day$day.cpp" 
