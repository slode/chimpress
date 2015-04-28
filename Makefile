default:
	mkdir -p bin
	$(CC) -O2 src/rle_dec.c -o bin/rle_dec
	$(CC) -O2 src/rle_enc.c -o bin/rle_enc
	$(CXX) -O2 src/lzw_enc.c -o bin/lzw_enc
	$(CXX) -O2 src/lzw_dec.c -o bin/lzw_dec
