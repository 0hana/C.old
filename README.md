# C
0hana C Utilities
## Docker container setup
Requires on a [`Linux`](https://github.com/torvalds/linux) host the installation of [`docker`](https://docs.docker.com/get-started/) and [`make`](https://www.gnu.org/software/make/), your user a member of the `docker` group 
```
$ make docker container
```
### Compiling (and testing) code in the Docker container
```
$ make
```
## Adding code
Place new code files in their appropriate [`Source/<directory>`](Source)  
Prefix CUnit file and associated function names with `CU_`

That's it.
