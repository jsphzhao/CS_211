build:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp buildings.cpp footway.cpp footways.cpp busstop.cpp busstops.cpp dist.cpp curl_util.cpp -Wno-unused-variable -Wno-unused-function -lcurl

run:
	./a.out

valgrind:
	rm -f ./a.out
	g++ -std=c++17 -g -Wall main.cpp building.cpp node.cpp nodes.cpp osm.cpp tinyxml2.cpp buildings.cpp footway.cpp footways.cpp busstop.cpp busstops.cpp dist.cpp curl_util.cpp -Wno-unused-variable -Wno-unused-function -lcurl
	valgrind --tool=memcheck --leak-check=full ./a.out

clean:
	rm -f ./a.out

submit:
	/home/cs211/w2024/tools/project05 submit *.cpp *.h
