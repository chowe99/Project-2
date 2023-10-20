#/bin/bash
rm -rf dir1
rm -rf seconddir
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
mkdir seconddir
echo "secondSuccess" > seconddir/secondfile 
#
echo "before syncing:"
tree dir1
tree dir1/dir2
tree seconddir
echo "____________________________"
#
./mysync -v -r -o "*duptest*" -o "*nested*" dir1 dir1/dir2 seconddir
echo "after syncing:"
tree dir1
tree dir1/dir2
tree seconddir
printf "\n"


printf "two cats should fail\n"
cat dir1/file1
cat dir1/dir2/file1
cat seconddir/file1
printf "\n"
printf "one cat should fail\n"
cat dir1/file3
cat dir1/dir2/file3
cat seconddir/file3

printf "output should be \nnested_file\nnested_file\nnested_file\n\ngot:\n"
cat dir1/dir4/dir5/dir6/nested
cat seconddir/dir4/dir5/dir6/nested
cat dir1/dir2/dir4/dir5/dir6/nested

printf "two cats should fail\n"
cat seconddir/secondfile
cat dir1/secondfile
cat dir1/dir2/secondfile

#
rm -rf dir1
rm -rf seconddir
