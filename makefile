CPP=g++
SHOW=cat
CPPFLAGS=-Wall -O2 -std=c++11
OUTPUTFLAGS=-o
OUTPUT=leviathan
RUN=./
SOURCE=./source
TESTS=./levTests/source
TESTLIBS=./levTests/libs

leviathan: $(SOURCE)/evaluator.cpp $(SOURCE)/types.cpp $(SOURCE)/builtin.hh $(SOURCE)/environment.hh $(SOURCE)/evaluation.hh $(SOURCE)/extern.hh $(SOURCE)/listOperations.hh $(SOURCE)/parser.hh $(SOURCE)/lexer.hh $(SOURCE)/lexeme.hh $(SOURCE)/types.hh
	$(CPP) $(CPPFLAGS) $(SOURCE)/evaluator.cpp $(SOURCE)/types.cpp $(OUTPUTFLAGS) $(OUTPUT)

compile: build
build: leviathan
dpl: build

cat-run-all: build cat-error1 run-error1 cat-error2 run-error2 cat-error3 run-error3 cat-arrays run-arrays cat-conditionals run-conditionals cat-recursion run-recursion cat-iteration run-iteration cat-functions run-functions cat-dictionary run-dictionary

cat-error1:
	$(SHOW) $(TESTS)/error1.lv
	@echo "\n"
	
run-error1: build
	$(RUN)$(OUTPUT) $(TESTS)/error1.lv
	
cat-error2:
	$(SHOW) $(TESTS)/error2.lv
	@echo "\n"
	
run-error2: build
	$(RUN)$(OUTPUT) $(TESTS)/error2.lv

cat-error3:
	$(SHOW) $(TESTS)/error3.lv
	@echo "\n"
	
run-error3: build
	$(RUN)$(OUTPUT) $(TESTS)/error3.lv
	
cat-arrays:
	$(SHOW) $(TESTS)/arrays.lv
	@echo "\n"
	
run-arrays: build
	$(RUN)$(OUTPUT) $(TESTS)/arrays.lv

cat-conditionals:
	$(SHOW) $(TESTS)/conditionals.lv
	@echo "\n"
	
run-conditionals: build
	$(RUN)$(OUTPUT) $(TESTS)/conditionals.lv
	
cat-recursion:
	$(SHOW) $(TESTS)/recursion.lv
	@echo "\n"
	
run-recursion: build
	$(RUN)$(OUTPUT) $(TESTS)/recursion.lv
	
cat-iteration:
	$(SHOW) $(TESTS)/iteration.lv
	@echo "\n"
	
run-iteration: build
	$(RUN)$(OUTPUT) $(TESTS)/iteration.lv

cat-functions:
	$(SHOW) $(TESTS)/functions.lv
	@echo "\n"
	
run-functions: build
	$(RUN)$(OUTPUT) $(TESTS)/functions.lv
	
cat-dictionary:
	$(SHOW) $(TESTS)/dictionary.lv
	@echo "\n"
	
run-dictionary: build
	$(RUN)$(OUTPUT) $(TESTS)/dictionary.lv

cat-problem:
	$(SHOW) $(TESTLIBS)/Cons.lv
	@echo "\n"
	$(SHOW) $(TESTLIBS)/PriorityQueue.lv
	@echo "\n"
	$(SHOW) $(TESTLIBS)/Agenda.lv
	@echo "\n"
	$(SHOW) $(TESTLIBS)/Wire.lv
	@echo "\n"
	$(SHOW) $(TESTLIBS)/Adder.lv
	@echo "\n"
	$(SHOW) $(TESTS)/TestProblem.lv
	@echo "\n"
	
run-problem: build
	$(RUN)$(OUTPUT) $(TESTS)/TestProblem.lv
	
clean:
	rm $(OUTPUT)