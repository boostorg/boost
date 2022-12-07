# escape=`

# Use the latest Windows Server Core image with .NET Framework 4.8.
FROM mcr.microsoft.com/dotnet/framework/sdk:4.8-windowsservercore-ltsc2019

# Restore the default Windows shell for correct batch processing.
SHELL ["cmd", "/S", "/C"]

# Download the Build Tools bootstrapper.
ADD https://aka.ms/vs/16/release/vs_buildtools.exe C:\TEMP\vs_buildtools.exe

RUN C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache `
    --installPath C:\BuildTools `
    --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended `
 || IF "%ERRORLEVEL%"=="3010" EXIT 0

# --------------
# Boost Section
# --------------

WORKDIR C:\
RUN powershell -Command iex ((new-object net.webclient).DownloadString('https://chocolatey.org/install.ps1'))
RUN powershell -Command Invoke-Expression (New-Object System.Net.WebClient).DownloadString('https://get.scoop.sh')
RUN choco install -y git.install
RUN choco install -y python --version 3.8.3

# chocolaty install of openssl 1.1.1
# RUN choco install -y openssl --x86 --version 1.1.1.700
# RUN mklink /D "OpenSSL" "Program Files (x86)\\OpenSSL-Win32"

# scoop install of openssl 1.0.2u
# RUN powershell -Command scoop install openssl@1.0.2u -a 32bit -g
# RUN mklink /D "OpenSSL" "ProgramData\\scoop\\apps\\openssl\\current"

# scoop install of openssl 1.1.1g
RUN powershell -Command scoop install openssl@1.1.1g -a 32bit -g
RUN mklink /D "OpenSSL" "ProgramData\\scoop\\apps\\openssl\\current"

RUN mkdir C:\devel

WORKDIR C:\devel
RUN git.exe clone -b develop https://github.com/boostorg/boost.git

WORKDIR C:\devel\boost
RUN git.exe submodule update --init --recursive
COPY . C:\devel\boost\libs\beast\
COPY ".dockers\\windows-vs-32\\user-config.jam" C:\devel\boost\
COPY ".dockers\\windows-vs-32\\tests.bat" C:\devel\boost\

RUN C:\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat && bootstrap.bat
RUN C:\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat && b2 variant=release cxxstd=latest headers

# Choose one of the following two lines. The second will succeed regardless of the test results, if you'd prefer the build to complete.
# RUN C:\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat && set OPENSSL_ROOT=C:/OpenSSL&& tests.bat
RUN C:\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat && set OPENSSL_ROOT=C:/OpenSSL&& tests.bat || ver>nul

ENTRYPOINT ["C:\\BuildTools\\VC\\Auxiliary\\Build\\vcvars32.bat", "&&", "powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]
