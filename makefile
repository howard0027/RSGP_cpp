CC = g++
VERSION = c++17
OP = O3
DEBUG_FLAG =         # assign -g for debugging
MTHREAD = -fopenmp

OBJS = main.o container.o crossover.o grammar.o parm.o random.o fitness.o functions.o data.o mrmr.o ast.o progsimp.o evaluation.o adaptation.o evolution.o

main : ${OBJS}
	${CC} ${MTHREAD} ${DEBUG_FLAG} -std=${VERSION} -${OP} ${OBJS} -o $@

main.o : main.cpp
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} main.cpp

container.o : container.cpp container.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} container.cpp

ast.o : ast.cpp ast.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} ast.cpp

crossover.o : crossover.cpp crossover.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} crossover.cpp

grammar.o : grammar.cpp grammar.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} grammar.cpp

#mrmr_window.o : mrmr_window.cpp mrmr_window.h
#	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} mrmr_window.cpp

parm.o : parm.cpp parm.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} parm.cpp


random.o : random.cpp random.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} random.cpp

fitness.o : fitness.cpp fitness.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} fitness.cpp

functions.o : functions.cpp functions.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} functions.cpp

data.o : data.cpp data.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} data.cpp

mrmr.o : mrmr.cpp mrmr.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} mrmr.cpp

progsimp.o : progsimp.cpp progsimp.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} progsimp.cpp

evaluation.o : evaluation.cpp evaluation.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} evaluation.cpp

adaptation.o : adaptation.cpp adaptation.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} adaptation.cpp

evolution.o : evolution.cpp evolution.h
	${CC} ${MTHREAD} -c ${DEBUG_FLAG} -std=${VERSION} -${OP} evolution.cpp

clean :
	rm main.o
	rm mrmr.o
	rm container.o
	rm crossover.o
	rm grammar.o
	rm data.o
	rm functions.o
	rm random.o
	rm fitness.o
	rm parm.o
	rm ast.o
	rm progsimp.o
	rm evaluation.o
	rm adaptation.o
	rm evolution.o

