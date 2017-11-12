SOURCES: src/mst.cc

mst:
	g++ $SOURCES -o mst
make clean:
	rm -rf *o mst
