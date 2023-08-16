[ "$1" = "u" ] && { 
  [ -d libs/SFML ] && ( cd libs/SFML; git pull ) || git clone https://github.com/SFML/SFML.git libs/SFML
  [ -d libs/spdlog ] && ( cd libs/spdlog; git pull ) || git clone https://github.com/gabime/spdlog.git libs/spdlog
}

cmake -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug .
cd build
make -j8
