all: clean
	# generate
	cd client && $(MAKE)
	cd input && $(MAKE)
	cd server && $(MAKE)
	# create dir
	mkdir out
	# move generated
	mv client/client out/
	mv input/input out/
	mv server/server out/
	# copy resources
	cp client/font_2.bmp out/
	cp server/config.txt out/
	cp server/backup.txt out/

.PHONY: clean
clean:
	rm -rf out

