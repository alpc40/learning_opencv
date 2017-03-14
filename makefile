path=-I /usr/local/include/opencv -L /usr/local/lib -lm -lopencv_core -lopencv_highgui

source=$(S)
target=$(T)

$(target):$(source)
	g++ -o $(target) $(source) $(path)

clean:
	rm $(target)
