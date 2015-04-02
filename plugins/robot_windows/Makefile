TARGETS=fire_bird_6_window

.PHONY: $(TARGETS)

release debug profile clean:
	@+for X in $(TARGETS); do \
	  echo "# make" $@ $$X; \
	  make -s -C $$X $@ ; \
	done
	
