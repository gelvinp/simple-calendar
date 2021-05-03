CXX = g++
FLAGS = -Wall -g -std=c++17

calendar: StringUtils.o FileUtils.o Time24.o Errand.o Day.o Calendar.o Menu.o main.o
	$(CXX) $(FLAGS) -o calendar $^

Time24.o: Time24.cpp Time24.h StringUtils.h
	$(CXX) $(FLAGS) -c -o $@ $<

Errand.o: Errand.cpp Errand.h Time24.h StringUtils.h
	$(CXX) $(FLAGS) -c -o $@ $<

StringUtils.o: StringUtils.cpp StringUtils.h
	$(CXX) $(FLAGS) -c -o $@ $<

FileUtils.o: FileUtils.cpp FileUtils.h StringUtils.h
	$(CXX) $(FLAGS) -c -o $@ $<

Day.o: Day.cpp Day.h
	$(CXX) $(FLAGS) -c -o $@ $<

Calendar.o: Calendar.cpp Calendar.h StringUtils.h
	$(CXX) $(FLAGS) -c -o $@ $<

Menu.o: Menu.cpp Menu.h Calendar.h Day.h Errand.h FileUtils.h StringUtils.h
	$(CXX) $(FLAGS) -c -o $@ $<

main.o: main.cpp Menu.h
	$(CXX) $(FLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm *.o calendar
