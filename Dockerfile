FROM alpine:3.13.2 as compiler
RUN apk add gcc libc-dev make cunit cunit-dev cunit-doc bash valgrind
