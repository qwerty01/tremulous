all: msg CmdParser # COM_Parse 

CmdParser: CmdParser.cpp ../CmdParser.h
	c++ -g -O0 -std=c++14 -I ../../../external/catch -I ../.. CmdParser.cpp -o CmdParser 

msg: msg.cpp ../msg.cpp ../msg.h ../huffman.h
	c++ -g -O0 -std=c++14 -I ../.. -I ../../../external/catch -I ../.. ../huffman.cpp ../msg.cpp msg.cpp -o msg


#COM_Parse: COM_Parse.cpp ../q_shared.cpp ../q_shared.h
#	c++ -g -O0 -std=c++14 -I ../../../external/catch -I ../.. ../q_shared.cpp COM_Parse.cpp -o COM_Parse 

clean:
	rm -rf COM_Parse
	rm -rf CmdParser
	rm -rf msg
	rm -rf *.dSYM
