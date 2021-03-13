# Zero Hanami -- 0hana/C Makefile
# Invoke in containing directory

UPDATE := /home
SOURCE := $(shell find Source -type f -name *.c)
OBJECT := $(patsubst Source/%.c,Build/%.o,$(SOURCE))
DIRECT := $(dir $(OBJECT))
DEPEND := $(OBJECT:.o=.d)

.PHONY: update docker container
update:
ifeq "$(DOCKER)" 'CONTAINED'
	@touch Build Failures
	@printf "\nInitiating Build ... " && sleep 1 && printf "Building! \n\n"
	@time -f %E make -j $(OBJECT)
	@\
	UPDATES="";\
	for FUNCTION in $(OBJECT); do \
		if [ $$(date -d "$$(stat -c %z Build | awk '{ print $$1 " " $$2 }')" +%s) -lt $$(date -d "$$(stat -c %z $${FUNCTION} | awk '{ print $$1 " " $$2 }')" +%s) ]; then\
			UPDATES="$${UPDATES}$$(basename -s .o $${FUNCTION}) ";\
		fi;\
	done;\
	printf "\nUpdates ($$(echo $${UPDATES} | wc -w)): $${UPDATES}\n\n";\
	make UPDATES="$${UPDATES}" 0hana/run

# To add later:
# In the files not among the updates, check for the presense of any updated function being called -- any single one updated function is sufficient
# If such a function call is found in those non-updated files, run (pass to main) each of those affected (non-updated) files too for regression testing
else
	@docker cp . hanami:$(UPDATE)
	@printf "Initiating dependency reconstruction ... Reconstructing!\n\n"
	@docker exec -w $(UPDATE) hanami sh -c 'mkdir -p $(DIRECT); make DOCKER=CONTAINED update'
endif

clean:
	@docker exec -w $(UPDATE) hanami sh -c 'rm -rf *'

docker:
	@printf "FROM alpine:3 as compile\nRUN apk add coreutils cunit cunit-dev gcc libc-dev make tree valgrind\n" | docker build -t hanami -

container:
	@docker run -dti --rm --name hanami hanami

0hana/run: $(OBJECT)
	@printf "\nInitiating Link ... Linking!\n\n"
	@echo gcc $^ -Wall -Wextra -g -o $@ -lcunit
	@time -f %E gcc $^ -Wall -Wextra -g -o $@ -lcunit
	@valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes 0hana/run $${UPDATES}

Build/%.o: Source/%.c Build/%.d # (e_e)
	gcc $< -Wall -Wextra -g -c -o $@

Build/%.d: Source/%.c
	cpp $< -MMD -MF $@ -MT "$(@:.d=.o) $@" > /dev/null

ifeq "$(DOCKER)" 'CONTAINED'
include $(DEPEND)
endif
