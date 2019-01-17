all: create fileconverter naiveinterface indexer srchindx

create:
	gcc -o create create.c

fileconverter:
	gcc -o fileconverter fileconverter.c

naiveinterface:
	gcc -o naiveinterface naiveinterface.c input.c interfaceFunctions.c

indexer:
	gcc -o indexer indexer.c

srchindx:
	gcc -o srchindx srchindx.c

clean:
	rm -f *.o create fileconverter naiveinterface indexer srchindx
