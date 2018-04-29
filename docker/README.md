# Docker Containers for Boost Development #

These containers have all the tools and libraries necessary
to completely build and debug boost.  Currently this is targeting Unix builds.
There are three scripts each provided for Windows and for Unix:

* **build** will build a docker image
* **clean** will clean up the docker cruft that accumulates
* **run** will give you a shell prompt in a container

The build and run commands take two arguments, distribution and codename.
When building, the images will be named `boost:<dist>-<codename>`.
The following environments are currently provided:

| distribution | codename | version   | clang | gcc   | java   | openssl | python |
| :----------- | :------- | :-------- | :---- | :---- | :----- | :------ | :----- |
| debian       | stretch  | 9.4       | 3.8   | 6.3.0 | 8u171  | 1.1.0f  | 3.5.3  |
| debian       | buster   | testing   | 6.0.1 | 7.3.0 | 11     | 1.1.0h  | 3.6.6  |
| ubuntu       | xenial   | 16.04 LTS | 3.8   | 5.4.0 | 8u171  | 1.0.2g  | 3.5.2  |
| ubuntu       | bionic   | 18.04 LTS | 6.0.0 | 7.3.0 | 10.0.1 | 1.1.0g  | 3.6.5  |

## Unix ##

1. Install docker-ce.
2. Build the docker container, for example:

        ~/boost$ docker/build.sh ubuntu bionic

3. Run the docker container, for example:

        ~/boost$ docker/run.sh ubuntu bionic

4. Follow normal build instructions: bootstrap, b2 headers, and b2.
5. Exit from the docker bash shell when done.

## Windows (targeting Unix) ##

1. Install Docker for Windows
2. Share the drive your boost sources are on.
3. Build the docker container, for example:

        PS C:\Users\Me\workspace\boost> docker\build.ps1 -distro ubuntu -version bionic
        ...
        Successfully tagged boost:ubuntu-bionic

4. Run the docker container, for example:

        PS C:\Users\Me\workspace\boost> docker\run.ps1 -distro ubuntu -version bionic
        boost@0e89ea6e8ca9:/boost$ ./bootstrap.sh --with-python=python3
        Building Boost.Build engine with toolset gcc... tools/build/src/engine/bin.linuxx86_64/b2
        Detecting Python version... 3.6
        Detecting Python root... /usr
        Unicode/ICU support for Boost.Regex?... /usr
        Generating Boost.Build configuration in project-config.jam...

        Bootstrapping is done. To build, run:

            ./b2

        To adjust configuration, edit 'project-config.jam'.

5. Run "./b2 headers"
6. Run "./b2 -q -j4" to build boost; see other documentation for more options.

## Clean ##

Run the clean script occasionally to get rid of crufty containers.
