#/bin/bash
rm -rf dir1
#
mkdir dir1
date > dir1/file1
cal  > dir1/file2
echo "new" > dir1/file3
#
sleep 1
mkdir dir1/dir2
mkdir dir1/dir3
mkdir dir1/dir3/dir3
mkdir dir1/dir3/dir3/dir3
mkdir dir1/dir10
mkdir dir1/dir10/dir10
echo "success" > dir1/dir10/dir10/duptest
mkdir dir1/dir4
mkdir dir1/dir4/dir5
mkdir dir1/dir4/dir5/dir6
echo "nested_file" > dir1/dir4/dir5/dir6/nested
# ls   > dir1/dir2/file3
echo "NEWEST" > dir1/dir2/file3
#
echo "before syncing:"
tree dir1
tree dir1/dir2
echo "____________________________"
#
./mysync -v -r dir1 dir1/dir2
echo "after syncing:"
tree dir1
tree dir1/dir2
printf "\n"
echo "two datetimes should exist:"
cat dir1/file1
cat dir1/dir2/file1
printf "\n"
printf "Output should be\nNEWEST\nNEWEST\ngot:\n"
cat dir1/file3
cat dir1/dir2/file3
printf "output should be 'nested_file': "
cat dir1/dir2/dir4/dir5/dir6/nested
printf "none of the dir3s should be synced\n"
printf "output should be 'success': "
cat dir1/dir2/dir10/dir10/duptest

#
rm -rf dir1
