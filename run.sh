./build.sh $@ &&
[ "$1" = "u" ] && ./build/Luntik ${@:2} || ./build/Luntik $@
# echo $@ | grep "" && ./build/Luntik ${@:2} || ./build/Luntik $@
