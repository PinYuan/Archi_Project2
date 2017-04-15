pipeline:main.o simulate.o ALU.o errorDetect.o instruction.o memory.o regFile.o pipelineReg.o piperpt.o hazard.o
	g++ -std=c++11 -g -o pipeline main.o simulate.o ALU.o errorDetect.o instruction.o memory.o regFile.o pipelineReg.o piperpt.o hazard.o
main.o:main.cpp
	g++ -std=c++11 -g -c main.cpp
simulate.o:simulate.cpp
	g++ -std=c++11 -g -c simulate.cpp
ALU.o:ALU.cpp
	g++ -std=c++11 -g -c ALU.cpp
errorDetect.o:errorDetect.cpp
	g++ -std=c++11 -g -c errorDetect.cpp
instruction.o:instruction.cpp
	g++ -std=c++11 -g -c instruction.cpp
memory.o:memory.cpp
	g++ -std=c++11 -g -c memory.cpp
regFile.o:regFile.cpp
	g++ -std=c++11 -g -c regFile.cpp
pipelineReg.o:pipelineReg.cpp
	g++ -std=c++11 -g -c pipelineReg.cpp
piperpt.o:piperpt.cpp
	g++ -std=c++11 -g -c piperpt.cpp
hazard.o:hazard.cpp
	g++ -std=c++11 -g -c hazard.cpp
clean:
	rm -f pipeline main.o simulate.o ALU.o errorDetect.o instruction.o memory.o regFile.o pipelineReg.o piperpt.o hazard.o 

