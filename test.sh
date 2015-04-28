file="$1" || "datafiles/datafile.txt"

cp $file tmp0
time ./src/rle_enc < tmp0 > tmp1
time ./src/rle_dec < tmp1 > tmp2
diff $file tmp2
ls -Flah | grep tmp
rm tmp0 tmp1 tmp2

cp $file tmp0
time ./src/lzw_enc < tmp0 > tmp1
time ./src/lzw_dec < tmp1 > tmp2
diff $file tmp2
ls -Flah | grep tmp
#xxd tmp0
#xxd tmp1
#xxd tmp2
rm tmp0 tmp1 tmp2

cp $file tmp0
time ./src/rle_enc < tmp0 | ./src/lzw_enc > tmp1
time ./src/lzw_dec < tmp1 | ./src/rle_dec > tmp2
diff $file tmp2
ls -Flah | grep tmp
#xxd tmp0
#xxd tmp1
#xxd tmp2
rm tmp0 tmp1 tmp2
