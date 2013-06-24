test:
	g++ -g test.cpp
	sh -c './a.out > log ; less log'

full-test:
	g++ -g test.cpp
	sh -c './a.out full > log ; less log'

.PHONY test:
.PHONY full-test:
