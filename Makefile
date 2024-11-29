.PHONY: all clean zip

PROJECT_DIRS := $(shell find . -maxdepth 1 -type d -name 'project*' -printf '%P\n')

all:
	@for dir in $(PROJECT_DIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	@for dir in $(PROJECT_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	$(RM) -r cs31.zip cs31/

zip:
	$(MAKE) clean
	rm -f cs31.zip
	mkdir cs31
	cp -r $(PROJECT_DIRS) cs31/
	zip -rq cs31.zip cs31
	rm -rf cs31
