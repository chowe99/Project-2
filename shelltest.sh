#/bin/bash
rm -rf dir1
#
mkdir dir1
date > dir1/file1
cal  > dir1/file2
#echo "new" > dir1/file10
#
sleep 1
mkdir dir1/dir2
ls   > dir1/dir2/file3
#echo "NEWEST" > dir1/dir2/file10
#
echo "before syncing:"
tree dir1
tree dir1/dir2
echo "____________________________"
#
./mysync -r dir1 dir1/dir2
echo "after syncing:"
tree dir1
tree dir1/dir2
printf "\n"
echo "two datetimes should exist:"
cat dir1/file1
cat dir1/dir2/file1
printf "\n"
printf "Output should be\nNEWEST\nNEWEST\ngot:\n"
cat dir1/file10
cat dir1/dir2/file10

#
rm -rf dir1
