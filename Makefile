.PHONY: all test run clean

all:
	$(MAKE) -C c all

test:
	$(MAKE) -C c test

run:
	$(MAKE) -C c run

clean:
	$(MAKE) -C c clean
