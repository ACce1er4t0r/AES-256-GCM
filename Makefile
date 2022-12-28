default:
	# tar xzvf openssl-1.1.1l.tar.gz
	# cd openssl-1.1.1l && ./config && make && make install
	gcc ufrec.c utils.c crypto.c -o ufrec -lcrypto
	gcc ufsend.c utils.c crypto.c -o ufsend -lcrypto
	
test:
	cp ufsend testfile
    ./ufsend testfile -l
    rm testfile
    ./ufrec testfile.ufsec -l
    diff testfile ufsend

clean:
	rm ufrec
	rm ufsend
	rm example.txt.ufsec