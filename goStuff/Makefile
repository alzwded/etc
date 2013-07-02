GOC = ../go/bin/go
#GOFLAGS = build # doesn't output a thing. Woot
GOFLAGS = install
GOPATH = /home/jakkal/Projects/goStuff/
GOROOT = /home/jakkal/Projects/go/
DEPS = src/test1/vector/vector.go src/test1/test.go src/test1/vector/interfaces.go

bin/test1: $(DEPS)
	bash -c "export GOROOT=$(GOROOT) ; \
		export GOPATH=$(GOPATH) ; \
		$(GOC) $(GOFLAGS) test1"

test-vector: $(DEPS)
	bash -c "export GOROOT=$(GOROOT) ; \
		export GOPATH=$(GOPATH) ; \
		$(GOC) test test1/vector"

test-all: test-vector

clean:
	rm -f bin/test1
	find pkg/ -name test1\* -exec rm -rf {} \;

.PHONY: test-all test-vector
