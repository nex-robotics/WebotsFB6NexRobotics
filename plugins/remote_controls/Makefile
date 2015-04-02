TARGETS=fire_bird_6_remote_control

.PHONY: $(TARGETS)

release debug profile clean:
	@+for X in $(TARGETS); do \
	  echo "# make" $@ $$X; \
	  make -s -C $$X $@ ; \
	done
	
