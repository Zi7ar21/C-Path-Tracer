all:
	${CC} pathtrace.c -o render -lm -O3
	${CC} volumetric-pathtrace.c -o volumerender -lm -O3