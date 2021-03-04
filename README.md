# C
0hana C Utilities
## Docker container setup
Requires on a [`Linux`](https://github.com/torvalds/linux) host the installation of [`docker`](https://docs.docker.com/get-started/) and [`make`](https://www.gnu.org/software/make/), your user a member of the `docker` group 
```
$ make docker container utilities
```
### Compiling (and testing) code in the Docker container
```
$ make
```
## Adding code
Place new feature code in its appropriate [`Source/<directory>`](Source)  
Place new testing code in its appropriate [`Source/CUnit/<directory>`](Source/CUnit)

That's it.
