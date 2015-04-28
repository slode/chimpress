file="$1" || "datafiles/datafile.txt"

cp $file tmp0
time ./bin/rle_enc < tmp0 > tmp1
time ./bin/rle_dec < tmp1 > tmp2
diff $file tmp2
ls -Flah | grep tmp
rm tmp0 tmp1 tmp2

cp $file tmp0
time ./bin/lzw_enc < tmp0 > tmp1
time ./bin/lzw_dec < tmp1 > tmp2
diff $file tmp2
ls -Flah | grep tmp
#xxd tmp0
#xxd tmp1
#xxd tmp2
rm tmp0 tmp1 tmp2

cp $file tmp0
time ./bin/rle_enc < tmp0 | ./bin/lzw_enc > tmp1
time ./bin/lzw_dec < tmp1 | ./bin/rle_dec > tmp2
diff $file tmp2
ls -Flah | grep tmp
#xxd tmp0
#xxd tmp1
#xxd tmp2
rm tmp0 tmp1 tmp2
