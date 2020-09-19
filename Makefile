all:
	cd CVersion && make all
	cd POC && make all

clean:
	cd CVersion && make clean
	cd POC && make clean