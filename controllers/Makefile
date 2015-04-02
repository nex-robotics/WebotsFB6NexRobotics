TARGETS=fire_bird_6_obstacle_avoidance fire_bird_6_sensor_acquisition

.PHONY: $(TARGETS)

release debug profile clean:
	@+for X in $(TARGETS); do \
	  echo "# make" $@ $$X; \
	  make -s -C $$X $@ ; \
	done
	
