all:
	(cd timerLib; make install)
	(cd lcdLib; make install)
	(cd wakedemo; make)
    #(cd msp430-pong; make)
#	(cd circledemo; make)

doc:
	rm -rf doxygen_docs
	doxygen Doxyfile
clean:
	(cd timerLib; make clean)
	(cd lcdLib; make clean)
#	(cd circledemo; make clean)
	(cd wakedemo; make clean)
#   (cd msp430-pong; make clean)
	rm -rf lib h
	rm -rf doxygen_docs/*
