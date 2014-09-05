Getting Started with Modular Boost
=====================================

This information is only relevant for these who intend to use boost library 
forked or cloned from GitHub. These steps are not necessary if you obtained 
code from official distribution in archived form (.gz, .zip and etc.).
                                                                    
Installing Modular Boost
------------------------

The initial cloning will take at least 45 minutes on a 3.0 mbps Internet 
connection and will consume roughly 1.5 GB of disk space. Cloning is only 
done once, so this lengthy initial time is not a long-term concern. 

Once cloning completes include files needs to be assembled in .\boost directory.

From the command line on Windows:
```dos
git clone --recursive git@github.com:boostorg/boost.git modular-boost >clone.log
cd modular-boost
bootstrap.bat
b2.exe headers
```

On POSIX-like operating systems:

```dos
git clone --recursive git@github.com:boostorg/boost.git modular-boost >clone.log
cd modular-boost
./bootstrap.sh
./b2 headers
```


For more information:
---------------------
https://svn.boost.org/trac/boost/wiki/ModularBoost<br>
http://www.boost.org/more/getting_started/index.html
